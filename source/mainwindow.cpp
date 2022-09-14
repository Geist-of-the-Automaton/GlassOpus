#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(string startPath, string projectFile, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(450, 450);
    hide();
    long long t = stdFuncs::getTime();
    QDir::setCurrent(startPath.c_str());
    setAcceptDrops(true);
    QLabel logo;
    QFile file(QDir::currentPath() + UI_Loc + Logo_FileName);
    bool logoFound = file.exists();
    QPoint center;
    QRect screenRect = QGuiApplication::screens().front()->availableGeometry();
    if (logoFound) {
        QImage qi(file.fileName());
        logo.setPixmap(QPixmap::fromImage(qi));
        center = screenRect.center();
        logo.setFixedSize(qi.size());
        logo.setWindowFlag(Qt::WindowType::FramelessWindowHint, true);
        logo.setWindowFlag(Qt::WindowType::WindowStaysOnTopHint, true);
        logo.move(center - logo.rect().center());
        logo.show();
        QCoreApplication::processEvents();
    }
    qme = new QErrorMessage(this);
    shiftFlag = false;
    ctrlFlag = false;
    altFlag = false;
    lastButton = NoButton;
    vs = new viewScroller(this);
    vs->setWidgetResizable(true);
    progress = new QProgressDialog("Updating Views", "", 0, 0, this, Qt::WindowType::FramelessWindowHint);
    progress->setCancelButton(nullptr);
    progress->close();
    ioh = new DataIOHandler(progress);
    sr = new screenRender(ioh, vs);
    LayerMenu = new SidePanel(this);
    layerMenuInteract = false;
    connect(LayerMenu, &SidePanel::isMoving, this, [this] {
        layerMenuInteract = true;
    });
    connect(LayerMenu, &SidePanel::notMoving, this, [this] {
        layerMenuInteract = false;
    });
    setCentralWidget(vs);
    setGeometry(screenRect.width() / 8, screenRect.height() / 8, 6 * screenRect.width() / 8, 6 * screenRect.height() / 8);
    setWindowTitle("Glass Opus");
    bool exists = createMenubar();
    if (exists) {
        QMenu* vFiltering = static_cast<QMenu *>(objFetch.at("Vector Filter"));
        QMenu* pFiltering = static_cast<QMenu *>(objFetch.at("Polygon Filter"));
        for (string name : vectorFilters) {
            QAction *vAction = vFiltering->addAction((name).c_str());
            connect(vAction, &QAction::triggered, this, [=]() { this->changeVectorFilter(vAction->text().toStdString()); });
            log(name, vAction);
            QAction *pAction = pFiltering->addAction((name).c_str());
            connect(pAction, &QAction::triggered, this, [=]() { this->changePolygonFilter(pAction->text().toStdString()); });
            log(name, pAction);
        }
        QMenu* bShape = static_cast<QMenu *>(objFetch.at("Brush Shape"));
        for (string name : brushShapes) {
            QAction *bAction = bShape->addAction((name).c_str());
            QFile qf(QDir::currentPath() + Icon_Loc + QString(name.c_str()) + ".png");
            if (qf.exists())
                bAction->setIcon(QIcon(qf.fileName()));
            connect(bAction, &QAction::triggered, this, [=]() { this->changeBrushShape(bAction->text().toStdString()); });
            log(name, bAction);
        }
        QMenu* bMethod = static_cast<QMenu *>(objFetch.at("Brush Method"));
        for (string name : appMethods) {
            QAction *bAction = bMethod->addAction((name).c_str());
            QFile qf(QDir::currentPath() + Icon_Loc + QString(name.c_str()) + ".png");
            if (qf.exists())
                bAction->setIcon(QIcon(qf.fileName()));
            connect(bAction, &QAction::triggered, this, [=]() { this->changeBrushMethod(bAction->text().toStdString()); });
            log(name, bAction);
        }
    }
    else
        downloadItem(UI_Loc, UI_FileName, DownloadThenRestart, "Menu Configuration Not Found", "Menu configuration not found locally/offline.\nFetch and download menu configuration online?");
    ui->menubar->setCornerWidget(dynamic_cast<QMenuBar *>(objFetch.at(UI_FileName.toStdString())), Qt::TopLeftCorner);
    resizeCheck = new resizeWindow(this, ioh);
    radialProfiler = new RadialProfiler(&bh, this);
    brushProlfiler = new brushShape(this);
    pp = new patternProfiler(this);
    symDialog = new SymDialog(this);
    onePress = false;
    vs->setWidget(sr);
    file.setFileName(QDir::currentPath() + UI_Loc + Icon_Loc + WinIco_FileName);
    if (file.exists())
        setWindowIcon(QIcon(file.fileName()));
    takeFlag = false;
    magicFlag = false;
    move(center - rect().center());
    if (projectFile == "") {
        QInputDialog resPrompt;
        QStringList items;
        items.push_back("360p");
        items.push_back("480p");
        items.push_back("720p");
        items.push_back("900p");
        items.push_back("1080p");
        items.push_back("1444p");
        items.push_back("2160p");
        resPrompt.setOptions(QInputDialog::UseListViewForComboBoxItems);
        resPrompt.setComboBoxItems(items);
        resPrompt.setTextValue(items.first());
        resPrompt.setWindowTitle("New Project Resolution");
        resPrompt.setWhatsThis("This will set the resolution of the layers and resulting export. Importing a saved project file after this dialog will update the resolution");
        t = stdFuncs::getTime(t);
        if (logoFound)
            std::this_thread::sleep_for(std::chrono::milliseconds(t > 2000 ? 0 : 2000 - t));
        show();
        logo.hide();
        //resPrompt.exec();
        resPrompt.setTextValue("1080p");
        int sizeY = stoi(resPrompt.textValue().toStdString());
        ioh->setDims(QSize(static_cast<int>((16.0 / 9.0) * static_cast<float>(sizeY)), sizeY));
    }
    sr->updateHoverMap(bh.getSize(), bh.getBrushMap());
    brushDetails = new QDockWidget(this);
    brushScroller = new viewScroller(brushDetails);
    brushPanel = new BrushPanel(brushScroller);
    brushPanel->setWork(&bh, pp, brushProlfiler, radialProfiler);
    connect(brushPanel, &BrushPanel::updateSizeShape, this, [this] {
        if (bh.getBrushShape() == custom)
            bh.setShape(brushShapes[bh.getBrushShape()], brushProlfiler->getShapeSize(bh.getSize()));
        sr->updateHoverMap(bh.getSize(), bh.getBrushMap());
    });
    connect(brushPanel, &BrushPanel::symExec, this, [this] {
       doSomething("Symmetry Tool");
    });
    setupDock(brushDetails, brushScroller, brushPanel);
    vectDetails = new QDockWidget(this);
    vectScroller = new viewScroller(vectDetails);
    vectPanel = new VectPanel(vectScroller);
    connect(vectPanel, &VectPanel::actionGiven, this, [this] {
        Layer *layer = ioh->getWorkingLayer();
        pair <vType, unsigned int> data = vectPanel->getData();
        if (layer->getActiveVectors().size() == 1 || layer->symActive())
            switch (data.first) {
            case vSym:
                doSomething("Symmetry Tool");
                break;
            case vMode:
                doSomething(data.second == 0 ? "Color Vector" : "Filter Vector");
                break;
            case vFilter:
                changeVectorFilter(vectorFilters[data.second]);
                break;
            case vStrength:
                layer->setVectorFilterStrength(data.second);
                break;
            case vColor1:
                layer->setVectorColor2(data.second);
                break;
            case vColor2:
                layer->setVectorColor1(data.second);
                break;
            case vTaper1:
                layer->setVectorTaper1(data.second);
                break;
            case vTaper2:
                layer->setVectorTaper2(data.second);
                break;
            case vGap:
                layer->setGap(data.second);
                break;
            case vBand:
                layer->setBand(data.second);
                break;
            case vWidth:
                layer->setWidth(data.second);
                break;
            case vStyle:
                doSomething(data.second == 0 ? "Single Taper" : "Double Taper");
                break;
            }
        refresh();
    });
    setupDock(vectDetails, vectScroller, vectPanel);
    polyDetails = new QDockWidget(this);
    polyScroller = new viewScroller(polyDetails);
    polyPanel = new PolyPanel(polyScroller);
    connect(polyPanel, &PolyPanel::actionGiven, this, [this] {
        Layer *layer = ioh->getWorkingLayer();
        pair <pType, unsigned int> data = polyPanel->getData();
        if (layer->getActiveGons().size() == 1)
            switch (data.first) {
            case pMode:
                doSomething(data.second == 0 ? "Color Polygon" : "Filter Polygon");
                break;
            case pFilter:
                changePolygonFilter(vectorFilters[data.second]);
                break;
            case pStrength:
                layer->setPolyFilterStrength(data.second);
                break;
            case pColor1:
                layer->setGonColor(pair <QColor, QColor> (data.second, layer->getGonColor().second));
                break;
            case pColor2:
                layer->setGonColor(pair <QColor, QColor> (layer->getGonColor().first, data.second));
                break;
            case pBand:
                layer->setEdgeSize(data.second);
                break;
            case pReduce:
                doSomething("Reduce Points");
                break;
            case pTriView:
                layer->setShowDivs(data.second == 0 ? false : true);
                break;
            }
        refresh();
    });
    setupDock(polyDetails, polyScroller, polyPanel);
    rasterDetails = new QDockWidget(this);
    rasterScroller = new viewScroller(polyDetails);
    rasterPanel = new RasterPanel(rasterScroller);
    connect(rasterPanel, &RasterPanel::actionGiven, this, [this] {
        Layer *layer = ioh->getWorkingLayer();
        pair <rType, unsigned int> data = rasterPanel->getData();
        if (data.first == rColor)
            bh.setFillColor(data.second);
        else {
            string arr[] = {"Take Color", "Transparency Fill", "Magic Wand Selection", "Magic Wand Settings", "Flip Selection Horizontal", "Flip Selection Vertical", "Brightness Adjustment", "Contrast Adjustment", "Gamma Adjustment", "Saturation Adjustment", "Hue Adjustment", "Target Tool", "Filter", "Convolve", "Dither", "Color Transfer"};
            doSomething(arr[data.first - 1]);
        }
    });
    setupDock(rasterDetails, rasterScroller, rasterPanel);
    textDetails = new QDockWidget(this);
    textScroller = new viewScroller(polyDetails);
    textPanel = new TextPanel(textScroller);
    connect(textPanel, &TextPanel::actionGiven, this, [this] {
        Layer *layer = ioh->getWorkingLayer();
        pair <textType, unsigned int> data = textPanel->getData();
        if (layer->getActiveTexts().size() == 1) {
            QFont font = layer->getFont();
            QStaticText qst = layer->getText();
            QFontDatabase qfd;
            switch (data.first) {
            case tColor:
                layer->setTextColor(data.second);
                break;
            case tText:
                qst.setText(textPanel->getText());
                layer->setText(qst);
                break;
            case tFont:
                font.setFamily(qfd.families()[data.second]);
                layer->setFont(font);
                break;
            case tSize:
                font.setPointSize(data.second);
                layer->setFont(font);
                break;
            case tTaw:
                qst.setTextWidth(data.second);
                layer->setText(qst);
                break;
            case tWord:
                font.setWordSpacing(static_cast<int>(data.second));
                layer->setFont(font);
                break;
            case tLetter:
                font.setLetterSpacing(font.letterSpacingType(), static_cast<int>(data.second));
                layer->setFont(font);
                break;
            case tBold:
                font.setBold(data.second == 0 ? false : true);
                layer->setFont(font);
                break;
            case tItal:
                font.setItalic(data.second == 0 ? false : true);
                layer->setFont(font);
                break;
            case tUndr:
                font.setUnderline(data.second == 0 ? false : true);
                layer->setFont(font);
                break;
            case tOver:
                font.setOverline(data.second == 0 ? false : true);
                layer->setFont(font);
                break;
            case tStrk:
                font.setStrikeOut(data.second == 0 ? false : true);
                layer->setFont(font);
                break;
            }
        }
        refresh();
    });
    setupDock(textDetails, textScroller, textPanel);
    objDetails = vectDetails;
    objScroller = vectScroller;
    addDockWidget(Qt::LeftDockWidgetArea, objDetails);
    setMode(Brush_Mode);
    sr->setCursor(Qt::ArrowCursor);
    sr->setHoverActive(true);
    mwd = new MagicWandDialog(this);
    if (projectFile != "") {
        show();
        ioh->load(QString(projectFile.c_str()));
        saveFileName = projectFile.c_str();
        string toFind = projectFile.find_last_of("/") >= projectFile.length() ? "\\" : "/";
        projectFile = projectFile.substr(projectFile.find_last_of(toFind) + 1);
        projectFile = projectFile.substr(0, projectFile.length() - 6);
        setWindowTitle(QString("Glass Opus - ") + projectFile.c_str());
        refresh();
        t = stdFuncs::getTime(t);
        if (logoFound)
            std::this_thread::sleep_for(std::chrono::milliseconds(t > 2000 ? 0 : 2000 - t));
        logo.hide();
    }
    ioh->addLayer();
    histograms = new QLabel();
    connect(&qte, SIGNAL(textChanged()), this, SLOT(textChanged()));
    showMaximized();
    LayerMenu->setOffsets(menubar->height() + toolbar->height(), ui->statusbar->height(), 0);
    connect(toolbar, &QToolBar::topLevelChanged, this, [this] {
        changeOffsets();
    });
    toolbar->setContextMenuPolicy (Qt::PreventContextMenu);
//    statBarTest = new QDockWidget(this);
//    statBarTest->setTitleBarWidget(new QWidget(statBarTest));
//    addDockWidget(Qt::BottomDockWidgetArea, statBarTest);
    connect(LayerMenu, &SidePanel::layerSet, this, [this] {
        ioh->getWorkingLayer()->deselect();
        ioh->setActiveLayer(LayerMenu->getActive(), mode);
        ioh->getWorkingLayer();
        LayerMenu->setActive(ioh->getActiveLayer());
    });
    connect(LayerMenu, &SidePanel::layerMoved, this, [this] {
        ioh->getWorkingLayer()->deselect();
        pair <int, int> toSwap = LayerMenu->getToSwap();
        ioh->swapLayers(toSwap.first, toSwap.second);
        int active = ioh->getActiveLayer();
        if (active == toSwap.first)
            ioh->setActiveLayer(toSwap.second, mode);
        ioh->getWorkingLayer();
    });
    QTimer::singleShot(100, [this] {
        if (objScroller->verticalScrollBar()->isVisible()) {
            objScroller->setMinimumWidth(300);
            objScroller->setMaximumWidth(300);
        }
        else {
            objScroller->setMinimumWidth(280);
            objScroller->setMaximumWidth(280);
        }
        changeOffsets();
        ioh->addLayer();
        changeOffsets();
        ioh->deleteLayer();
        changeOffsets();
        ioh->deleteLayer();
    });
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (layerMenuInteract || ioh->getWorkingLayer() == nullptr || takeFlag || magicFlag)
        return;
    QPoint qp = sr->getZoomCorrected(vs->getScrollCorrected(event->pos() - QPoint(brushDetails->width() + 5, toolbar->height())));
    statusBar()->showMessage(("(" + to_string(qp.x()) + " , " + to_string(qp.y()) + ")").c_str(), 1000);
    if (altFlag) {
        sr->setSamplePt(qp);
        refresh();
        return;
    }
    if (mode == Brush_Mode) {
        if (lastButton == LeftButton) {
            bh.applyBrush(ioh->getWorkingLayer()->getCanvas(), qp);
        }
        else if (lastButton == RightButton) {
            if (bh.getMethodIndex() == appMethod::sample)
                setSamplePt(qp);
            else
                bh.erase(ioh->getWorkingLayer()->getCanvas(), qp);
        }
        sr->allowRedraw(1);
    }
    else if (mode == Spline_Mode || mode == Raster_Mode || mode == Text_Mode) {
        if (ctrlFlag)
            return;
        Layer *layer = ioh->getWorkingLayer();
        if (lastButton == Qt::LeftButton)
            layer->moveLeft(qp, lastPos);
        else if (lastButton == RightButton && shiftFlag)
            layer->moveRight(qp, lastPos);
        lastPos = qp;
        refresh();
    }
    else if (mode == Polygon_Mode) {
        Layer *layer = ioh->getWorkingLayer();
        if (lastButton == Qt::LeftButton)
            layer->moveLeft(qp);
        else if (lastButton == RightButton)
            layer->moveRight(qp);
        refresh();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (layerMenuInteract || ioh->getWorkingLayer() == nullptr)
        return;
    if (ctrlFlag || onePress || takeFlag || magicFlag)
        return;
    onePress = true;
    if (event->button() >= 8) {
        if (mode == Brush_Mode)
            takeFlag = true;
        setShiftFlag(true);
        return;
    }
    lastButton = event->button();
    QPoint qp = lastPos = sr->getZoomCorrected(vs->getScrollCorrected(event->pos() - QPoint(brushDetails->width() + 5,toolbar->height())));
    if (altFlag) {
        onePress = false;
        sr->setSamplePt(qp);
        refresh();
        return;
    }
    if (mode == Raster_Mode && event->button() == RightButton && !shiftFlag && !ioh->getWorkingLayer()->isRotating())
        ioh->getWorkingLayer()->fillColor(qp, bh.getFillColor());
    else if (mode == Brush_Mode) {
        sr->setHoverActive(false);
        if (lastButton == RightButton) {
            if (bh.getMethodIndex() == appMethod::sample)
                setSamplePt(qp);
            else {
                bh.erase(ioh->getWorkingLayer()->getCanvas(), qp);
                bh.setInterpolationActive(true);
            }
        }
        else if (lastButton == LeftButton) {
            bh.setRelativePoint(qp);
            bh.applyBrush(ioh->getWorkingLayer()->getCanvas(), qp);
            bh.setInterpolationActive(true);
        }
    }
    else if (mode == Polygon_Mode || mode == Spline_Mode || mode == Raster_Mode || mode == Text_Mode) {
        Layer *layer = ioh->getWorkingLayer();
        if (lastButton == RightButton)
            lastButton = layer->pressRight(qp);
        else if (lastButton == LeftButton)
            layer->pressLeft(qp);
    }
    refresh();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (layerMenuInteract) {
        layerMenuInteract = false;
        LayerMenu->stopMoving();
    }
    if (ioh->getWorkingLayer() == nullptr)
        return;
    QPoint qp = sr->getZoomCorrected(vs->getScrollCorrected(event->pos() - QPoint(brushDetails->width() + 5,toolbar->height())));
    if (altFlag) {
        if (bh.getMethodIndex() == appMethod::sample) {
            sr->setSamplePt(qp);
            bh.setSamplePoint(qp);
        }
        else if (mode == Polygon_Mode) {
            sr->setSamplePt(qp);
            ioh->getWorkingLayer()->createEllipse(qp);
            sr->setSamplePt(QPoint(-1000, -1000));
            altFlag = false;
        }
        else {
            symDialog->exec();
            ioh->setSym(qp, symDialog->getDiv(), symDialog->getOfEvery(), symDialog->getSkip());
            bh.setSym(qp, symDialog->getDiv(), symDialog->getOfEvery(), symDialog->getSkip());
            if (symDialog->getDiv() == 1)
                sr->setSamplePt(QPoint(-1000, -1000));
            brushPanel->updateFromBH();
            altFlag = false;
        }
        refresh();
        return;
    }
    if (magicFlag) {
        ioh->getWorkingLayer()->magicSelect(qp, mwd->getVals());
        magicFlag = false;
        return;
    }
    if (takeFlag) {
        if (mode == Brush_Mode) {
            bh.setBrushColor(ioh->getWorkingLayer()->getCanvas()->pixelColor(qp.x(), qp.y()));
            brushPanel->updateFromBH();
            sr->allowRedraw(-1);
        }
        else if (mode == Raster_Mode) {
            bh.setFillColor(ioh->getWorkingLayer()->getCanvas()->pixelColor(qp.x(), qp.y()));
            rasterPanel->updatePanel(bh.getFillColor());
        }
        if (!shiftFlag)
            takeFlag = false;
        return;
    }
    if (mode == Brush_Mode) {
        sr->setHoverActive(true);
        sr->allowRedraw(-1);
        bh.setInterpolationActive(false);
        refresh();
    }
    else if (mode == Polygon_Mode || mode == Spline_Mode || (mode == Raster_Mode && event->button() != Qt::RightButton))
        ioh->getWorkingLayer()->release(qp, event->button());
    else if (event->button() >= 8) {
        takeFlag = false;
        magicFlag = false;
        setShiftFlag(false);
        if (onePress)
            ioh->getWorkingLayer()->cleanUp();
        return;
    }
    onePress = false;
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    if (layerMenuInteract || ioh->getWorkingLayer() == nullptr)
        return;
    QPoint qp = sr->getZoomCorrected(vs->getScrollCorrected(event->pos() - QPoint(brushDetails->width() + 5,toolbar->height())));
    MouseButton button = event->button();
    if (mode == Spline_Mode || mode == Polygon_Mode || mode == Text_Mode) {
        if (shiftFlag)
            return;
        Layer *layer = ioh->getWorkingLayer();
        if (button == LeftButton)
            layer->doubleClickLeft(qp, ctrlFlag);
        else if (button == RightButton && (ctrlFlag || mode == Text_Mode))
            layer->doubleClickRight(qp);
        if (mode == Spline_Mode) {
            if (layer->getActiveVectors().size() == 1 || layer->symActive())
                vectPanel->updatePanel(layer->getVectors()[layer->getActiveVectors()[0]]);
            else
                vectPanel->resetPanel();
        }
        else if (mode == Polygon_Mode) {
            cout << "here" << endl;
            if (layer->getActiveGons().size() == 1)
                polyPanel->updatePanel(layer->getPolgons()[layer->getActiveGons()[0]]);
            else
                polyPanel->resetPanel();
            cout << "there" << endl;
        }
        else if (mode == Text_Mode) {
            if (layer->getActiveTexts().size() == 1)
                textPanel->updatePanel(layer->getTexts()[layer->getActiveTexts()[0]]);
            else
                textPanel->resetPanel();
        }
        sr->showPts();
        refresh();
    }
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    scrollLock.lock();
    if (layerMenuInteract || ioh->getWorkingLayer() == nullptr) {
        scrollLock.unlock();
        return;
    }
    int dy = event->angleDelta().y(), dx = -event->angleDelta().x();
    dy = dy > 0 ? 1 : -1;

    if (dx != 0) {
        double zoom = sr->getZoom();
        double c = (dx < 0 ? 1.0 : -1.0) / pow(graphics::maxZoom - zoom, (graphics::maxZoom - zoom) / 3.0);
        if ((zoom + c > graphics::minZoom && dx > 0 ) || (zoom + c < graphics::maxZoom && dx < 0))
            sr->setZoom(zoom + c);
        changeOffsets();
    }
    else if (mode == Brush_Mode) {
        if (shiftFlag) {
            bh.setStrength(bh.getStength() + dy);
            statusBar()->showMessage(("Brush Strength: " + to_string(bh.getStength())).c_str(), 1000);
            brushPanel->updateFromBH();
        }
        else if (ctrlFlag) {
            bh.setDensity(bh.getDensity() + dy);
            statusBar()->showMessage(("Brush Density: " + to_string(bh.getDensity())).c_str(), 1000);
            brushPanel->updateFromBH();
        }
        else {
            radialProfiler->updateSize(bh.getSize() + dy);
            if (bh.getBrushShape() == custom)
                bh.setShape(brushShapes[bh.getBrushShape()], brushProlfiler->getShapeSize(bh.getSize()));
            sr->updateHoverMap(bh.getSize(), bh.getBrushMap());
            statusBar()->showMessage(("Brush Radius: " + to_string(bh.getSize())).c_str(), 1000);
            brushPanel->updateFromBH();
        }
    }
    else if (mode == Spline_Mode) {
        if (shiftFlag) {
            vector <unsigned char> activeVects = ioh->getWorkingLayer()->getActiveVectors();
            if (activeVects.size() != 1 && !ioh->getWorkingLayer()->symActive())
                return;
            ioh->getWorkingLayer()->setVectorTaper1(ioh->getWorkingLayer()->getVectorTapers().first + dy);
            statusBar()->showMessage(("Vector Taper 1: " + to_string(ioh->getWorkingLayer()->getVectorTapers().first)).c_str(), 1000);
            vectPanel->updatePanel(pair <vType, unsigned int> (vTaper1, ioh->getWorkingLayer()->getVectorTapers().first));
        }
        else if (ctrlFlag) {
            vector <unsigned char> activeVects = ioh->getWorkingLayer()->getActiveVectors();
            if (activeVects.size() != 1 && !ioh->getWorkingLayer()->symActive())
                return;
            ioh->getWorkingLayer()->setVectorTaper2(ioh->getWorkingLayer()->getVectorTapers().second + dy);
            statusBar()->showMessage(("Vector Taper 2: " + to_string(ioh->getWorkingLayer()->getVectorTapers().second)).c_str(), 1000);
            vectPanel->updatePanel(pair <vType, unsigned int> (vTaper2, ioh->getWorkingLayer()->getVectorTapers().second));
        }
        else {
            ioh->getWorkingLayer()->spinWheel(dy);
            if (ioh->getWorkingLayer()->getActiveVectors().size() == 1)
                statusBar()->showMessage(("Vector Width: " + to_string(ioh->getWorkingLayer()->getWidth())).c_str(), 1000);
            vectPanel->updatePanel(pair <vType, unsigned int> (vWidth, ioh->getWorkingLayer()->getWidth()));
        }
    }
    else if (mode == Polygon_Mode) {
        ioh->getWorkingLayer()->spinWheel(dy);
        Layer *layer = ioh->getWorkingLayer();
        if (layer->getActiveGons().size() == 1)
            polyPanel->updatePanel(pair <pType, unsigned int> (pBand, (layer->getPolgons()[layer->getActiveGons()[0]]).getEdgeSize()));
    }
    else if (mode == Raster_Mode || mode == Text_Mode)
        ioh->getWorkingLayer()->spinWheel(dy);
    scrollLock.unlock();
    refresh();
}

void MainWindow::setSamplePt(QPoint qp) {
    bh.setSamplePoint(qp);
    sr->setSamplePt(qp);
}

void MainWindow::setShiftFlag(bool b) {
    shiftFlag = b;
    ioh->getWorkingLayer()->setShiftFlag(b);
}

void MainWindow::changeOffsets() {
    bool maxFlag = isMaximized();
    QRect reset = geometry();
    int left = 0, top = menubar->height(), bottom = ui->statusbar->height();//statBarTest->height() - 5;
    if (vs->verticalScrollBar()->isVisible())
        left += vs->horizontalScrollBar()->height();
    if (vs->horizontalScrollBar()->isVisible())
        bottom += vs->horizontalScrollBar()->height();
    if (!toolbar->isFloating()) {
        Qt::ToolBarArea area = toolBarArea(toolbar);
        if (area == Qt::ToolBarArea::TopToolBarArea)
            top += toolbar->height();
        else if (area == Qt::ToolBarArea::BottomToolBarArea)
            bottom += toolbar->height();
    }
    LayerMenu->setOffsets(top, bottom, left);
    setGeometry(reset);
    if (maxFlag)
        this->showMaximized();
}

bool MainWindow::createMenubar() {
    QFile qf(QDir::currentPath() + UI_Loc + UI_FileName);
    bool exists = qf.exists();
    if (exists) {
        toolbar = new QToolBar(this);
        toolbar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
        toolbar->setFloatable(false);
        log("toolbar", toolbar);
        this->addToolBar(toolbar);
        menubar = new QMenuBar(this);
        menubar->setGeometry(QRect(0, 0, 800, 20));
        log(UI_FileName.toStdString(), menubar);
        fstream uiFile;
        uiFile.open((QDir::currentPath() + UI_Loc + UI_FileName).toStdString(), ios::in);
        int line = 0;
        if (uiFile.is_open()){
            string fromFile;
            while(getline(uiFile, fromFile)) {
                if (line == 0 && fromFile[0] == '#')
                    addItems(toolbar, fromFile.substr(1));
                else {
                    string item = "";
                    size_t i;
                    for (i = 0; fromFile[i] != ';'; ++i)
                        item += fromFile[i];
                    ++i;
                    fromFile = fromFile.substr(i, fromFile.length() - i);
                    QMenu *menu = menubar->addMenu(item.c_str());
                    log(item, menu);
                    addItems(menu, fromFile);
                }
                ++line;
            }
            uiFile.close();
        }
    }
    return exists;
}

void MainWindow::addItems(QToolBar *qtb, string menuItems) {
    string item = "";
    for (size_t i = 0; i < menuItems.length(); ++i) {
        if (menuItems[i] == '|') {
            qtb->addSeparator();
            ++i;
        }
        else if (menuItems[i] == '~') {
            QWidget* empty = new QWidget();
            empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            qtb->addWidget(empty);
            ++i;
        }
        else if (menuItems[i] == ',') {
            QAction *action = qtb->addAction(item.c_str());
            QFile qf(QDir::currentPath() + Icon_Loc + QString(item.c_str()) + ".png");
            if (qf.exists())
                action->setIcon(QIcon(qf.fileName()));
            connect(action, &QAction::triggered, this, [=]() { this->doSomething(item); });
            log(item + "_toolbar", action);
            item = "";
        }
        else
            item += menuItems[i];
    }
}

void MainWindow::addItems(QMenu *menu, string menuItems) {
    string item = "";
    for (size_t i = 0; i < menuItems.length(); ++i) {
        if (menuItems[i] == '|') {
            menu->addSeparator();
            ++i;
        }
        else if (menuItems[i] == ',') {
            addAction(menu, item);
            item = "";
        }
        else if (menuItems[i] == '>') {
            QMenu *subMenu = menu->addMenu(item.c_str());
            QFile qf(QDir::currentPath() + Icon_Loc + QString(item.c_str()) + ".png");
            if (qf.exists())
                subMenu->setIcon(QIcon(qf.fileName()));
            log(item, subMenu);
            if (menuItems[i + 1] == '(') {
                string subItems = "";
                int flag = 0;
                for (i += 2; true; ++i) {
                    subItems += menuItems[i];
                    if (menuItems[i] == '(')
                        ++flag;
                    else if (menuItems[i] == ')') {
                        if (flag == 0)
                            break;
                        --flag;
                    }
                }
                addItems(subMenu, subItems);
                ++i;
            }
            else
                ++i;
            item = "";
        }
        else
            item += menuItems[i];
    }
}

void MainWindow::addAction(QMenu *menu, string item) {
    QAction *action = menu->addAction(item.c_str());
    QFile qf(QDir::currentPath() + Icon_Loc + QString(item.c_str()) + ".png");
    if (qf.exists())
        action->setIcon(QIcon(qf.fileName()));
    connect(action, &QAction::triggered, this, [=]() { this->doSomething(item); });
    log(item, action);
}

void MainWindow::doSomething(string btnPress) {
    altFlag = false;
    if (btnPress == "Import") {
        bool maxFlag = isMaximized();
        QRect reset = geometry();
        string formats = "";
        for (string s : acceptedImportImageFormats)
            formats += " *." + s;
        formats = "Image Files (" + formats.substr(1) + ")";
        QString fileName = QFileDialog::getOpenFileName(this, tr("Import"), "/", tr(formats.c_str()));
        if (fileName == "")
            return;
        string fn = fileName.toStdString();
        size_t index = fn.find_last_of('.');
        string fileType = fn.substr(index + 1);
        if (std::find(acceptedImportImageFormats.begin(), acceptedImportImageFormats.end(), fileType) != acceptedImportImageFormats.end()) {
            bool flag = ioh->importImage(fileName);
            setGeometry(reset);
            if (maxFlag)
                this->showMaximized();
            if (flag)
                resizeCheck->showRelative();
            ioh->getWorkingLayer()->setSym(bh.getSymPt(), symDialog->getDiv(), symDialog->getOfEvery(), symDialog->getSkip());
            changeOffsets();
            setMode(mode);
            LayerMenu->add(ioh->getWorkingLayer());
        }
        refresh();
        setGeometry(reset);
        if (maxFlag)
            this->showMaximized();
        sr->setMode(mode);
        changeOffsets();
    }
    else if (btnPress == "Open") {
        QMessageBox::StandardButton prompt = QMessageBox::question(this, "Open Project File", "Opening a project file will replace the current work. Continue?", QMessageBox::Yes|QMessageBox::No);
        if (prompt == QMessageBox::Yes) {
            QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), "/", tr("Glass Opus project files (*.glass)"));
            if (fileName == "")
                return;
            int ret = 0;
            ioh->load(fileName);
            if (ret == 0) {
                saveFileName = fileName;
                string s = saveFileName.toStdString();
                s = s.substr(s.find_last_of("/") + 1);
                s = s.substr(0, s.length() - 6);
                setWindowTitle(QString("Glass Opus - ") + s.c_str());
                refresh();
            }
            else if (ret == 1)
                qme->showMessage("File load aborted. File may be corrupted.");
            else if (ret == 2)
                qme->showMessage("File could not be opened.");
        }
        if (ioh->getNumLayers() == 0)
            sr->setCursor(Qt::ArrowCursor);
        else
            sr->setMode(mode);
    }
    else if (btnPress == "Help") {
        bool found = QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath() + Doc_Loc + Doc_FileName));
        if (!found)
            downloadItem(Doc_Loc, Doc_FileName, DownLoadThenOpen, "Documentation Not Found", "Documentation PDF not found locally/offline.\nFetch and download documentation online?");
    }
    else if (btnPress == "About") {
        QMessageBox qmb(QMessageBox::Question, "Glass Opus", "Glass Opus is an open source rotoscoping software for students and artists. The software will provide a variety of features to allow the users to see their work from start to finish. Drawing with a variety brushes and vectors, image manipulation, and filtering are among the many features than one can employ to create their vision.\n\nThe focus of Glass Opus, and the team behind it, is to provide a free software that students and artists can use to further their work and portfolio. This is often a difficult endeavor for artists due to the restrictive cost of major softwares. Since Glass Opus is open source, users can tweak features or add their own to suit specific needs. It will also serve as a foundation for those who seek to improve their knowledge in image processing and manipulation, as well as basic graphics programming.", QMessageBox::Yes, this);
        qmb.exec();
    }
    else if (btnPress == "Exit") {
        QMessageBox qmb(QMessageBox::Question, "Glass Opus", "Close Glass Opus?", QMessageBox::Yes, this);
        if (ioh->getNumLayers() != 0)
            qmb.addButton("Save and Close", QMessageBox::AcceptRole);
        qmb.addButton("No", QMessageBox::RejectRole);
        int choice = qmb.exec();
        if (choice == QMessageBox::AcceptRole && ioh->getNumLayers() == 0)
            choice = QMessageBox::RejectRole;
        if (choice == QMessageBox::AcceptRole) {
            if (saveFileName.isEmpty())
                saveFileName = QFileDialog::getSaveFileName(this, tr("Save Project"), "/", tr("Glass Opus project files (*.glass)"));
            ioh->save(saveFileName);
        }
        if (choice != QMessageBox::RejectRole)
            QApplication::exit();
    }
    else if (btnPress == "Insert Layer") {
        sr->setMode(mode);
        ioh->addLayer();
        ioh->getWorkingLayer()->setSym(bh.getSymPt(), symDialog->getDiv(), symDialog->getOfEvery(), symDialog->getSkip());
        changeOffsets();
        setMode(mode);
        LayerMenu->add(ioh->getWorkingLayer());
    }
    if (ioh->getWorkingLayer() == nullptr)
        return;
    else if (btnPress == "Export") {    //TODO
        sr->stopFlashing();
        string formats = "";
        for (string s : acceptedExportImageFormats)
            formats += " *." + s;
        formats = "Image Files (" + formats.substr(1) + ")";
        QString fileName = QFileDialog::getSaveFileName(this, tr("Export"), "/", tr(formats.c_str()));
        if (fileName == "")
            return;
        string fn = fileName.toStdString();
        size_t index = fn.find_last_of('.');
        string fileType = fn.substr(index + 1);
        if (std::find(acceptedExportImageFormats.begin(), acceptedExportImageFormats.end(), fileType) != acceptedExportImageFormats.end())
            ioh->exportImage(fileName);
        if (mode != Brush_Mode)
            sr->resume();
    }
    else if (btnPress == "Save") {
        if (saveFileName.isEmpty())
            saveFileName = QFileDialog::getSaveFileName(this, tr("Save Project"), "/", tr("Glass Opus project files (*.glass)"));
        if (saveFileName != "")
            ioh->save(saveFileName);

    }
    else if (btnPress == "Save As") {
        saveFileName = QFileDialog::getSaveFileName(this, tr("Save Project As"), "/", tr("Glass Opus project files (*.glass)"));
        if (saveFileName != "") {
            ioh->save(saveFileName);
            string s = saveFileName.toStdString();
            s = s.substr(s.find_last_of("/") + 1);
            s = s.substr(0, s.length() - 6);
            setWindowTitle(QString("Glass Opus - ") + s.c_str());
        }
    }
    else if (btnPress == "On")
        sr->showFg(true);
    else if (btnPress == "Off")
        sr->showFg(false);
    else if (btnPress == "Brush Color") {
        QColor color = QColorDialog::getColor(bh.getBrushColor(), this);
        bh.setBrushColor(color);
        brushPanel->updateFromBH();
    }
    else if (btnPress == "Radial Profiler")
        radialProfiler->showRelative();
    else if (btnPress == "Brush Radius") {
        bool ok = false;
        int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter a brush radius", bh.getSize(), minRadius, maxRadius, 1, &ok);
        if (ok) {
            radialProfiler->updateSize(ret);
            if (bh.getBrushShape() == custom)
                bh.setShape(brushShapes[bh.getBrushShape()], brushProlfiler->getShapeSize(bh.getSize()));
            sr->updateHoverMap(bh.getSize(), bh.getBrushMap());
            brushPanel->updateFromBH();
        }
    }
    else if (btnPress == "Brush Strength") {
        bool ok = false;
        int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter a brush strength", bh.getStength(), minStrength, maxStrength, 1, &ok );
        if (ok) {
            bh.setStrength(ret);
            brushPanel->updateFromBH();
        }
    }
    else if (btnPress == "Brush Filter") {
        QInputDialog resPrompt;
        QStringList items;
        for (string s : filterNames)
            items.push_back(s.c_str());
        resPrompt.setOptions(QInputDialog::UseListViewForComboBoxItems);
        resPrompt.setComboBoxItems(items);
        resPrompt.setTextValue(items.first());
        resPrompt.setWindowTitle("Filters");
        int res = resPrompt.exec();
        if (res) {
            bh.setFilter(resPrompt.textValue().toStdString());
            brushPanel->updateFromBH();
        }
    }
    else if (btnPress == "Spray Density") {
        bool ok = false;
        int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter a spray density", bh.getDensity(), minDensity, maxDensity, 1, &ok );
        if (ok) {
            bh.setDensity(ret);
            brushPanel->updateFromBH();
        }
    }
    else if (btnPress == "Flip Selection Vertical")
        ioh->getWorkingLayer()->flipVert();
    else if (btnPress == "Flip Selection Horizontal")
        ioh->getWorkingLayer()->flipHori();
    else if (btnPress == "Pattern On") {
        bh.setPatternInUse(true);
        brushPanel->updateFromBH();
    }
    else if (btnPress == "Pattern Off") {
        bh.setPatternInUse(false);
        brushPanel->updateFromBH();
    }
    else if (btnPress == "Vector Width") {
        vector <unsigned char> activeVects = ioh->getWorkingLayer()->getActiveVectors();
        if (activeVects.size() != 1 && !ioh->getWorkingLayer()->symActive())
            return;
        bool ok = false;
        int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter a vector width", ioh->getWorkingLayer()->getWidth(), minWidth, maxWidth, 1, &ok );
        if (ok) {
            ioh->getWorkingLayer()->setWidth(ret);
            vectPanel->updatePanel(pair <vType, unsigned int> (vWidth, ret));
        }
    }
    else if (btnPress == "Vector Filter Strength") {
        int val = ioh->getWorkingLayer()->getVectorFilterStrength();
        if (val == -1)
            return;
        bool ok = false;
        int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter a vector filter strength", val, graphics::minColor, graphics::maxColor, 1, &ok );
        if (ok) {
            ioh->getWorkingLayer()->setVectorFilterStrength(ret);
            vectPanel->updatePanel(pair <vType, unsigned int> (vStrength, ret));
        }
    }
    else if (btnPress == "Taper 1") {
        vector <unsigned char> activeVects = ioh->getWorkingLayer()->getActiveVectors();
        if (activeVects.size() != 1 && !ioh->getWorkingLayer()->symActive())
            return;
        bool ok = false;
        int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter a first taper degree", ioh->getWorkingLayer()->getVectorTapers().first, minTaper, maxTaper, 1, &ok);
        if (ok) {
            ioh->getWorkingLayer()->setVectorTaper1(ret);
            vectPanel->updatePanel(pair <vType, unsigned int> (vTaper1, ret));
        }
    }
    else if (btnPress == "Taper 2") {
        vector <unsigned char> activeVects = ioh->getWorkingLayer()->getActiveVectors();
        if (activeVects.size() != 1 && !ioh->getWorkingLayer()->symActive())
            return;
        bool ok = false;
        int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter a second taper degree", ioh->getWorkingLayer()->getVectorTapers().second, minTaper, maxTaper, 1, &ok);
        if (ok) {
            ioh->getWorkingLayer()->setVectorTaper2(ret);
            vectPanel->updatePanel(pair <vType, unsigned int> (vTaper2, ret));
        }
    }
    else if (btnPress == "Vector Color 1") {
        vector <unsigned char> activeVects = ioh->getWorkingLayer()->getActiveVectors();
        if (activeVects.size() != 1 && !ioh->getWorkingLayer()->symActive())
            return;
        QColor color = QColorDialog::getColor(ioh->getWorkingLayer()->getVectorColors().second, this);
        ioh->getWorkingLayer()->setVectorColor2(color.rgba());
        vectPanel->updatePanel(pair <vType, unsigned int> (vColor1, color.rgba()));
    }
    else if (btnPress == "Vector Color 2") {
        vector <unsigned char> activeVects = ioh->getWorkingLayer()->getActiveVectors();
        if (activeVects.size() != 1 && !ioh->getWorkingLayer()->symActive())
            return;
        QColor color = QColorDialog::getColor(ioh->getWorkingLayer()->getVectorColors().first, this);
        ioh->getWorkingLayer()->setVectorColor1(color.rgba());
        vectPanel->updatePanel(pair <vType, unsigned int> (vColor2, color.rgba()));
    }
    else if (btnPress == "Single Taper") {
        ioh->getWorkingLayer()->setVectorTaperType(Single);
        vectPanel->updatePanel(pair <vType, unsigned int> (vStyle, 1));
    }
    else if (btnPress == "Double Taper") {
        ioh->getWorkingLayer()->setVectorTaperType(Double);
        vectPanel->updatePanel(pair <vType, unsigned int> (vStyle, 0));
    }
    else if (btnPress == "Band Size") {
        vector <unsigned char> activeVects = ioh->getWorkingLayer()->getActiveVectors();
        if (activeVects.size() != 1 && !ioh->getWorkingLayer()->symActive())
            return;
        bool ok = false;
        int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter a banding width", ioh->getWorkingLayer()->getBand(), minStyle, maxStyle, 1, &ok);
        if (ok) {
            ioh->getWorkingLayer()->setBand(ret);
            vectPanel->updatePanel(pair <vType, unsigned int> (vBand, ret));
        }
    }
    else if (btnPress == "Gap Size") {
        vector <unsigned char> activeVects = ioh->getWorkingLayer()->getActiveVectors();
        if (activeVects.size() != 1 && !ioh->getWorkingLayer()->symActive())
            return;
        bool ok = false;
        int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter a gap width", ioh->getWorkingLayer()->getGap(), 0, maxStyle, 1, &ok);
        if (ok) {
            ioh->getWorkingLayer()->setGap(ret);
            vectPanel->updatePanel(pair <vType, unsigned int> (vGap, ret));
        }
    }
    else if (btnPress == "Swap Colors") {
        ioh->getWorkingLayer()->swapColors();
    }
    else if (btnPress == "Swap Tapers") {
        ioh->getWorkingLayer()->swapTapers();
        pair <char, char> tapers = ioh->getWorkingLayer()->getVectorTapers();
        vectPanel->updatePanel(pair <vType, unsigned int> (vTaper1, tapers.first));
        vectPanel->updatePanel(pair <vType, unsigned int> (vTaper2, tapers.second));
    }
    else if (btnPress == "Color Vector") {
        ioh->getWorkingLayer()->setVectorMode(ColorFill);
        vectPanel->updatePanel(pair <vType, unsigned int> (vMode, 0));
    }
    else if (btnPress == "Filter Vector") {
        ioh->getWorkingLayer()->setVectorMode(Filtered);
        vectPanel->updatePanel(pair <vType, unsigned int> (vMode, 1));
    }
    else if (btnPress == "Symmetry Tool") {
        symDialog->exec();
        ioh->setSym(bh.getSymPt(), symDialog->getDiv(), symDialog->getOfEvery(), symDialog->getSkip());
        bh.setSym(bh.getSymPt(), symDialog->getDiv(), symDialog->getOfEvery(), symDialog->getSkip());
        if (symDialog->getDiv() != 1)
            sr->setSamplePt(bh.getSymPt());
    }
    else if (btnPress == "Color Polygon") {
        ioh->getWorkingLayer()->setPolyMode(ColorGon);
        polyPanel->updatePanel(pair <pType, unsigned int> (pMode, 0));
    }
    else if (btnPress == "Filter Polygon") {
        ioh->getWorkingLayer()->setPolyMode(FilterGon);
        polyPanel->updatePanel(pair <pType, unsigned int> (pMode, 1));
    }
    else if (btnPress == "Interior Color") {
        Layer *layer = ioh->getWorkingLayer();
        if (layer->getActiveGons().size() == 1) {
            pair<QColor, QColor> colors = layer->getGonColor();
            colors.first = QColorDialog::getColor(colors.first, this);
            layer->setGonColor(colors);
            polyPanel->updatePanel(pair <pType, unsigned int> (pColor1, ioh->getWorkingLayer()->getGonColor().first.rgba()));
        }
    }
    else if (btnPress == "Edge Color") {
        Layer *layer = ioh->getWorkingLayer();
        if (layer->getActiveGons().size() == 1) {
            pair<QColor, QColor> colors = layer->getGonColor();
            colors.second = QColorDialog::getColor(colors.second, this);
            layer->setGonColor(colors);
            polyPanel->updatePanel(pair <pType, unsigned int> (pColor2, ioh->getWorkingLayer()->getGonColor().second.rgba()));
        }
    }
    else if (btnPress == "Transparent Interior") {
        Layer *layer = ioh->getWorkingLayer();
        if (layer->getActiveGons().size() == 1) {
            pair<QColor, QColor> colors = layer->getGonColor();
            colors.first.setAlpha(0);
            layer->setGonColor(colors);
            polyPanel->updatePanel(pair <pType, unsigned int> (pColor1, ioh->getWorkingLayer()->getGonColor().first.rgba()));
        }
    }
    else if (btnPress == "Polygon Filter Strength") {
        Layer *layer = ioh->getWorkingLayer();
        if (layer->getActiveGons().size() == 1) {
            bool ok = false;
            int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter a filter strength", layer->getPolyFilterStrength(), 0, 255, 1, &ok);
            if (ok) {
                layer->setPolyFilterStrength(ret);
                polyPanel->updatePanel(pair <pType, unsigned int> (pStrength, ret));
            }
        }
    }
    else if (btnPress == "Edge Size") {
        Layer *layer = ioh->getWorkingLayer();
        if (layer->getActiveGons().size() == 1) {
            bool ok = false;
            int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter a edge size", layer->getEdgeSize(), 0, maxEdgeSize, 1, &ok);
            if (ok) {
                polyPanel->updatePanel(pair <pType, unsigned int> (pStrength, ret));
                layer->setEdgeSize(ret);
            }
        }
    }
    else if (btnPress == "Drag Draw On")
        ioh->getWorkingLayer()->setDragDraw(true);
    else if (btnPress == "Drag Draw Off")
        ioh->getWorkingLayer()->setDragDraw(false);
    else if (btnPress == "Reduce Points")
        ioh->getWorkingLayer()->reduceGonPts();
    else if (btnPress == "View Divisions") {
        ioh->getWorkingLayer()->setShowDivs(true);
        polyPanel->updatePanel(pair <pType, unsigned int> (pTriView, 1));
    }
    else if (btnPress == "Hide Divisions") {
        ioh->getWorkingLayer()->setShowDivs(false);
        polyPanel->updatePanel(pair <pType, unsigned int> (pTriView, 0));
    }
    else if (btnPress == "Polygons to Raster Selection") {
        ioh->getWorkingLayer()->polyToSelect();
        setMode(Raster_Mode);
        polyPanel->resetPanel();
    }
    else if (btnPress == "Edit Text") {
        if (ioh->getWorkingLayer()->getActiveTexts().size() == 1) {
            QString text = ioh->getWorkingLayer()->getText().text();
            qte.clear();
            qte.setText(text);
            qte.setWindowModality(Qt::ApplicationModal);
            qte.show();
            textPanel->updatePanel(text);
        }
    }
    else if (btnPress == "Font") {
        if (ioh->getWorkingLayer()->getActiveTexts().size() == 1) {
            QInputDialog fontPrompt;
            QFontDatabase qfd;
            QStringList items = qfd.families();
            Layer *l = ioh->getWorkingLayer();
            QFont font = l->getFont();
            items.indexOf(font.family());
            fontPrompt.setOptions(QInputDialog::UseListViewForComboBoxItems);
            fontPrompt.setComboBoxItems(items);
            fontPrompt.setTextValue(items.first());
            fontPrompt.setWindowTitle("Fonts");
            fontPrompt.setWhatsThis("This will set the resolution of the layers and resulting export. Importing a saved project file after this dialog will update the resolution");
            fontPrompt.exec();
            font.setFamily(fontPrompt.textValue());
            l->setFont(font);
            textPanel->updatePanel(pair <textType, unsigned int> (tFont, items.indexOf(fontPrompt.textValue())));
        }
    }
    else if (btnPress == "Text Size") {
        if (ioh->getWorkingLayer()->getActiveTexts().size() == 1) {
            Layer *l = ioh->getWorkingLayer();
            QFont font = l->getFont();
            bool ok = false;
            int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter a text size", font.pointSize(), 0, 1000, 1, &ok);
            if (ok)
                font.setPointSize(ret);
            l->setFont(font);
            textPanel->updatePanel(pair <textType, unsigned int> (tSize, ret));
        }
    }
    else if (btnPress == "Text Color") {
        if (ioh->getWorkingLayer()->getActiveTexts().size() == 1) {
            Layer *l = ioh->getWorkingLayer();
            QColor qc = QColorDialog::getColor(l->getTextColor(), this);
            l->setTextColor(qc);
            textPanel->updatePanel(pair <textType, unsigned int> (tColor, qc.rgba()));
        }
    }
    else if (btnPress == "Bold") {
        if (ioh->getWorkingLayer()->getActiveTexts().size() == 1) {
            Layer *l = ioh->getWorkingLayer();
            QFont font = l->getFont();
            font.setBold(!font.bold());
            l->setFont(font);
            textPanel->updatePanel(pair <textType, unsigned int> (tBold, font.bold()));
        }
    }
    else if (btnPress == "Italic") {
        if (ioh->getWorkingLayer()->getActiveTexts().size() == 1) {
            Layer *l = ioh->getWorkingLayer();
            QFont font = l->getFont();
            font.setItalic(!font.italic());
            l->setFont(font);
            textPanel->updatePanel(pair <textType, unsigned int> (tItal, font.italic()));
        }
    }
    else if (btnPress == "Underline") {
        if (ioh->getWorkingLayer()->getActiveTexts().size() == 1) {
            Layer *l = ioh->getWorkingLayer();
            QFont font = l->getFont();
            font.setUnderline(!font.underline());
            l->setFont(font);
            textPanel->updatePanel(pair <textType, unsigned int> (tUndr, font.underline()));
        }
    }
    else if (btnPress == "Overline") {
        if (ioh->getWorkingLayer()->getActiveTexts().size() == 1) {
            Layer *l = ioh->getWorkingLayer();
            QFont font = l->getFont();
            font.setOverline(!font.overline());
            l->setFont(font);
            textPanel->updatePanel(pair <textType, unsigned int> (tOver, font.overline()));
        }
    }
    else if (btnPress == "Strikeout") {
        if (ioh->getWorkingLayer()->getActiveTexts().size() == 1) {
            Layer *l = ioh->getWorkingLayer();
            QFont font = l->getFont();
            font.setStrikeOut(!font.strikeOut());
            l->setFont(font);
            textPanel->updatePanel(pair <textType, unsigned int> (tStrk, font.strikeOut()));
        }
    }
    else if (btnPress == "Text Area Size") {
        if (ioh->getWorkingLayer()->getActiveTexts().size() == 1) {
            Layer *l = ioh->getWorkingLayer();
            QStaticText qst = l->getText();
            bool ok = false;
            int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter text area width", qst.textWidth(), -1, 10000, 1, &ok);
            if (ok)
                qst.setTextWidth(ret);
            textPanel->updatePanel(pair <textType, unsigned int> (tTaw, ret));
            l->setText(qst);
        }
    }
    else if (btnPress == "Word Spacing") {
        if (ioh->getWorkingLayer()->getActiveTexts().size() == 1) {
            Layer *l = ioh->getWorkingLayer();
            QFont font = l->getFont();
            bool ok = false;
            double ret = QInputDialog::getDouble(this, "Glass Opus", "Please enter word spacing", font.wordSpacing(), -100, 100.0, 1.0, &ok);
            if (ok)
                font.setWordSpacing(ret);
            textPanel->updatePanel(pair <textType, unsigned int> (tWord, ret));
            l->setFont(font);
        }
    }
    else if (btnPress == "Letter Spacing") {
        if (ioh->getWorkingLayer()->getActiveTexts().size() == 1) {
            Layer *l = ioh->getWorkingLayer();
            QFont font = l->getFont();
            bool ok = false;
            double ret = QInputDialog::getDouble(this, "Glass Opus", "Please enter letter spacing", font.letterSpacing(), -100, 100.0, 1.0, &ok);
            if (ok)
                font.setLetterSpacing(QFont::AbsoluteSpacing, ret);
            textPanel->updatePanel(pair <textType, unsigned int> (tLetter, ret));
            l->setFont(font);
        }
    }
    else if (btnPress == "Brightness Adjustment") {
        Layer *l = ioh->getWorkingLayer();
        QImage *img = l->getRaster().isNull() ? l->getCanvas() : l->getRasterPtr();
        BrightnessAdj badj(this);
        badj.setWork(img);
        badj.exec();
    }
    else if (btnPress == "Contrast Adjustment") {
        Layer *l = ioh->getWorkingLayer();
        QImage *img = l->getRaster().isNull() ? l->getCanvas() : l->getRasterPtr();
        ContrastAdj cadj(this);
        cadj.setWork(img);
        cadj.exec();
    }
    else if (btnPress == "Gamma Adjustment") {
        Layer *l = ioh->getWorkingLayer();
        QImage *img = l->getRaster().isNull() ? l->getCanvas() : l->getRasterPtr();
        GammaAdj gadj(this);
        gadj.setWork(img);
        gadj.exec();
    }
    else if (btnPress == "Saturation Adjustment") {
        Layer *l = ioh->getWorkingLayer();
        QImage *img = l->getRaster().isNull() ? l->getCanvas() : l->getRasterPtr();
        SaturationAdj sadj(this);
        sadj.setWork(img);
        sadj.exec();
    }
    else if (btnPress == "Hue Shift") {
        Layer *l = ioh->getWorkingLayer();
        QImage *img = l->getRaster().isNull() ? l->getCanvas() : l->getRasterPtr();
        HueShift hadj(this);
        hadj.setWork(img);
        hadj.exec();
    }
    else if (btnPress == "Dither") {
        Layer *l = ioh->getWorkingLayer();
        QImage *img = l->getRaster().isNull() ? l->getCanvas() : l->getRasterPtr();
        DitherDialog dd(this);
        dd.setWork(img);
        dd.exec();
    }
    else if (btnPress == "Target Tool") {
        Layer *l = ioh->getWorkingLayer();
        QImage *img = l->getRaster().isNull() ? l->getCanvas() : l->getRasterPtr();
        TargetTool tt(this);
        tt.setWork(img);
        tt.exec();
    }
    else if (btnPress == "Color Transfer") {
        string formats = "";
        for (string s : acceptedImportImageFormats)
            formats += " *." + s;
        formats = "Image Files (" + formats.substr(1) + ")";
        QString fileName = QFileDialog::getOpenFileName(this, tr("Import"), "/", tr(formats.c_str()));
        if (fileName == "")
            return;
        TransferDialog tdia(this);
        Layer *l = ioh->getWorkingLayer();
        QImage *img = l->getRaster().isNull() ? l->getCanvas() : l->getRasterPtr();
        tdia.setWork(img, QImage(fileName));
        tdia.exec();
    }
    else if (btnPress == "Fill Color") {
        QColor color = QColorDialog::getColor(bh.getFillColor(), this);
        bh.setFillColor(color);
        rasterPanel->updatePanel(color);
    }
    else if (btnPress == "Take Color" && (mode == Brush_Mode || mode == Raster_Mode))
        takeFlag = true;
    else if (btnPress == "Magic Wand Selection")
        magicFlag = true;
    else if (btnPress == "Magic Wand Settings") {
        vec4 settings = mwd->getVals();
        bool ok = mwd->exec();
        if (!ok)
            mwd->setVals(settings);
    }
    else if (btnPress == "Invert Selection")
        ioh->getWorkingLayer()->invertSelection();
    else if (btnPress == "Transparency Fill")
        bh.setFillColor(QColor(255, 255, 255, 0));
    else if (btnPress == "Filter") {
        FilterDialog fd(this);
        Layer *l = ioh->getWorkingLayer();
        QImage *img = l->getRaster().isNull() ? l->getCanvas() : l->getRasterPtr();
        fd.setWork(img);
        fd.exec();
    }
    else if (btnPress == "Convolve") {
        Layer *l = ioh->getWorkingLayer();
        QImage *img = l->getRaster().isNull() ? l->getCanvas() : l->getRasterPtr();
        KernelDialog kd(this);
        kd.setWork(img);
        kd.exec();
    }
    else if (btnPress == "Layer Opacity (Alpha)") {
        bool ok = false;
        int ret = QInputDialog::getInt(this, "Glass Opus", "Please enter a layer alpha", ioh->getWorkingLayer()->getAlpha(), 1, graphics::maxColor, 1, &ok);
        if (ok)
            ioh->getWorkingLayer()->setAlpha(ret);
    }
    else if (btnPress == "Layer Filter") {
        QInputDialog resPrompt;
        QStringList items;
        for (string s : filterNames)
            items.push_back(s.c_str());
        resPrompt.setOptions(QInputDialog::UseListViewForComboBoxItems);
        resPrompt.setComboBoxItems(items);
        resPrompt.setTextValue(items.first());
        resPrompt.setWindowTitle("Filters");
        int res = resPrompt.exec();
        if (res)
            ioh->getWorkingLayer()->setFilter(resPrompt.textValue().toStdString());
    }
    else if (btnPress == "Brush Mode") {
        ioh->getWorkingLayer()->deselect();
        setMode(Brush_Mode);
    }
    else if (btnPress == "Vector Mode") {
        ioh->getWorkingLayer()->deselect();
        setMode(Spline_Mode);
        if (symDialog->getDiv() == 1)
            setSamplePt(QPoint(-1000, -1000));
        else
            bh.setSamplePoint(QPoint(-1000, -1000));
    }
    else if (btnPress == "Raster Mode") {
        ioh->getWorkingLayer()->deselect();
        setMode(Raster_Mode);
        setSamplePt(QPoint(-1000, -1000));
        ioh->getWorkingLayer()->deselect();
    }
    else if (btnPress == "Polygon Mode") {
        ioh->getWorkingLayer()->deselect();
        sr->setSamplePt(QPoint(-1000, -1000));
        setMode(Polygon_Mode);
    }
    else if (btnPress == "Text Mode") {
        ioh->getWorkingLayer()->deselect();
        setMode(Text_Mode);
    }
    else if (btnPress == "3D Mode") {
        ioh->getWorkingLayer()->deselect();
        setMode(Mode_3D);
    }
    else if (btnPress == "Copy") {
        if (mode == Spline_Mode)
            ioh->copyVectors();
        else if (mode == Raster_Mode)
            ioh->copyRaster();
        else if (mode == Polygon_Mode)
            ioh->copyPolygons();
        else if (mode == Text_Mode)
            ioh->copyText();
    }
    else if (btnPress == "Cut") {
        if (mode == Spline_Mode) {
            ioh->cutVectors();
            vectPanel->resetPanel();
        }
        else if (mode == Raster_Mode)
            ioh->cutRaster();
        else if (mode == Polygon_Mode) {
            ioh->cutPolygons();
            polyPanel->resetPanel();
        }
        else if (mode == Text_Mode) {
            ioh->cutText();
            textPanel->resetPanel();
        }
    }
    else if (btnPress == "Delete") {
        ioh->getWorkingLayer()->deleteSelected();
        if (mode == Polygon_Mode)
            polyPanel->resetPanel();
        else if (mode == Spline_Mode)
            vectPanel->resetPanel();
        else if (mode == Text_Mode)
            textPanel->resetPanel();
    }
    else if (btnPress == "Deselect") {
        ioh->getWorkingLayer()->deselect();
        if (mode == Polygon_Mode)
            polyPanel->resetPanel();
        else if (mode == Spline_Mode)
            vectPanel->resetPanel();
        else if (mode == Text_Mode)
            textPanel->resetPanel();
    }
    else if (btnPress == "Paste") {
        if (mode == Spline_Mode) {
            ioh->pasteVectors();
            if (ioh->getWorkingLayer()->getActiveVectors().size() == 1 || ioh->getWorkingLayer()->symActive())
                vectPanel->updatePanel(ioh->getWorkingLayer()->getVectors()[ioh->getWorkingLayer()->getActiveVectors()[0]]);
        }
        else if (mode == Raster_Mode)
            ioh->pasteRaster();
        else if (mode == Polygon_Mode) {
            ioh->pastePolygons();
            if (ioh->getWorkingLayer()->getActiveGons().size() == 1)
                polyPanel->updatePanel(ioh->getWorkingLayer()->getPolgons()[ioh->getWorkingLayer()->getActiveGons()[0]]);
        }
        else if (mode == Text_Mode) {
            ioh->pasteText();
            if (ioh->getWorkingLayer()->getActiveTexts().size() == 1)
                textPanel->updatePanel(ioh->getWorkingLayer()->getTexts()[ioh->getWorkingLayer()->getActiveTexts()[0]]);
        }
    }
    else if (btnPress == "Select All") {
        ioh->getWorkingLayer()->selectAll();
        if (mode == Spline_Mode && (ioh->getWorkingLayer()->getActiveVectors().size() == 1 || ioh->getWorkingLayer()->symActive()))
            vectPanel->updatePanel(ioh->getWorkingLayer()->getVectors()[ioh->getWorkingLayer()->getActiveVectors()[0]]);
        else if (mode == Polygon_Mode && ioh->getWorkingLayer()->getActiveGons().size() == 1)
            polyPanel->updatePanel(ioh->getWorkingLayer()->getPolgons()[ioh->getWorkingLayer()->getActiveGons()[0]]);
        else if (mode == Text_Mode && ioh->getWorkingLayer()->getActiveTexts().size() == 1)
            textPanel->updatePanel(ioh->getWorkingLayer()->getTexts()[ioh->getWorkingLayer()->getActiveTexts()[0]]);
    }
    else if (btnPress == "Set Active Layer") {
        bool ok = false;
        int ret = QInputDialog::getInt(this, "Glass Opus", "Select a layer to edit", ioh->getActiveLayer() + 1, 1, ioh->getNumLayers(), 1, &ok) - 1;
        if (ok && ret != ioh->getActiveLayer()) {
            ioh->getWorkingLayer()->deselect();
            ioh->setActiveLayer(ret, mode);
            ioh->getWorkingLayer();
            LayerMenu->setActive(ioh->getActiveLayer());
        }
    }
    else if (btnPress == "Move Backward") {
        LayerMenu->moveBack(ioh->getActiveLayer());
        ioh->moveBackward();
    }
    else if (btnPress == "Move Forward") {
        LayerMenu->moveFront(ioh->getActiveLayer());
        ioh->moveForward();
    }
    else if (btnPress == "Move To Back") {
        LayerMenu->moveToBack(ioh->getActiveLayer());
        ioh->moveToBack();
    }
    else if (btnPress == "Move To Front") {
        LayerMenu->moveToFront(ioh->getActiveLayer());
        ioh->moveToFront();
    }
    else if (btnPress == "Copy Layer")
        ioh->copyLayer();
    else if (btnPress == "Cut Layer") {
        LayerMenu->remove(ioh->getActiveLayer());
        ioh->copyLayer();
        ioh->deleteLayer();
        if (ioh->getNumLayers() == 0)
            sr->setCursor(Qt::ArrowCursor);
    }
    else if (btnPress == "Paste Layer") {
        ioh->pasteLayer();
        ioh->getWorkingLayer()->setSym(bh.getSymPt(), symDialog->getDiv(), symDialog->getOfEvery(), symDialog->getSkip());
        changeOffsets();
        setMode(mode);
        LayerMenu->add(ioh->getWorkingLayer());

    }
    else if (btnPress == "Delete Layer") {
        LayerMenu->remove(ioh->getActiveLayer());
        ioh->deleteLayer();
        if (ioh->getNumLayers() == 0) {
            sr->setCursor(Qt::ArrowCursor);
            changeOffsets();
        }
    }
    else if (btnPress == "Compile Layer")
        ioh->compileLayer();
    else if (btnPress == "Compile Layers") {
        LayerMenu->compileFrame();
        ioh->compileFrame();
        setMode(mode);
        LayerMenu->setActive(0);
    }
    else if (btnPress == "Clear Vectors")
        ioh->getWorkingLayer()->clearVectors();
    else if (btnPress == "Clear Polygons")
        ioh->getWorkingLayer()->clearPolygons();
    else if (btnPress == "Clear Texts")
        ioh->getWorkingLayer()->clearTexts();
    else if (btnPress == "Clear Canvas")
        ioh->getWorkingLayer()->getCanvas()->fill(0x00000000);
    else if (btnPress == "Clear All")
        ioh->getWorkingLayer()->wipe();
    else if (btnPress == "Layer Functions") {
        LayerFunc lf(this);
        lf.setWork(ioh->getNumLayers());
        bool ok = lf.exec();
        if (ok) {
            ioh->layerFunc(lf.getChoice());
            ioh->setActiveLayer(ioh->getNumLayers() - 1, mode);
            ioh->getWorkingLayer()->setSym(bh.getSymPt(), symDialog->getDiv(), symDialog->getOfEvery(), symDialog->getSkip());
            changeOffsets();
            ioh->getWorkingLayer()->render(1, progress);
            LayerMenu->add(ioh->getWorkingLayer());
        }
    }
    else if (btnPress == "Zoom 100%") {
        sr->setZoom(1.0);
        changeOffsets();
    }
    else if (btnPress == "Set Zoom") {
        bool ok = false;
        double ret = QInputDialog::getDouble(this, "Glass Opus", "Select a layer to edit", sr->getZoom(), graphics::minZoom, graphics::maxZoom, 2, &ok);
        if (ok)
            sr->setZoom(ret);
        changeOffsets();
    }
    else if (btnPress == "Zoom In") {
        sr->zoomIn();
        changeOffsets();
    }
    else if (btnPress == "Zoom Out") {
        sr->zoomOut();
        changeOffsets();
    }
    else if (btnPress == "Shape Profiler") {
        brushProlfiler->exec();
        bh.setShape(brushShapes[bh.getBrushShape()], brushProlfiler->getShapeSize(bh.getSize()));
    }
    else if (btnPress == "Pattern Profiler") {
        pp->exec();
        bh.setPattern(pp->getPattern());
    }
    else if (btnPress == "Histograms") {
        HistoDialog hd(this);
        Layer *l = ioh->getWorkingLayer();
        QImage *img = l->getRaster().isNull() ? l->getCanvas() : l->getRasterPtr();
        hd.setWork(img);
        hd.exec();
    }
    refresh();
}

void MainWindow::downloadItem(QString subfolder, QString fileName, downloadAction action, QString promptTitle, QString promptText) {
    dSubfolder = subfolder;
    dFileName = fileName;
    dAction = action;
    QDir qd(QDir::currentPath() + dSubfolder);
    if (!qd.exists())
        qd.mkdir(QDir::currentPath() + dSubfolder);
    QMessageBox qmb(QMessageBox::Question, promptTitle, promptText, QMessageBox::Yes, this);
    qmb.addButton("No", QMessageBox::NoRole);
    int choice = qmb.exec();
    if (choice == QMessageBox::Yes) {
        downloadWatcher.addPath(QDir::homePath() + "/Downloads/");
        downloadWatcher.connect(&downloadWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(downloadFinished()));
        downloadTimer.connect(&downloadTimer, SIGNAL(timeout()), this, SLOT(downloadTimeout()));
        downloadTimer.start(5000);
        QDesktopServices::openUrl(QUrl::fromUserInput(FetchLink + dSubfolder + dFileName));
        setEnabled(false);
    }
    else if (dAction == DownloadThenRestart)
        QApplication::exit();
}

void MainWindow::downloadFinished() {
    QFile qf(QDir::homePath() + "/Downloads/" + dFileName);
    if (qf.exists()) {
        downloadTimer.stop();
        bool copied = false;
        while (!copied)
            copied = qf.copy(QDir::currentPath() + dSubfolder + dFileName);
        QDir qd;
        qd.remove(qf.fileName());
        if (dAction == DownLoadThenOpen)
            QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath() + dSubfolder + dFileName));
        else if (dAction == DownloadThenRestart) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath() + "/Roto.exe"));
            QApplication::exit();
        }
        downloadWatcher.removePath(QDir::homePath() + "/Downloads/");
        setEnabled(true);
    }
}

void MainWindow::downloadTimeout() {
    downloadTimer.stop();
    downloadWatcher.removePath(QDir::homePath() + "/Downloads/");
    if (dAction == DownloadThenRestart) {
        qme->showMessage("Unable to download or connect to internet.\nPlease fix Glass Opus installation.");
        QTimer::singleShot(5000, this, SLOT(close()));
    }
    else {
        setEnabled(true);
        qme->showMessage("Unable to download or connect to internet.");
    }
}

void MainWindow::changeVectorFilter(string s) {
    if (ioh->getWorkingLayer() == nullptr || !(ioh->getWorkingLayer()->getActiveVectors().size() == 1 || ioh->getWorkingLayer()->symActive()))
        return;
    ioh->getWorkingLayer()->setVectorFilter(s);
    vectPanel->updatePanel(pair <vType, unsigned int> (vFilter, (ioh->getWorkingLayer()->getVectors()[ioh->getWorkingLayer()->getActiveVectors()[0]]).getFilter().getFilterIndex()));
    refresh();
}

void MainWindow::changePolygonFilter(string s) {
    if (ioh->getWorkingLayer() == nullptr || ioh->getWorkingLayer()->getActiveGons().size() == 0)
        return;
    ioh->getWorkingLayer()->setPolygonFilter(s);
    polyPanel->updatePanel(pair <pType, unsigned int> (pFilter, (ioh->getWorkingLayer()->getPolgons()[ioh->getWorkingLayer()->getActiveGons()[0]]).getFilter().getFilterIndex()));
    refresh();
}

void MainWindow::changeScreenFilter(string filterName) {
    if (ioh->getWorkingLayer() == nullptr)
        return;
    ioh->getWorkingLayer()->setFilter(filterName);
    refresh();
}

void MainWindow::changeBrushFilter(string filterName) {
    bh.setFilter(filterName);
    brushPanel->updateFromBH();
}

void MainWindow::changeBrushShape(string shape) {
    if (shape == "Custom") {
        brushProlfiler->exec();
        bh.setShape(shape, brushProlfiler->getShapeSize(bh.getSize()));
    }
    else
        bh.setShape(shape);
    brushPanel->updateFromBH();
    sr->updateHoverMap(bh.getSize(), bh.getBrushMap());
}

void MainWindow::changeBrushMethod(string method) {
    if (method == "Sample" && symDialog->getDiv() != 1) {
        ioh->setSym(bh.getSymPt(), 1, 1, 0);
        bh.setSym(bh.getSymPt(), 1, 1, 0);
    }
    bh.setAppMethod(method);
    brushPanel->updateFromBH();
}

void MainWindow::textChanged() {
    if (ioh->getNumLayers() > 0) {
        Layer *l = ioh->getWorkingLayer();
        if (l->getActiveTexts().size() == 1) {
            QStaticText text = l->getText();
            text.setText(qte.toPlainText());
            l->setText(text);
            refresh();
        }
    }
}

void MainWindow::log(string title, QObject *obj) {
    objFetch[title] = obj;
    toDel.push_front(obj);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (ioh->getWorkingLayer() == nullptr)
        return;
    Qt::Key key = Qt::Key(event->key());
    Layer *layer = ioh->getWorkingLayer();
    if (mode == Text_Mode && !ctrlFlag && !altFlag && layer->getActiveTexts().size() == 1) {
        Layer *layer = ioh->getWorkingLayer();
        bool ret = layer->updateText(key, shiftFlag);
        refresh();
        if (ret) {
            textPanel->updatePanel(layer->getText().text());
            return;
        }
    }
    switch (key) {
    case Key_Up:
        sr->zoomIn();
        break;
    case Key_Down:
        sr->zoomOut();
        break;
    case Key_Left:
        if (mode == Brush_Mode) {
            if (shiftFlag) {
                bh.strengthDown();
                statusBar()->showMessage(("Brush Strength: " + to_string(bh.getStength())).c_str(), 1000);
                brushPanel->updateFromBH();
            }
            else if (ctrlFlag) {
                bh.densityDown();
                statusBar()->showMessage(("Brush Density: " + to_string(bh.getDensity())).c_str(), 1000);
                brushPanel->updateFromBH();
            }
            else {
                radialProfiler->updateSize(bh.getSize() - 1);
                statusBar()->showMessage(("Brush Radius: " + to_string(bh.getSize())).c_str(), 1000);
                brushPanel->updateFromBH();
            }
        }
        else if (mode == Spline_Mode) {
            layer->widthDown();
            vectPanel->updatePanel(pair <vType, unsigned int> (vWidth, layer->getWidth()));
        }
        else if (mode == Polygon_Mode) {
            if (layer->getActiveGons().size() == 1) {
                layer->setEdgeSize(layer->getEdgeSize() - 1);
                statusBar()->showMessage(("Edge Size: " + to_string(layer->getEdgeSize())).c_str(), 1000);
                polyPanel->updatePanel(pair <pType, unsigned int> (pBand, layer->getEdgeSize()));
            }
        }
        break;
    case Key_Right:
        if (mode == Brush_Mode) {
            if (shiftFlag) {
                bh.strengthUp();
                statusBar()->showMessage(("Brush Strength: " + to_string(bh.getStength())).c_str(), 1000);
                brushPanel->updateFromBH();
            }
            else if (ctrlFlag) {
                bh.densityUp();
                statusBar()->showMessage(("Brush Density: " + to_string(bh.getDensity())).c_str(), 1000);
                brushPanel->updateFromBH();
            }
            else {
                radialProfiler->updateSize(bh.getSize() + 1);
                statusBar()->showMessage(("Brush Radius: " + to_string(bh.getSize())).c_str(), 1000);
                brushPanel->updateFromBH();
            }
        }
        else if (mode == Spline_Mode) {
            layer->widthUp();
            vectPanel->updatePanel(pair <vType, unsigned int> (vWidth, layer->getWidth()));
        }
        else if (mode == Polygon_Mode) {
            if (layer->getActiveGons().size() == 1) {
                layer->setEdgeSize(layer->getEdgeSize() + 1);
                statusBar()->showMessage(("Edge Size: " + to_string(layer->getEdgeSize())).c_str(), 1000);
                polyPanel->updatePanel(pair <pType, unsigned int> (pBand, layer->getEdgeSize()));
            }
        }
        break;
    case Key_Control:
        if (!shiftFlag)
            ctrlFlag = true;
        break;
    case Key_Shift:
        if (!ctrlFlag) {
            if (mode == Brush_Mode)
                takeFlag = true;
            setShiftFlag(true);
        }
        break;
    case Key_Alt:
        if (mode == Brush_Mode || mode == Spline_Mode || mode == Polygon_Mode)
            altFlag = true;
        break;
    case Key_Escape:
        doSomething("Deselect");
        /*
        layer->deselect();
        if (mode == Spline_Mode)
            vectPanel->resetPanel();
        else if (mode == Polygon_Mode)
            polyPanel->resetPanel();
            */
        break;
    case Key_Backspace:
    case Key_Delete:
        doSomething("Delete");
        /*
        layer->deleteSelected();
        if (mode == Spline_Mode)
            vectPanel->resetPanel();
        if (mode == Polygon_Mode)
            polyPanel->resetPanel();
            */
        break;
    case Key_X:
        if (ctrlFlag) {
            doSomething("Cut");
            /*
            if (mode == Spline_Mode) {
                ioh->cutVectors();
                vectPanel->resetPanel();
            }
            else if (mode == Raster_Mode)
                ioh->cutRaster();
            else if (mode == Polygon_Mode) {
                ioh->cutPolygons();
                polyPanel->resetPanel();
            }
            else if (mode == Text_Mode)
                ioh->cutText();
                */
        }
        break;
    case Key_C:
        if (ctrlFlag) {
            if (mode == Spline_Mode)
                ioh->copyVectors();
            else if (mode == Raster_Mode)
                ioh->copyRaster();
            else if (mode == Polygon_Mode)
                ioh->copyPolygons();
            else if (mode == Text_Mode)
                ioh->copyText();
        }
        break;
    case Key_V:
        if (ctrlFlag) {
            doSomething("Paste");
            /*
            if (mode == Spline_Mode) {
                ioh->pasteVectors();
                if (ioh->getWorkingLayer()->getActiveVectors().size() == 1 || ioh->getWorkingLayer()->symActive())
                    vectPanel->updatePanel(ioh->getWorkingLayer()->getVectors()[ioh->getWorkingLayer()->getActiveVectors()[0]]);
                else
                    vectPanel->resetPanel();
            }
            else if (mode == Raster_Mode)
                ioh->pasteRaster();
            else if (mode == Polygon_Mode) {
                ioh->pastePolygons();
                if (ioh->getWorkingLayer()->getActiveGons().size() == 1)
                    polyPanel->updatePanel(ioh->getWorkingLayer()->getPolgons()[ioh->getWorkingLayer()->getActiveGons()[0]]);
                else
                    polyPanel->resetPanel();
            }
            else if (mode == Text_Mode)
                ioh->pasteText();
                */
        }
        break;
    case Key_A:
        if (ctrlFlag)
            layer->selectAll();
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Key_Shift:
        if (mode == Brush_Mode)
            takeFlag = false;
        setShiftFlag(false);
        if (onePress)
            ioh->getWorkingLayer()->cleanUp();
        break;
    case Key_Control:
        ctrlFlag = false;
        break;
    case Key_Alt:
        altFlag = false;
        break;
    }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    if (objFetch.find(UI_FileName.toStdString()) != objFetch.end()) {
        QMenuBar *menu = dynamic_cast<QMenuBar *>(objFetch.at(UI_FileName.toStdString()));
        menu->resize(width(), menu->height());
    }
    if (isVisible()) {
        QTimer::singleShot(100, [this] {
            if (objScroller->verticalScrollBar()->isVisible()) {
                objScroller->setMinimumWidth(300);
                objScroller->setMaximumWidth(300);
            }
            else {
                objScroller->setMinimumWidth(280);
                objScroller->setMaximumWidth(280);
            }
            changeOffsets();
        });
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event) {
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        bool maxFlag = isMaximized();
        QRect reset = geometry();
        QList<QUrl> urlList = mimeData->urls();
        for (int i = 0; i < urlList.size(); ++i) {
            QString fileName = urlList.at(i).toLocalFile();
            string fn = fileName.toStdString();
            size_t index = fn.find_last_of('.');
            string fileType = fn.substr(index + 1);
            if (std::find(acceptedImportImageFormats.begin(), acceptedImportImageFormats.end(), fileType) != acceptedImportImageFormats.end()) {
                bool flag = ioh->importImage(fileName);
                if (flag) {
                    setGeometry(reset);
                    if (maxFlag)
                        this->showMaximized();
                    resizeCheck->showRelative();
                    while (resizeCheck->isVisible())
                        QCoreApplication::processEvents();
                }
                ioh->getWorkingLayer()->setSym(bh.getSymPt(), symDialog->getDiv(), symDialog->getOfEvery(), symDialog->getSkip());
                changeOffsets();
                setMode(mode);
                LayerMenu->add(ioh->getWorkingLayer());
            }
        }
        refresh();
        setGeometry(reset);
        if (maxFlag)
            this->showMaximized();
    }
    sr->setMode(mode);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QMessageBox qmb(QMessageBox::Question, "Glass Opus", "Close Glass Opus?", QMessageBox::Yes, this);
    if (ioh->getNumLayers() != 0)
        qmb.addButton("Save and Close", QMessageBox::AcceptRole);
    qmb.addButton("No", QMessageBox::RejectRole);
    int choice = qmb.exec();
    if (choice == QMessageBox::AcceptRole && ioh->getNumLayers() == 0)
        choice = QMessageBox::RejectRole;
    if (choice == QMessageBox::AcceptRole) {
        if (saveFileName.isEmpty())
            saveFileName = QFileDialog::getSaveFileName(this, tr("Save Project"), "/", tr("Glass Opus project files (*.glass)"));
        ioh->save(saveFileName);
    }
    if (choice == QMessageBox::RejectRole)
        event->ignore();
    else
        QApplication::exit();
}

void MainWindow::setMode(EditMode emode) {
    mode = emode;
    if (ioh->getWorkingLayer() != nullptr)
        ioh->getWorkingLayer()->setMode(emode);
    if (mode == Spline_Mode)
        vectPanel->resetPanel();
//    else if (mode == Polygon_Mode)
//        polyPanel->resetPanel();
    sr->setMode(emode);
    if (isVisible()) {
        removeDockWidget(objDetails);
        switch (mode) {
        case Brush_Mode:
            objDetails = brushDetails;
            objScroller = brushScroller;
            break;
        case Spline_Mode:
            objDetails = vectDetails;
            objScroller = vectScroller;
            break;
        case Polygon_Mode:
            objDetails = polyDetails;
            objScroller = polyScroller;
            break;
        case Raster_Mode:
            objDetails = rasterDetails;
            objScroller = rasterScroller;
            break;
        case Text_Mode:
            objDetails = textDetails;
            objScroller = textScroller;
            break;
        }
        addDockWidget(Qt::LeftDockWidgetArea, objDetails);
        objDetails->setVisible(true);
        QTimer::singleShot(100, [this] {
            if (objScroller->verticalScrollBar()->isVisible()) {
                objScroller->setMinimumWidth(300);
                objScroller->setMaximumWidth(300);
            }
            else {
                objScroller->setMinimumWidth(280);
                objScroller->setMaximumWidth(280);
            }
        });
    }
}

void MainWindow::refresh() {
    sr->repaint();
}

void MainWindow::setupDock(QDockWidget *qdw, viewScroller *vsw, QWidget *qw) {
    qdw->setAllowedAreas(Qt::LeftDockWidgetArea);
    qdw->setCursor(Qt::ArrowCursor);
    qdw->setFeatures(qdw->features() & ~QDockWidget::DockWidgetClosable);
    qdw->setContextMenuPolicy (Qt::PreventContextMenu);

    vsw->setLayoutDirection(Qt::RightToLeft);
    vsw->setMaximumWidth(280);
    vsw->setMinimumWidth(280);

    qw->setLayoutDirection(Qt::LeftToRight);

    qdw->setWidget(vsw);
    vsw->setWidget(qw);
}

MainWindow::~MainWindow() {
    delete histograms;
    delete qme;
    resizeCheck->doClose();
    delete resizeCheck;
    hide();
    delete ioh;
    delete sr;
    delete vs;
    delete ui;
    objFetch.clear();
    while (!toDel.empty()) {
        QObject *obj = toDel.front();
        delete obj;
        toDel.pop_front();
    }
}
