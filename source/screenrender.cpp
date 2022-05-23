#include <screenrender.h>


screenRender::screenRender(DataIOHandler *dioh, QWidget *parent) : QWidget(parent) {
    hoverActive = false;
    brushLoc = QPoint(0,0);
    flashFlag = false;
    flasher = new QTimer(this);
    connect(flasher, SIGNAL(timeout()), this, SLOT(toggleFlasher()));
    fgVisible = true;
    adder = 0.1f;
    samplePoint = QPoint(-1000, -1000);
    ioh = dioh;
    filter.setFilter("Greyscale");
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    radius = -1;
    yStart = 0;
    setCursor(CrossCursor);
}

screenRender::~screenRender() {
    flasher->stop();
    delete flasher;
}

void screenRender::setMode(EditMode emode) {
    mode = emode;
    if (mode != Brush_Mode) {
        resume();
        setCursor(CrossCursor);
    }
    else {
        stopFlashing();
        setCursor(QCursor(QPixmap::fromImage(QImage(1, 1, QImage::Format_ARGB32_Premultiplied)), -1, -1));
    }
}

void screenRender::setHoverActive(bool active) {
    hoverActive = active;
}

void screenRender::updateHoverMap(int r, const unsigned char const* const* arr) {
    //return;
    //fix this
    stopFlashing();
    hoverLock.lock();
    int size = 2 * radius + 1;
    if (hoverMap != nullptr && radius != -1) {
        for (int i = 0; i < size; ++i)
            delete [] hoverMap[i];
        delete [] hoverMap;
    }
    radius = r;
    size = 2 * radius + 1;
    hoverMap = new unsigned char*[size];
    for (int i = 0; i < size; ++i) {
        hoverMap[i] = new unsigned char[size];
        for (int j = 0; j < size; ++j)
            hoverMap[i][j] = arr[i][j];
    }
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            if (hoverMap[i][j] == 1) {
                int n = j == 0 ? 0 : hoverMap[i][j - 1];
                int w = i == 0 ? 0 : hoverMap[i - 1][j];
                hoverMap[i][j] = min(n, w) + 1;
            }
        }
    for (int i = size - 1; i >= 0; --i)
        for (int j = size - 1; j >= 0; --j) {
            if (arr[i][j] == 0)
                hoverMap[i][j] = 0;
            else {
                int s = j == size - 1 ? 0 : hoverMap[i][j + 1];
                int e = i == size - 1 ? 0 : hoverMap[i + 1][j];
                int val = min(s, e) + 1;
                hoverMap[i][j] = min(static_cast<int>(hoverMap[i][j]), val);
            }
        }
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            if (hoverMap[i][j] != 1)
                hoverMap[i][j] = 0;
    hoverLock.unlock();
    if (mode != Brush_Mode)
        resume();
}

void screenRender::mouseMoveEvent(QMouseEvent *event) {
    event->ignore();
    if (mode == Brush_Mode) {
        brushLoc = getZoomCorrected(event->pos());
        repaint();
    }
}

double screenRender::getZoom() {
    return screenZoom.getZoom();
}

void screenRender::setZoom(double Zoom){
    screenZoom.setZoom(Zoom);
    doZoom();
}

void screenRender::zoomIn() {
    screenZoom.zoomIn();
    doZoom();
}

void screenRender::zoomOut() {
    screenZoom.zoomOut();
    doZoom();
}

QPoint screenRender::getZoomCorrected(QPoint qp) {
    return screenZoom.getZoomCorrected(qp);
}

void screenRender::doZoom() {
    if (!fgPrescaled.isNull())
        fgLayers.convertFromImage(screenZoom.zoomImg(fgPrescaled));
    else
        fgLayers = QPixmap();
    if (!bgPrescaled.isNull())
        bgLayers.convertFromImage(bgPrescaled);//bgLayers.convertFromImage(screenZoom.zoomImg(bgPrescaled));
    else
        bgLayers = QPixmap();
    if (!isDrawing)
        repaint();
}

void screenRender::showPts() {
    flashFlag = true;
}

void screenRender::toggleFlasher() {
    flashFlag = !flashFlag;
    if (!isDrawing)
        repaint();
}

void screenRender::updateViews() {
    flasher->stop();
    workLayer = ioh->getWorkingLayer();
    if (workLayer != nullptr) {
        bgPrescaled = ioh->getBackground();
        fgPrescaled = ioh->getForeground();
        doZoom();
        alphaVal = static_cast<unsigned int>(workLayer->getAlpha()) << 24;
        yEnd = workLayer->getCanvas()->height();
        yMid = yEnd / 2;
    }
    if (mode != Brush_Mode)
        flasher->start(flashSpeed);
}

void screenRender::paintEvent(QPaintEvent *event) {
    long long time = stdFuncs::getTime();
    isDrawing = true;
    if (ioh->wasUpdated())
        updateViews();
    if (workLayer == nullptr)
        return;
    qi = QImage(workLayer->getRenderCanvas().size(), QImage::Format_ARGB32);
    QPainter qp;
    qp.begin(&qi);
    if (!bgLayers.isNull())
        qp.drawPixmap(0, 0, bgLayers);
    if (workLayer->isVisible()) {
        qp.drawImage(0, 0, workLayer->getRenderCanvas());
        alphaVal = static_cast<unsigned int>(workLayer->getAlpha()) << 24;
        setFixedSize(screenZoom.getZoomCorrected(qi.size()));
        int w = qi.width(), h = qi.height();
        vector <list <Triangle> > tris = workLayer->getTriangles();
        vector <SplineVector> vects = workLayer->getVectors();
        vector <unsigned char> activeVects = workLayer->getActiveVectors();
        int alpha = workLayer->getAlpha();
        for (size_t i = 0; i < tris.size(); ++i) {
            pair <QRgb, QRgb> colors = vects[i].getColors();
            QColor ca = QColor (colors.first), cb = QColor(colors.second);
            ca.setAlpha(alpha);
            short band = vects[i].getBand(), gap = vects[i].getGap();
            float totalTri = static_cast<float>(tris[i].size() / 2);
            float offset = (totalTri / static_cast<float>(band + gap));
            int styler = -static_cast<int>(((static_cast<float>(band + gap) * offset) - totalTri) / 2.0);
            if (vects[i].getMode() == ColorFill) {
                if (colors.first == colors.second) {
                    color = ca.rgba();
                    for (Triangle &t : tris[i]) {
                        if (styler >= 0)
                            fillTri(t);
                        styler += 1;
                        if (styler == band - 1)
                            styler = -gap;
                    }
                }
                else {
                    cb.setAlpha(alpha);
                    float ccomp = 1.0 / static_cast<float>(tris[i].size());
                    float cnt = 0.0;
                    for (Triangle &t : tris[i]) {
                        float ccc = ccomp * cnt;
                        int r = static_cast<int>((ccc * static_cast<float>(ca.red())) + ((1.0 - ccc) * static_cast<float>(cb.red())));
                        int g = static_cast<int>((ccc * static_cast<float>(ca.green())) + ((1.0 - ccc) * static_cast<float>(cb.green())));
                        int b = static_cast<int>((ccc * static_cast<float>(ca.blue())) + ((1.0 - ccc) * static_cast<float>(cb.blue())));
                        color = QColor(r, g, b, alpha).rgba();
                        if (styler >= 0)
                            fillTri(t);
                        styler += 1;
                        if (styler == band - 1)
                            styler = -gap;
                        cnt += 1.0;
                    }
                }
            }
            else {
                filter = vects[i].getFilter();
                for (Triangle &t : tris[i]) {
                    if (styler >= 0)
                        filterTri(t);
                    styler += 1;
                    if (styler == band - 1)
                        styler = -gap;
                }
            }
        }
        int symCreate = workLayer->symActive();
        vector <Polygon> gons = workLayer->getPolgons();
        vector <unsigned char> activeGons = workLayer->getActiveGons();
        int index = 0;
        for (Polygon gon : gons) {
            list <Triangle> tris = gon.getTris();
            vector <QPoint> pts = gon.getPts();
            color = gon.getPolyColor();
            if (gon.getPolyMode() == ColorGon && color >= 0x01000000)
                for (Triangle &t : tris)
                    fillTri(t);
            else if (gon.getPolyMode() == FilterGon) {
                filter = gon.getFilter();
                for (Triangle &t : tris)
                    filterTri(t);
            }
            int edgeSize = gon.getEdgeSize();
            if (edgeSize != 0) {
                bool dispDivs = gon.getShowDivs();
                qp.setPen(QPen(QColor(gon.getEdgeColor()), edgeSize, Qt::SolidLine, Qt::RoundCap));
                QVector <QLine> lines;
                if (dispDivs) {
                    for (Triangle &t : tris) {
                        qp.drawLine(t.a, t.b);
                        qp.drawLine(t.a, t.c);
                        qp.drawLine(t.c, t.b);
                    }
                }
                else {
                    if (pts.size() >= 2) {
                        for (int i = 1; i < pts.size(); ++i)
                            lines.push_back(QLine(pts[i - 1], pts[i]));
                        lines.push_back(QLine(pts[0], pts[pts.size() - 1]));
                    }
                }
                qp.drawLines(lines);
            }
            ++index;
        }
        vector <DrawText> texts = workLayer->getTexts();
        vector <unsigned char> activeTexts = workLayer->getActiveTexts();
        for (DrawText &dt : texts) {
            qp.setPen(QPen(dt.getColor(), 1));
            qp.setFont(dt.getFont());
            qp.setTransform(dt.getTransform());
            qp.drawStaticText(0, 0, dt.getText());
        }
        if (mode == Polygon_Mode && samplePoint != QPoint(-1000, -1000)) {
            qp.setPen(QPen(QColor(128, 128, 128), 5));
            qp.drawEllipse(samplePoint, samplePoint.x() - 2, samplePoint.y() - 2);
        }
        list <vector<QPoint>> ptsToDraw;
        if (mode == Polygon_Mode)
            for (unsigned char i : activeGons)
                ptsToDraw.push_back(gons[i].getPts());
        else if (mode == Spline_Mode) {
            if (activeVects.size() > 0) {
                for (unsigned char i = 0; i < static_cast<unsigned char>(symCreate ? 1 : activeVects.size()); ++i)
                    ptsToDraw.push_back(vects[activeVects[i]].getControls());
            }
        }
        else if (mode == Text_Mode)
            for (unsigned char i : activeTexts)
                ptsToDraw.push_back(vector<QPoint>(1, texts[i].getShowPoint()));
        else if (mode == Raster_Mode)
            ptsToDraw.push_back(workLayer->getRasterEdges());
        ptsToDraw.push_back(vector<QPoint>(1, samplePoint));
        for (vector <QPoint> &pts : ptsToDraw) {
            for (QPoint pt : pts) {
                for (int j = max(0, pt.y() - ptSize); j <= min(pt.y() + ptSize, h - 1); ++j) {
                    QRgb *line = reinterpret_cast<QRgb *>(qi.scanLine(j));
                    for (int i = max(pt.x() - ptSize, 0); i <= min(pt.x() + ptSize, w - 1); ++i) {
                        int dist = abs(i - pt.x()) + abs(j - pt.y());
                        if ((flashFlag || dist >= ptSize - 1) && dist <= ptSize) {
                            QRgb c = line[i];
                            if (!(c & 0xFF000000))
                                c = 0xFFFFFFFF;
                            else
                                c |= 0xFF000000;
                            line[i] = graphics::Filtering::negative(graphics::Filtering::polarize(c, 128), 255);
                        }
                    }
                }
            }
        }
        if (underMouse() && hoverActive && mode == Brush_Mode && hoverLock.try_lock()) {
            QPoint qp = brushLoc;
            brushLoc = QPoint(brushLoc.x() + 1, brushLoc.y() + 7);
            int x = brushLoc.x() < radius ? radius - brushLoc.x() - 1 : 0, yStarter = brushLoc.y() < radius ? radius - brushLoc.y() : 0;
            for (int i = max(0, brushLoc.x() - radius); i <= min(qi.width() - 1, brushLoc.x() + radius + (brushLoc.x() < radius ? 1 : 0)); ++i) {
                int y = yStarter;
                for (int j = max(0, brushLoc.y() - radius); j <= min(qi.height() - 1, brushLoc.y() + radius); ++j) {
                    if (hoverMap[x][y] == 1) {
                        QRgb c = qi.pixel(i, j);
                        if (!(c & 0xFF000000))
                            c = 0xFFFFFFFF;
                        else
                            c |= 0xFF000000;
                        qi.setPixel(i, j, graphics::Filtering::negative(graphics::Filtering::polarize(c, 128), 255));
                    }
                    ++y;
                }
                ++x;
            }
            hoverLock.unlock();
            brushLoc = qp;
        }
        graphics::ImgSupport::applyAlpha(&qi, &yStart, &yEnd, &alphaVal);
    }
    qp.end();
    qp.begin(this);
    qp.drawImage(0, 0, screenZoom.zoomImg(qi));
    if (fgVisible && !fgLayers.isNull())
        qp.drawPixmap(0, 0, fgLayers);
    isDrawing = false;
    //cout << stdFuncs::getTime(time) << endl;
}

void screenRender::setSamplePt(QPoint qp) {
    samplePoint = qp;
}

void screenRender::stopFlashing() {
    flasher->stop();
    flashFlag = false;
    if (!isDrawing)
        repaint();
}

void screenRender::resume() {
    flashFlag = true;
    flasher->start(flashSpeed);
}

void screenRender::showFg(bool shown) {
    fgVisible = shown;
}

void screenRender::fillTri(Triangle t) {
    QPoint a = t.a, b = t.b, c = t.c;
    if (a.y() > b.y()) {
        QPoint tmp = a;
        a = b;
        b = tmp;
    }
    if (b.y() > c.y()) {
        QPoint tmp = b;
        b = c;
        c = tmp;
        if (a.y() > b.y()) {
            tmp = a;
            a = b;
            b = tmp;
        }
    }
    if (b.y() == a.y())
        fillBTri(c, b, a);
    else if (b.y() == c.y())
        fillTTri(a, b, c);
    else {
        QPoint d (a.x() + static_cast<float>(c.x() - a.x()) * (static_cast<float>(b.y() - a.y()) / static_cast<float>(c.y() - a.y())) , b.y());
        fillBTri(c, b, d);
        fillTTri(a, b, d);
    }
}

void screenRender::fillBTri(QPoint a, QPoint b, QPoint c) {
    if (b.x() > c.x()) {
        QPoint tmp = b;
        b = c;
        c = tmp;
    }
    int h = qi.height() - 1, w = qi.width() - 1;
    float invslope1 = static_cast<float>(b.x() - a.x()) / static_cast<float>(b.y() - a.y());
    float invslope2 = static_cast<float>(c.x() - a.x()) / static_cast<float>(c.y() - a.y());
    float curx1 = static_cast<float>(a.x());
    float curx2 = static_cast<float>(a.x());
    float offset = a.y() > h ? a.y() - h : 0;
    curx1 -= invslope1 * offset;
    curx2 -= invslope2 * offset;
    for (int y = min(h, a.y()); y >= max(0, b.y()); --y) {
        QRgb *line = reinterpret_cast<QRgb *>(qi.scanLine(y));
        for (int x = max(static_cast<int>(curx1), 0); x <= min(static_cast<int>(curx2), w); ++x)
            line[x] = color;
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void screenRender::fillTTri(QPoint a, QPoint b, QPoint c) {
    if (b.x() > c.x()) {
        QPoint tmp = b;
        b = c;
        c = tmp;
    }
    float invslope1 = static_cast<float>(b.x() - a.x()) / static_cast<float>(b.y() - a.y());
    float invslope2 = static_cast<float>(c.x() - a.x()) / static_cast<float>(c.y() - a.y());
    float curx1 = static_cast<float>(a.x());
    float curx2 = static_cast<float>(a.x());
    float offset = a.y() < 0 ? -a.y() : 0;
    curx1 += invslope1 * offset;
    curx2 += invslope2 * offset;
    int h = qi.height(), w = qi.width() - 1;
    for (int y = max(0, a.y()); y < min(h, b.y()); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(qi.scanLine(y));
        for (int x = max(static_cast<int>(curx1), 0); x <= min(static_cast<int>(curx2), w); ++x)
            line[x] = color;
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void screenRender::filterTri(Triangle t) {
    QPoint a = t.a, b = t.b, c = t.c;
    if (a.y() > b.y()) {
        QPoint tmp = a;
        a = b;
        b = tmp;
    }
    if (b.y() > c.y()) {
        QPoint tmp = b;
        b = c;
        c = tmp;
        if (a.y() > b.y()) {
            tmp = a;
            a = b;
            b = tmp;
        }
    }
    if (b.y() == a.y())
        filterBTri(c, b, a);
    else if (b.y() == c.y())
        filterTTri(a, b, c);
    else {
        QPoint d (a.x() + static_cast<float>(c.x() - a.x()) * (static_cast<float>(b.y() - a.y()) / static_cast<float>(c.y() - a.y())) , b.y());
        filterBTri(c, b, d);
        filterTTri(a, b, d);
    }
}

void screenRender::filterBTri(QPoint a, QPoint b, QPoint c) {
    if (b.x() > c.x()) {
        QPoint tmp = b;
        b = c;
        c = tmp;
    }
    int h = qi.height() - 1, w = qi.width() - 1;
    float invslope1 = static_cast<float>(b.x() - a.x()) / static_cast<float>(b.y() - a.y());
    float invslope2 = static_cast<float>(c.x() - a.x()) / static_cast<float>(c.y() - a.y());
    float curx1 = static_cast<float>(a.x());
    float curx2 = static_cast<float>(a.x());
    float offset = a.y() > h ? a.y() - h : 0;
    curx1 -= invslope1 * offset;
    curx2 -= invslope2 * offset;
    for (int y = min(h, a.y()); y >= max(0, b.y()); --y) {
        QRgb *line = reinterpret_cast<QRgb *>(qi.scanLine(y));
        for (int x = max(static_cast<int>(curx1), 0); x <= min(static_cast<int>(curx2), w); ++x)
            line[x] = filter.applyTo(line[x]);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void screenRender::filterTTri(QPoint a, QPoint b, QPoint c) {
    if (b.x() > c.x()) {
        QPoint tmp = b;
        b = c;
        c = tmp;
    }
    float invslope1 = static_cast<float>(b.x() - a.x()) / static_cast<float>(b.y() - a.y());
    float invslope2 = static_cast<float>(c.x() - a.x()) / static_cast<float>(c.y() - a.y());
    float curx1 = static_cast<float>(a.x());
    float curx2 = static_cast<float>(a.x());
    float offset = a.y() < 0 ? -a.y() : 0;
    curx1 += invslope1 * offset;
    curx2 += invslope2 * offset;
    int h = qi.height(), w = qi.width() - 1;
    for (int y = max(0, a.y()); y < min(h, b.y()); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(qi.scanLine(y));
        for (int x = max(static_cast<int>(curx1), 0); x <= min(static_cast<int>(curx2), w); ++x)
            line[x] = filter.applyTo(line[x]);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}
