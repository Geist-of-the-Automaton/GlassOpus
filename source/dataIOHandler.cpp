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
    QSize size = frame[0]->render().size();
    QImage qi = QImage(size, QImage::Format_ARGB32);
    progress->setLabelText("Compiling Frame");
    renderFrame(progress, &qi, frame);
    *frame[0] = Layer(qi, 255);
    //either just set canvas or use old active layer's editing mode through method
    setActiveLayer(0, Brush_Mode);
    while (frame.size() > 1) {
        delete frame.back();
        frame.pop_back();
    }
    updated = true;
}

void DataIOHandler::compileLayer() {
    Layer *layer = getWorkingLayer();
    layer->deselect();
    progress->setLabelText("Compiling Layer");
    *(layer->getCanvas()) = layer->render(1, progress);
    layer->clearAll();
}

void DataIOHandler::renderFrame(QProgressDialog *fqpd, QImage *ret, vector <Layer *> layers) {
    progressMarker = 0;
    fqpd->setValue(0);
    fqpd->setMaximum(static_cast<int>(layers.size()));
    fqpd->show();
    QCoreApplication::processEvents();
    ret->fill(0x00000000);
    QPainter qp;
    qp.begin(ret);
    for (Layer *layer : layers) {
        if (layer->isVisible()) {
            if (layer->hasFilterItems()) {
                qp.drawImage(0, 0, layer->getRenderCanvas());
                qp.end();
                renderLayer(nullptr, ret, layer);
                qp.begin(ret);
            }
            else
                qp.drawImage(0, 0, layer->render());
        }
        fqpd->setValue(++progressMarker);
    }
    qp.end();
    fqpd->close();
    fqpd->hide();
}

void DataIOHandler::setActiveLayer(int i, EditMode mode) {
    if (i > getNumLayers())
        return;
    frame[activeLayer]->render(1, progress);
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
    connect(frame.back(), &Layer::visUpdated, this, [this] {
         updated = true;
         emit(hasUpdate());
    });
    if (frame.size() != 1)
        frame[activeLayer]->render(1, progress);
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
    connect(frame.back(), &Layer::visUpdated, this, [this] {
         updated = true;
         emit(DataIOHandler::hasUpdate());
    });
    if (frame.size() != 1)
        frame[activeLayer]->render(1, progress);
    updated = true;
    activeLayer = static_cast<unsigned char>(frame.size() - 1);
    frame[activeLayer]->render(1, progress);
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

void DataIOHandler::swapLayers(int a, int b) {
    int c = a;
    int adder = c < b ? 1 : -1;
    while (c != b) {
        swap(frame[c], frame[c + adder]);
        c += adder;
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
    QImage qi = QImage(frame[0]->render().size(), QImage::Format_ARGB32);
    qi.fill(0x00000000);
    progress->setValue(0);
    QCoreApplication::processEvents();
    QPainter p;
    p.begin(&qi);
    for (size_t i = 0; i < activeLayer; ++i) {
        if (frame[i]->isVisible()) {
            if (frame[i]->hasFilterItems()) {
                p.drawImage(0, 0, frame[i]->getRenderCanvas());
                p.end();
                renderLayer(nullptr, &qi, frame[i]);
                p.begin(&qi);
            }
            else
                p.drawImage(0, 0, frame[i]->render());
        }
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
    QImage qi = QImage(frame[activeLayer + 1]->render().size(), QImage::Format_ARGB32);
    qi.fill(0x00000000);
    progress->setValue(progress->value() + 2);
    QCoreApplication::processEvents();
    QPainter p;
    p.begin(&qi);
    for (size_t i = activeLayer + 1; i < frame.size(); ++i) {
        if (frame[i]->isVisible()) {
            if (frame[i]->hasFilterItems()) {
                p.drawImage(0, 0, frame[i]->getRenderCanvas());
                p.end();
                renderLayer(nullptr, &qi, frame[i]);
                p.begin(&qi);
            }
            else
                p.drawImage(0, 0, frame[i]->render());
        }
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
    if (frame.size() != 0)
        frame[activeLayer]->render(1, progress);
    frame.push_back(new Layer(importImg, 255));
    connect(frame.back(), &Layer::visUpdated, this, [this] {
         updated = true;
         emit(DataIOHandler::hasUpdate());
    });
    updated = true;
    activeLayer = static_cast<unsigned char>(frame.size() - 1);
    frame[activeLayer]->render(1, progress);
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

void DataIOHandler::layerFunc(vector<int> choices) {
    QImage qi, a, b;
    if (choices[0] == activeLayer) {
        frame[choices[0]]->deselect();
        progress->setLabelText("Rendering Layer A");
        frame[choices[0]]->render(1, progress);
    }
    if (choices[1] == activeLayer) {
        frame[choices[1]]->deselect();
        progress->setLabelText("Rendering Layer B");
        frame[choices[1]]->render(1, progress);
    }
    a = frame[choices[0]]->render();
    b = frame[choices[1]]->render();
    if (choices[2] >= 7)
        qi = graphics::ImgSupport::bitLayers(a, b, graphics::bType(choices[2] - 7));
    else if (choices[2] == 0)
        qi = graphics::ImgSupport::addLayers(a, b, graphics::eType(choices[3]));
    else if (choices[2] == 1)
        qi = graphics::ImgSupport::subLayers(a, b, graphics::eType(choices[3]));
    else if (choices[2] == 2)
        qi = graphics::ImgSupport::diffLayers(a, b, graphics::eType(choices[3]));
    else if (choices[2] == 3)
        qi = graphics::ImgSupport::maxLayers(a, b, graphics::eType(choices[3]));
    else if (choices[2] == 4)
        qi = graphics::ImgSupport::minLayers(a, b, graphics::eType(choices[3]));
    else if (choices[2] == 5)
        qi = graphics::ImgSupport::avgLayers(a, b, graphics::eType(choices[3]));
    else if (choices[2] == 6)
        qi = graphics::ImgSupport::remLayers(a, b, graphics::eType(choices[3]));
    frame.push_back(new Layer(qi, 255));
}
