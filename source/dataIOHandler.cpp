#include "dataIOHandler.h"


DataIOHandler::DataIOHandler(QProgressDialog *qpd) {
    progress = qpd;
    activeLayer = 0;
    updated = true;
}

DataIOHandler::~DataIOHandler() {
    for (size_t i = 0; i < frame.size(); ++i)
        delete frame[i];
}

void DataIOHandler::setDims(QSize size) {
    dims = size;
}

QSize DataIOHandler::getdims() {
    return dims;
}

void DataIOHandler::compileFrame() {
    QImage *qi = new QImage(frame[0]->getCanvas()->size(), QImage::Format_ARGB32);
    qi->fill(0x00FFFFFF);
    progress->setLabelText("Compiling Frame");
    renderFrame(progress, qi, frame);
    frame[activeLayer]->clearAll();
    Layer *layer = frame[activeLayer];
    for (size_t i = frame.size() - 1; i > activeLayer; --i) {
        delete frame[i];
        frame.pop_back();
    }
    frame.pop_back();
    for (int i = static_cast<int>(frame.size() - 1); i >= 0; --i) {
        delete frame[i];
        frame.pop_back();
    }
    frame.push_back(layer);
    activeLayer = 0;
    layer->setAlpha(255);
    QImage *img = layer->getCanvas();
    img->fill(0x00FFFFFF);
    QPainter qp;
    qp.begin(img);
    qp.drawImage(0, 0, *qi);
    qp.end();
    delete qi;
    updated = true;
}

void DataIOHandler::compileLayer() {
    Layer *layer = getWorkingLayer();
    layer->deselect();
    progress->setLabelText("Compiling Layer");
    renderLayer(nullptr, progress, layer->getCanvas(), layer);
    layer->clearAll();
}

void DataIOHandler::renderFrame(QProgressDialog *fqpd, QImage *ret, vector <Layer *> layers) {
    progressMarker = 0;
    fqpd->setValue(0);
    fqpd->setMaximum(static_cast<int>(layers.size() + 2));
    fqpd->show();
    QCoreApplication::processEvents();
    ret->fill(0xFFFFFFFF);
    vector <QImage *> imgs;
    for (size_t i = 0; i < layers.size(); ++i)
        imgs.push_back(new QImage(layers[i]->getCanvas()->copy()));
    vector <thread> imgThreads;
    for(size_t i = 0; i < layers.size(); ++i) {
        thread th = thread(renderLayer, fqpd, nullptr, imgs[i], layers[i]);
        imgThreads.push_back(move(th));
    }
    ++progressMarker;
    fqpd->setValue(progressMarker);
    QCoreApplication::processEvents();
    for(auto &th : imgThreads)
        th.join();
    QPainter qp;
    qp.begin(ret);
    for (size_t i = 0; i < imgs.size(); ++i) {
        qp.drawImage(0, 0, *imgs[i]);
        delete imgs[i];
    }
    qp.end();
    ++progressMarker;
    fqpd->setValue(progressMarker);
    fqpd->close();
    fqpd->hide();
}

void DataIOHandler::renderLayer(QProgressDialog *fqpd, QProgressDialog *qpd, QImage *toProcess, Layer *layer) {
    int alpha = layer->getAlpha();
    vector <SplineVector> vects = layer->getVectors();
    vector <list <Triangle>> tris = layer->getTriangles();
    vector <Polygon> gons = layer->getPolgons();
    vector <DrawText> texts = layer->getTexts();
    Filter filter = layer->getFilter();
    if (qpd != nullptr) {
        qpd->setValue(0);
        qpd->setMaximum(static_cast<int>(1 + vects.size() + gons.size() + texts.size() + (tris.size() != vects.size() ? 1 : 0)));
        qpd->show();
        QCoreApplication::processEvents();
    }
    vector <list <Triangle> *> dTris;
    if (!tris.empty()) {
        for (size_t i = 0; i < vects.size(); ++i)
            dTris.push_back(new list <Triangle> (tris[i]));
    }
    else if (vects.size() != 0) {
        for (size_t i = 0; i < vects.size(); ++i)
            dTris.push_back( new list <Triangle> ());
        vector <thread> splineThreads;
        for(size_t i = 0; i < dTris.size(); ++i) {
            thread th = thread(calcLine, vects[i], dTris[i]);
            splineThreads.push_back(move(th));
        }
        for(auto &th : splineThreads)
            th.join();
        if (qpd != nullptr) {
            qpd->setValue(qpd->value() + 1);
            QCoreApplication::processEvents();
        }
    }
    QRgb color;
    for (size_t i = 0; i < dTris.size(); ++i) {
        Filter vf = vects[i].getFilter();
        pair <QRgb, QRgb> colors = vects[i].getColors();
        QColor ca = QColor (colors.first), cb = QColor(colors.second);
        short band = vects[i].getBand(), gap = vects[i].getGap();
        float totalTri = static_cast<float>(tris[i].size() / 2);
        float offset = (totalTri / static_cast<float>(band + gap));
        int styler = -static_cast<int>(((static_cast<float>(band + gap) * offset) - totalTri) / 2.0);
        if (vects[i].getMode() == ColorFill) {
            if (colors.first == colors.second) {
                color = ca.rgba();
                for (Triangle &t : *dTris[i]) {
                    if (styler >= 0)
                        fillTri(toProcess, t, color);
                    styler += 1;
                    if (styler == band - 1)
                        styler = -gap;
                }
            }
            else {
                float ccomp = 1.0 / static_cast<float>((*dTris[i]).size());
                float cnt = 0.0;
                for (Triangle &t : *dTris[i]) {
                    float ccc = ccomp * cnt;
                    int r = static_cast<int>((ccc * static_cast<float>(ca.red())) + ((1.0 - ccc) * static_cast<float>(cb.red())));
                    int g = static_cast<int>((ccc * static_cast<float>(ca.green())) + ((1.0 - ccc) * static_cast<float>(cb.green())));
                    int b = static_cast<int>((ccc * static_cast<float>(ca.blue())) + ((1.0 - ccc) * static_cast<float>(cb.blue())));
                    color = QColor(r, g, b).rgba();
                    if (styler >= 0)
                        fillTri(toProcess, t, color);
                    styler += 1;
                    if (styler == band - 1)
                        styler = -gap;
                    cnt += 1.0;
                }
            }
        }
        else
            for (Triangle &t : *dTris[i]) {
                if (styler >= 0)
                    filterTri(toProcess, t, vf);
                styler += 1;
                if (styler == band - 1)
                    styler = -gap;
            }
        if (qpd != nullptr) {
            qpd->setValue(qpd->value() + 1);
            QCoreApplication::processEvents();
        }
    }
    for (size_t i = 0; i < vects.size(); ++i)
        delete dTris[i];
    QPainter painter(toProcess);
    for (Polygon gon : gons) {
        list <Triangle> tris = gon.getTris();
        vector <QPoint> pts = gon.getPts();
        color = gon.getPolyColor();
        if (gon.getPolyMode() == ColorGon && color >= 0x01000000)
            for (Triangle &t : tris)
                fillTri(toProcess, t, color);
        else if (gon.getPolyMode() == FilterGon) {
            filter = gon.getFilter();
            for (Triangle &t : tris)
                filterTri(toProcess, t, color);
        }
        int edgeSize = gon.getEdgeSize();
        if (edgeSize != 0) {
            bool dispDivs = gon.getShowDivs();
            painter.setPen(QPen(QColor(gon.getEdgeColor()), edgeSize, Qt::SolidLine, Qt::RoundCap));
            QVector <QLine> lines;
            if (dispDivs) {
                for (Triangle &t : tris) {
                    painter.drawLine(t.a, t.b);
                    painter.drawLine(t.a, t.c);
                    painter.drawLine(t.c, t.b);
                }
            }
            else {
                if (pts.size() >= 2) {
                    for (int i = 1; i < pts.size(); ++i)
                        lines.push_back(QLine(pts[i - 1], pts[i]));
                    lines.push_back(QLine(pts[0], pts[pts.size() - 1]));
                }
            }
            painter.drawLines(lines);
        }
        if (qpd != nullptr) {
            qpd->setValue(qpd->value() + 1);
            QCoreApplication::processEvents();
        }
    }
    for (DrawText &dt : texts) {
        painter.setPen(QPen(dt.getColor(), 1));
        painter.setFont(dt.getFont());
        painter.setTransform(dt.getTransform());
        painter.drawStaticText(0, 0, dt.getText());
        if (qpd != nullptr) {
            qpd->setValue(qpd->value() + 1);
            QCoreApplication::processEvents();
        }
    }
    painter.end();
    if (qpd != nullptr) {
        qpd->setValue(qpd->value() + 1);
        QCoreApplication::processEvents();
    }
    filter.applyTo(toProcess);
    *toProcess = toProcess->convertToFormat(QImage::Format_ARGB32);
    unsigned int alphaVal = static_cast<unsigned int>(alpha) << 24;
    int yStart = 0, yEnd = toProcess->height();
    graphics::ImgSupport::applyAlpha(toProcess, &yStart, &yEnd, &alphaVal);
    if (qpd != nullptr) {
        qpd->close();
        qpd->hide();
    }
    if (fqpd != nullptr) {
        ++progressMarker;
        locker.lock();
        fqpd->setValue(progressMarker);
        QCoreApplication::processEvents();
        locker.unlock();
    }
}

void DataIOHandler::calcLine(SplineVector sv, list<Triangle> *tris) {
    vector <QPointF> workPts;
    vector <QPoint> controlPts = sv.getControls();
    char numpts = static_cast<char>(controlPts.size()) - 1;
    for (char i = 0; i < numpts + 1; ++i)
        workPts.push_back(QPointF(controlPts[i]));
    list <pair <QPoint, QPoint> > pairs;
    pair <unsigned char, unsigned char> taper = sv.getTaper();
    float taper1 = taper.first == 0 ? 0.0 : 1.0 / static_cast<float>((9 - (taper.first - 1)) + 1), taper2 = taper.second == 0 ? 0.0 : 1.0 / static_cast<float>((9 - (taper.second - 1)) + 1);
    for (float ipol = 0.0; ipol <= 1.0; ipol += ipolMin) {
        float twidth = static_cast<float>(sv.getWidth());
        if (taper.first != 0 || taper.second != 0) {
            if (sv.getTaperType() == Single)
                twidth *= pow(ipol, taper1);
            else {
                float f = 2.0 * abs(abs(ipol - 0.5) - 0.5);
                twidth *= pow(f, ipol <= 0.5 ? taper1 : taper2);
            }
        }
        for (int max = numpts; max > 1; --max) {    // og  > 0
            for (char i = 0; i < max; ++i) {
                workPts[i].setX(Layer::getipol(workPts[i].x(), workPts[i + 1].x(), ipol));
                workPts[i].setY(Layer::getipol(workPts[i].y(), workPts[i + 1].y(), ipol));
            }
            workPts[max] = controlPts[max];
        }
        float dx = workPts[1].x() - workPts[0].x(), dy = workPts[1].y() - workPts[0].y();
        workPts[0].setX(Layer::getipol(workPts[0].x(), workPts[1].x(), ipol));
        workPts[0].setY(Layer::getipol(workPts[0].y(), workPts[1].y(), ipol));
        workPts[1] = controlPts[1];
        float slope = dx / dy;
        float sqrSlope = slope * slope;
        QPointF midPt = workPts[0];
        float dist = twidth;
        float inverter = slope < 0.0 ? 1.0 : -1.0;
        int x1 = static_cast<int>(((inverter * dist) / sqrt(1 + sqrSlope)) + midPt.x());
        int y1 = static_cast<int>((dist / sqrt(1 + (1 / sqrSlope))) + midPt.y());
        int x2 = midPt.x() + (midPt.x() - x1);
        int y2 = midPt.y() + (midPt.y() - y1);
        pairs.push_back(pair <QPoint, QPoint> (QPoint(x1, y1), QPoint(x2, y2)));
        workPts[0] = controlPts[0];
    }
    pair <QPoint, QPoint> first = pairs.front();
    pairs.pop_front();
    int x, y;
    while (pairs.size() > 0) {
        pair <QPoint, QPoint> second = pairs.front();
        pairs.pop_front();
        x = first.first.x() - second.second.x();
        y = first.first.y() - second.second.y();
        int dist1 = x * x + y * y;
        x = first.second.x() - second.first.x();
        y = first.second.y() - second.first.y();
        int dist2 = x * x + y * y;
        if (dist1 > dist2) {
            tris->push_back(Triangle(first.first, second.second, first.second));
            tris->push_back(Triangle(first.first, second.second, second.first));
        }
        else {
            tris->push_back(Triangle(first.second, second.first, first.first));
            tris->push_back(Triangle(first.second, second.first, second.second));
        }
        first = second;
    }
}

void DataIOHandler::fillTri(QImage *toProcess, Triangle t, QRgb color) {
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
        fillBTri(toProcess, c, b, a, color);
    else if (b.y() == c.y())
        fillTTri(toProcess, a, b, c, color);
    else {
        QPoint d (a.x() + static_cast<float>(c.x() - a.x()) * (static_cast<float>(b.y() - a.y()) / static_cast<float>(c.y() - a.y())) , b.y());
        fillBTri(toProcess, c, b, d, color);
        fillTTri(toProcess, a, b, d, color);
    }
}

void DataIOHandler::fillBTri(QImage *toProcess, QPoint a, QPoint b, QPoint c, QRgb color) {
    if (b.x() > c.x()) {
        QPoint tmp = b;
        b = c;
        c = tmp;
    }
    int h = toProcess->height() - 1, w = toProcess->width() - 1;
    float invslope1 = static_cast<float>(b.x() - a.x()) / static_cast<float>(b.y() - a.y());
    float invslope2 = static_cast<float>(c.x() - a.x()) / static_cast<float>(c.y() - a.y());
    float curx1 = static_cast<float>(a.x());
    float curx2 = static_cast<float>(a.x());
    float offset = a.y() > h ? a.y() - h : 0;
    curx1 -= invslope1 * offset;
    curx2 -= invslope2 * offset;
    for (int y = min(h, a.y()); y >= max(0, b.y()); --y) {
        QRgb *line = reinterpret_cast<QRgb *>(toProcess->scanLine(y));
        for (int x = max(static_cast<int>(curx1), 0); x <= min(static_cast<int>(curx2), w); ++x)
            line[x] = color;
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void DataIOHandler::fillTTri(QImage *toProcess, QPoint a, QPoint b, QPoint c, QRgb color) {
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
    int h = toProcess->height(), w = toProcess->width() - 1;
    for (int y = max(0, a.y()); y < min(h, b.y()); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(toProcess->scanLine(y));
        for (int x = max(static_cast<int>(curx1), 0); x <= min(static_cast<int>(curx2), w); ++x)
            line[x] = color;
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void DataIOHandler::filterTri(QImage *toProcess, Triangle t, Filter f) {
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
        filterBTri(toProcess, c, b, a, f);
    else if (b.y() == c.y())
        filterTTri(toProcess, a, b, c, f);
    else {
        QPoint d (a.x() + static_cast<float>(c.x() - a.x()) * (static_cast<float>(b.y() - a.y()) / static_cast<float>(c.y() - a.y())) , b.y());
        filterBTri(toProcess, c, b, d, f);
        filterTTri(toProcess, a, b, d, f);
    }
}

void DataIOHandler::filterBTri(QImage *toProcess, QPoint a, QPoint b, QPoint c, Filter f) {
    if (b.x() > c.x()) {
        QPoint tmp = b;
        b = c;
        c = tmp;
    }
    int h = toProcess->height() - 1, w = toProcess->width() - 1;
    float invslope1 = static_cast<float>(b.x() - a.x()) / static_cast<float>(b.y() - a.y());
    float invslope2 = static_cast<float>(c.x() - a.x()) / static_cast<float>(c.y() - a.y());
    float curx1 = static_cast<float>(a.x());
    float curx2 = static_cast<float>(a.x());
    float offset = a.y() > h ? a.y() - h : 0;
    curx1 -= invslope1 * offset;
    curx2 -= invslope2 * offset;
    for (int y = min(h, a.y()); y >= max(0, b.y()); --y) {
        QRgb *line = reinterpret_cast<QRgb *>(toProcess->scanLine(y));
        for (int x = max(static_cast<int>(curx1), 0); x <= min(static_cast<int>(curx2), w); ++x)
            line[x] = f.applyTo(line[x]);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void DataIOHandler::filterTTri(QImage *toProcess, QPoint a, QPoint b, QPoint c, Filter f) {
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
    int h = toProcess->height(), w = toProcess->width() - 1;
    for (int y = max(0, a.y()); y < min(h, b.y()); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(toProcess->scanLine(y));
        for (int x = max(static_cast<int>(curx1), 0); x <= min(static_cast<int>(curx2), w); ++x)
            line[x] = f.applyTo(line[x]);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void DataIOHandler::setActiveLayer(int i, EditMode mode) {
    if (i > getNumLayers())
        return;
    activeLayer = static_cast<unsigned char>(i);
    getWorkingLayer()->setMode(mode);
    updated = true;
}

int DataIOHandler::getNumLayers() {
    return static_cast<int>(frame.size());
}

int DataIOHandler::getActiveLayer() {
    return activeLayer;
}

bool DataIOHandler::wasUpdated() {
    bool b = updated;
    updated = false;
    return b;
}

Layer * DataIOHandler::getWorkingLayer() {
    return frame.empty() ? nullptr : frame[activeLayer];
}

void DataIOHandler::addLayer() {
    frame.push_back(new Layer(dims));
    updated = true;
    activeLayer = static_cast<unsigned char>(frame.size() - 1);
}

void DataIOHandler::copyLayer() {
    layerCopySlot = Layer(*frame[activeLayer]);
}

void DataIOHandler::pasteLayer() {
    if (layerCopySlot.getCanvas()->isNull())
        return;
    frame.push_back(new Layer(layerCopySlot));
    updated = true;
    activeLayer = static_cast<unsigned char>(frame.size() - 1);
}

void DataIOHandler::deleteLayer() {
    unsigned char temp = activeLayer;
    if (activeLayer == frame.size() - 1)
        --activeLayer;
    delete frame[temp];
    frame.erase((frame.begin() + temp));
    updated = true;
}

void DataIOHandler::moveBackward() {
    if (activeLayer != 0) {
        swap(frame[activeLayer - 1], frame[activeLayer]);
        --activeLayer;
    }
    updated = true;
}

void DataIOHandler::moveForward() {
    if (activeLayer != frame.size() - 1) {
        swap(frame[activeLayer], frame[activeLayer + 1]);
        ++activeLayer;
    }
    updated = true;
}

void DataIOHandler::moveToFront() {
    while (activeLayer != frame.size() - 1) {
        swap(frame[activeLayer], frame[activeLayer + 1]);
        ++activeLayer;
    }
    updated = true;
}

void DataIOHandler::moveToBack() {
    while (activeLayer != 0) {
        swap(frame[activeLayer - 1], frame[activeLayer]);
        --activeLayer;
    }
    updated = true;
}

QImage DataIOHandler::getBackground() {
    if (activeLayer == 0)
        return QImage();
    progress->setLabelText("Updating Background View");
    progress->setMaximum(static_cast<int>(frame.size()));
    progress->show();
    QCoreApplication::processEvents();
    QImage qi = frame[0]->getCanvas()->copy();
    renderLayer(nullptr, nullptr, &qi, frame[0]);
    progress->setValue(0);
    QCoreApplication::processEvents();
    QPainter p;
    p.begin(&qi);
    for (size_t i = 0; i < activeLayer; ++i) {
        QImage temp = frame[i]->getCanvas()->copy();
        renderLayer(nullptr, nullptr, &temp, frame[i]);
        p.drawImage(0, 0, temp);
        progress->setValue(static_cast<int>(i + 1));
        QCoreApplication::processEvents();
    }
    p.end();
    if (progress->value() == static_cast<int>(frame.size() - 1)) {
        progress->close();
        progress->hide();
    }
    return qi;
}

QImage DataIOHandler::getForeground() {
    progress->setLabelText("Updating Foreground View");
    if (frame.size() == 0 || activeLayer == frame.size() - 1)
        return QImage();
    QImage qi = frame[activeLayer + 1]->getCanvas()->copy();
    renderLayer(nullptr, nullptr, &qi, frame[activeLayer + 1]);
    progress->setValue(progress->value() + 2);
    QCoreApplication::processEvents();
    QPainter p;
    p.begin(&qi);
    for (size_t i = activeLayer + 2; i < frame.size(); ++i) {
        QImage temp = frame[i]->getCanvas()->copy();
        renderLayer(nullptr, nullptr, &temp, frame[i]);
        p.drawImage(0, 0, temp);
        progress->setValue(static_cast<int>(i + 1));
        QCoreApplication::processEvents();
    }
    p.end();
    progress->close();
    progress->hide();
    return qi;
}

void DataIOHandler::copyText() {
    textCopySlot.clear();
    Layer *layer = getWorkingLayer();
    vector <unsigned char> activeTexts = layer->getActiveTexts();
    vector <DrawText> texts = layer->getTexts();
    for (unsigned char i : activeTexts)
        textCopySlot.push_back(texts[i]);
}

void DataIOHandler::cutText() {
    copyText();
    deleteText();
}

void DataIOHandler::pasteText() {
    getWorkingLayer()->pasteText(textCopySlot);
}

void DataIOHandler::deleteText() {
    getWorkingLayer()->deleteSelected();
}

void DataIOHandler::copyVectors() {
    vectorCopySlot.clear();
    Layer *layer = getWorkingLayer();
    vector <unsigned char> activeVectors = layer->getActiveVectors();
    vector <SplineVector> vects = layer->getVectors();
    for (unsigned char i : activeVectors)
        vectorCopySlot.push_back(vects[i]);
}

void DataIOHandler::cutVectors() {
    copyVectors();
    deleteVectors();
}

void DataIOHandler::deleteVectors() {
    getWorkingLayer()->deleteSelected();
}

void DataIOHandler::pasteVectors() {
    getWorkingLayer()->pasteVectors(vectorCopySlot);
}

void DataIOHandler::copyPolygons() {
    polygonCopySlot.clear();
    Layer *layer = getWorkingLayer();
    vector <unsigned char> activeGons = layer->getActiveGons();
    vector <Polygon> gons = layer->getPolgons();
    for (unsigned char i : activeGons)
        polygonCopySlot.push_back(gons[i]);
}

void DataIOHandler::cutPolygons() {
    copyPolygons();
    deletePolygons();
}

void DataIOHandler::deletePolygons() {
    getWorkingLayer()->deleteSelected();
}

void DataIOHandler::pastePolygons() {
    getWorkingLayer()->pastePolygons(polygonCopySlot);
}

void DataIOHandler::copyRaster() {
    Layer *layer = getWorkingLayer();
    rasterCopySlot = layer->getRaster();
    angleCopySlot = layer->getAngle();
    boundCopySlot = layer->getBounds();
}

void DataIOHandler::cutRaster() {
    copyRaster();
    deleteRaster();
}

void DataIOHandler::pasteRaster() {
    Layer *layer = getWorkingLayer();
    layer->deselect();
    layer->pasteRaster(rasterCopySlot, angleCopySlot, boundCopySlot);
}

void DataIOHandler::deleteRaster() {
    getWorkingLayer()->deleteSelected();
}

bool DataIOHandler::importImage(QString fileName) {
    importImg = QImage(fileName).convertToFormat(QImage::Format_ARGB32);
    importType = image;
    bool match = importImg.width() == dims.width() && importImg.height() == dims.height();
    if (match)
        scale(dontScale);
    return !match;
}


void DataIOHandler::exportImage(QString fileName) {
    QImage *out = new QImage(dims, QImage::Format_ARGB32);
    progress->setLabelText("Exporting Image");
    renderFrame(progress, out, frame);
    out->save(fileName);
}

void DataIOHandler::scale(scaleType type) {
    QImage toLayer(dims, QImage::Format_ARGB32);
    QImage toDraw;
    toLayer.fill(0x00FFFFFF);
    switch (type) {
    case dontScale:
        toDraw = importImg;
        break;
    case bestFit:
        if (importImg.scaledToHeight(toLayer.height()).width() > toLayer.width())
            toDraw = importImg.scaledToWidth(toLayer.width());
        else
            toDraw = importImg.scaledToHeight(toLayer.height());
        break;
    case aspectRatio:
        toDraw = importImg.scaled(toLayer.width(), toLayer.height());
        break;
    case toWidth:
        toDraw = importImg.scaledToWidth(toLayer.width());
        break;
    case toHeight:
        toDraw = importImg.scaledToHeight(toLayer.height());
        break;
    }
    QPainter qp;
    qp.begin(&toLayer);
    qp.drawImage(0, 0, toDraw);
    qp.end();
    importImg = toLayer;
    if (importType == image) {
        frame.push_back(new Layer(importImg, 255));
        updated = true;
        activeLayer = static_cast<unsigned char>(frame.size() - 1);
    }
}

void DataIOHandler::save(QString projectName) {
    string backupName;
    if (QFile::exists(projectName)) {
        backupName = projectName.toStdString();
        backupName = backupName.substr(0, backupName.find_last_of(".glass") - 5) + "__backup.glass";
        QFile::rename(projectName, backupName.c_str());
    }
    QFile file(projectName);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << static_cast<short>(dims.width());
    out << static_cast<short>(dims.height());
    out << static_cast<unsigned char>(frame.size());
    for (size_t i = 0; i < frame.size(); ++i) {
        // Write layer alpha and image
        unsigned char a = static_cast<unsigned char>(frame[i]->getAlpha());
        out << a;
        out << *(frame[i]->getCanvas());
        out << static_cast<unsigned char>(frame[i]->getVectors().size());
        vector <SplineVector> svs = frame[i]->getVectors();
        for (size_t j = 0; j < svs.size(); j++) {
            SplineVector sv = svs[j];
            // Write vector control point details
            unsigned char numPts = static_cast<unsigned char>(sv.getNumPts());
            out << numPts;
            vector <QPoint> controlPts = sv.getControls();
            for (short h = 0; h < numPts; ++h)
                out << controlPts[h];
            out << static_cast<unsigned char>(sv.getFilter().getFilterIndex());
            out << static_cast<unsigned char>(sv.getFilter().getStrength());
            out << sv.getColors().first;
            out << sv.getColors().second;
            out << static_cast<unsigned char>(sv.getWidth());
            out << static_cast<unsigned char>(sv.getTaper().first);
            out << static_cast<unsigned char>(sv.getTaper().second);
            out << static_cast<unsigned char>(Taper(sv.getTaperType()));
            out << static_cast<unsigned char>(VectorMode(sv.getMode()));
            out << static_cast<unsigned short>(sv.getBand());
            out << static_cast<unsigned short>(sv.getGap());
        }

        vector <Polygon> gons = frame[i]->getPolgons();
        out << static_cast<unsigned char>(gons.size());
        for (size_t j = 0; j < gons.size(); ++j) {
            vector <QPoint> pts = gons[j].getPts();
            out << static_cast<int>(pts.size());
            for (int k = 0; k < pts.size(); ++k)
                out << pts[k];
            out << static_cast<unsigned char>(gons[j].getFilter().getFilterIndex());
            out << static_cast<unsigned char>(gons[j].getFilter().getStrength());
            out << gons[j].getPolyColor();
            out << gons[j].getEdgeColor();
            out << static_cast<short>(gons[j].getEdgeSize());
            out << static_cast<unsigned char>(gons[j].getPolyMode());
            out << static_cast<unsigned char>(gons[j].getShowDivs());
        }

        vector <DrawText> texts = frame[i]->getTexts();
        out << static_cast<unsigned char>(texts.size());
        for (size_t j = 0; j < texts.size(); ++j) {
            out << texts[j].getFont();
            out << texts[j].getColor();
            out << texts[j].getText().textWidth();
            out << texts[i].getCorner();
            out << texts[i].getVals();
            out << texts[i].getText().text();
        }
    }
    if (QFile::exists(backupName.c_str()))
        QFile::remove(backupName.c_str());
    file.flush();
    file.close();
}

int DataIOHandler::load(QString projectName) {
    int retCode = 0;
    QFile file(projectName);
    QDataStream in(&file);
    if (!file.open(QIODevice::ReadOnly))
        return 2;
    vector <Layer *> frameBackup = frame;
    int backupLayer = activeLayer;
    clearFrame();
    activeLayer = 0;
    try {
        short size;
        unsigned char numLayers;
        in >> size;
        dims.setWidth(size);
        in >> size;
        dims.setHeight(size);
        in >> numLayers;
        if (numLayers <= maxLayer) {
            for (int i = 0; i < numLayers; ++i) {
                unsigned char alpha;
                in >> alpha;
                QImage qi;
                in >> qi;
                unsigned char ucharTemp, numVects;
                QRgb color;
                // Read size of vects
                in >> numVects;
                if (numVects > maxVects)  {
                    retCode = 1;
                    break;
                }
                list <SplineVector> svs;
                for (int j = 0; j < numVects; ++j) {
                    SplineVector sv;
                    // Read number of points
                    in >> ucharTemp;
                    if (ucharTemp > maxPoints)  {
                        retCode = 1;
                        break;
                    }
                    for (int h = 0; h < ucharTemp; h++) {
                        QPoint point;
                        in >> point;
                        sv.addPt(point, h);
                    }
                    // Read filter
                    in >> ucharTemp;
                    if (ucharTemp > graphics::numFilters)  {
                        retCode = 1;
                        break;
                    }
                    sv.setFilter(graphics::filterNames[ucharTemp]);
                    in >> ucharTemp;
                    sv.setFilterStrength(ucharTemp);
                    // Read colors
                    in >> color;
                    sv.setColor1(color);
                    in >> color;
                    sv.setColor2(color);
                    // Read width
                    in >> ucharTemp;
                    if (ucharTemp > maxWidth)  {
                        retCode = 1;
                        break;
                    }
                    sv.setWidth(ucharTemp);
                    // Read first taper
                    in >> ucharTemp;
                    if (ucharTemp > maxTaper)  {
                        retCode = 1;
                        break;
                    }
                    sv.setTaper1(ucharTemp);
                    // Read second taper
                    in >> ucharTemp;
                    if (ucharTemp > maxTaper)  {
                        retCode = 1;
                        break;
                    }
                    sv.setTaper2(ucharTemp);
                    // Read taper type
                    in >> ucharTemp;
                    if (ucharTemp > lastTaper)  {
                        retCode = 1;
                        break;
                    }
                    sv.setTaperType(static_cast<Taper>(ucharTemp));
                    //Read mode (fill type)
                    in >> ucharTemp;
                    if (ucharTemp > lastMode)  {
                        retCode = 1;
                        break;
                    }
                    sv.setMode(static_cast<VectorMode>(ucharTemp));
                    unsigned short uShortTemp;
                    in >> uShortTemp;
                    sv.setBand(uShortTemp);
                    in >> uShortTemp;
                    sv.setGap(uShortTemp);
                    svs.push_back(sv);
                }
                unsigned char numGons;
                in >> numGons;
                if (numGons > maxVects) {
                    retCode = 1;
                    break;
                }
                list <Polygon> gons;
                for (int j = 0; j < numGons; ++j) {
                    Polygon p;
                    int numPts;
                    in >> numPts;
                    for (int k = 0; k < numPts; ++k) {
                        QPoint qp;
                        in >> qp;
                        p.addPt(qp);
                    }
                    in >> ucharTemp;
                    p.setFilter(graphics::filterNames[ucharTemp]);
                    in >> ucharTemp;
                    p.setFilterStrength(ucharTemp);
                    QRgb color;
                    in >> color;
                    p.setPolyColor(color);
                    in >> color;
                    p.setEdgeColor(color);
                    short shortTemp;
                    in >> shortTemp;
                    if (shortTemp > maxEdgeSize) {
                        retCode = 1;
                        break;
                    }
                    p.setEdgeSize(shortTemp);
                    in >> ucharTemp;
                    p.setPolyMode(PolyMode(ucharTemp));
                    in >> ucharTemp;
                    p.setShowDivs(ucharTemp);
                    gons.push_back(p);
                }
                unsigned char numTexts;
                in >> numTexts;
                if (numTexts > maxVects) {
                    retCode = 1;
                    break;
                }
                list <DrawText> texts;
                for (int j = 0;  j < numTexts; ++j) {
                    DrawText dt;
                    QFont font;
                    in >> font;
                    QColor color;
                    in >> color;
                    dt.updateColor(color);
                    QStaticText text;
                    QString String;
                    qreal width;
                    in >> width;
                    text.setTextWidth(width);
                    QPoint corner;
                    in >> corner;
                    dt.updatePos(corner);
                    QVector <float> vals;
                    in >> vals;
                    dt.updateRotate(vals[0]);
                    dt.updateScale(vals[1], vals[2]);
                    in >> String;
                    text.setText(String);
                    dt.updateText(text);
                    texts.push_back(dt);
                }
                if (retCode == 0) {
                    qi = qi.convertToFormat(QImage::Format_ARGB32);
                    Layer *layer = new Layer(qi, alpha);
                    if (!svs.empty()) {
                        layer->setMode(Spline_Mode);
                        layer->pasteVectors(svs);
                        layer->deselect();
                        layer->setMode(Brush_Mode);
                    }
                    if (!gons.empty()) {
                        layer->setMode(Polygon_Mode);
                        layer->pastePolygons(gons);
                        layer->deselect();
                        layer->setMode(Brush_Mode);
                    }
                    if (!texts.empty()) {
                        layer->setMode(Text_Mode);
                        layer->pasteText(texts);
                        layer->deselect();
                        layer->setMode(Brush_Mode);
                    }
                    frame.push_back(layer);
                }
                else
                    break;
            }
        }
        if (retCode == 0) {
            activeLayer = static_cast<unsigned char>(frame.size() - 1);
            updated = true;
        }
    }
    catch (...) {
        retCode = 1;
    }
    file.close();
    if (retCode != 0) {
        for (size_t i = 0; i < frame.size(); ++i)
            delete frame[i];
        frame = frameBackup;
        activeLayer = backupLayer;
    }
    return retCode;
}

void DataIOHandler::clearFrame() {
    frame.clear();
}

vector <Layer *> DataIOHandler::backup() {
    vector <Layer *> compare = frame;
    return compare;
}

void DataIOHandler::setSym(QPoint qp, int div, int ofEvery, int skip) {
    for (Layer *l : frame)
        l->setSym(qp, div, ofEvery, skip);
}
