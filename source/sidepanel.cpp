#include "sidepanel.h"


SidePanel::SidePanel(QWidget *parent) : QScrollArea(parent) {
    curveOnOpen = QEasingCurve::Type::OutExpo;
    curveOnClose = QEasingCurve::Type::InExpo;
    panelSize = 380;
    offset = QPoint(0,0);
    state = Closed;
    animProgress = 0.0;
    const auto anim_func = [this](qreal t) -> void
    {
        const QRect parent_rect = parentRect();
        const QRectF geom_start = rectClosed(panelSize, parent_rect);
        const QRectF geom_end   = rectOpened(panelSize, parent_rect);
        const QRect new_geom = lerp(t, geom_start, geom_end).toRect();
        this->setGeometry( new_geom );
        updateHandlerRect(animProgress, new_geom);
    };
    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer, &QTimer::timeout, this, [this, anim_func]
    {
        const auto time_now = std::chrono::system_clock::now();
        if((time_now - timeStart) >= _duration) {
            timer->stop();

            // This setGeometry() for cases when duration=200ms, interval_time=100ms;
            switch (state) {
            case SidePanelState::Opening: { const auto geom = rectOpened(panelSize, parentRect()); this->setGeometry( geom ); animProgress = 1.0; updateHandlerRect(animProgress, geom); } break;
            case SidePanelState::Closing: { const auto geom = rectClosed(panelSize, parentRect()); this->setGeometry( geom ); animProgress = 0.0; updateHandlerRect(animProgress, geom); } break;
            default: break;
            }

            switch (state) {
            case SidePanelState::Opening: { this->show(); setState(SidePanelState::Opened); } break;
            case SidePanelState::Closing: { this->hide(); setState(SidePanelState::Closed); } break;
            default: break;
            }

            return;
        }

        const auto time_end = (timeStart + _duration);

        // [t_start .. t_now .. t_end] -> [0.0 .. t .. 1.0]
        qreal t = scale(
                    time_now.time_since_epoch().count(),
                    timeStart.time_since_epoch().count(),
                    time_end.time_since_epoch().count(),
                    0.0, 1.0);
        if(state == SidePanelState::Closing) // On closing - reverse it
            t = (1.0 - t);
        animProgress = t;
        if(state == SidePanelState::Opening)
            t = curveOnOpen.valueForProgress(t);
        else if(state == SidePanelState::Closing)
            t = curveOnClose.valueForProgress(t);
        anim_func(t);
    });

    handler = new QPushButton(parent);
    handler->setObjectName("SidePanelhandler");
    handler->resize(QSize(24, 60));//handler->size();
    handler->setFont(QFont(nullptr, 20));
    mainVert = new QVBoxLayout;
    QLabel* label = new QLabel("Layers", this);
    label->setFont(QFont(label->font().family(), 14));

    QPalette pal = label->palette();
    pal.setColor(QPalette::Window, QColor(215, 215, 215));
    pal.setColor(QPalette::WindowText, QColor(32, 32, 32));
    label->setAutoFillBackground(true);
    label->setPalette(pal);
    label->update();
    this->horizontalScrollBar()->setVisible(false);
    QHBoxLayout* init = new QHBoxLayout;
    init->addWidget(label);
    label->setAlignment(Qt::AlignCenter);
    mainVert->addLayout(init);
    headers = new QHBoxLayout();
    headers->addSpacerItem(new QSpacerItem(5, 30, QSizePolicy::Expanding, QSizePolicy::Minimum));
    QLabel *header1 = new QLabel(this);
    header1->setText("#");
    header1->setMaximumWidth(20);
    header1->setMinimumWidth(20);
    header1->setAlignment(Qt::AlignBottom);
    headers->addWidget(header1);
    headers->addSpacerItem(new QSpacerItem(5, 30, QSizePolicy::Expanding, QSizePolicy::Minimum));
    QLabel *header2 = new QLabel(this);
    header2->setText("Layer View");
    header2->setMaximumWidth(200);
    header2->setMinimumWidth(200);
    header2->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    headers->addWidget(header2);
    headers->addSpacerItem(new QSpacerItem(5, 30, QSizePolicy::Expanding, QSizePolicy::Minimum));
    QLabel *header3 = new QLabel(this);
    header3->setText("Visible");
    header3->setMaximumWidth(60);
    header3->setMinimumWidth(60);
    header3->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    headers->addWidget(header3);
    headers->addSpacerItem(new QSpacerItem(5, 30, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainVert->addLayout(headers);
    spacer = new QSpacerItem(320, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
    mainVert->addSpacerItem(spacer);


//    for(int i = 0; i < 20; ++i) {
//        QPushButton* btn = new QPushButton("Button " + QString::number(i), this);
//        btn->setText(QString().append(i % 2 ? 0x25EF : 0x25C9));
//        btn->setFont(QFont(nullptr, i % 2 ? 24 : 48));
//        QPalette pal = btn->palette();
//        pal.setColor(QPalette::Button, i % 2 ? QColor(192, 96, 96) : QColor(96, 192, 96));
//        pal.setColor(QPalette::ButtonText, i % 2 ? QColor(192, 96, 96) : QColor(96, 192, 96));
//        btn->setAutoFillBackground(true);
//        btn->setPalette(pal);
//        btn->update();

//        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//        btn->setMinimumHeight(90);
//        lay->addWidget(btn);
//    }

    proxy = new QWidget(this);
    proxy->setLayout(mainVert);
    setWidgetResizable(true);
    setWidget(proxy);
    QScroller::grabGesture(viewport(), QScroller::ScrollerGestureType::LeftMouseButtonGesture);

    auto* scroller = QScroller::scroller(viewport());
    QScrollerProperties props = scroller->scrollerProperties();
    props.setScrollMetric(QScrollerProperties::ScrollMetric::MousePressEventDelay, 0);
    scroller->setScrollerProperties(props);

    QTimer::singleShot(0, [this] {
        updateHandler(state, handler);
    });

    connect(handler, &QAbstractButton::pressed, this, [this] {
        emit(isMoving());
        localPos = this->mapFromGlobal(QCursor::pos());
        connect(&moveTimer, SIGNAL(timeout()), this, SLOT(moveSlot()));
        moveTimer.start(0);
        ogY = handler->geometry().center().y();
        emit(isMoving());
    });

    connect(handler, &QAbstractButton::clicked, this, [this]
    {
        emit(notMoving());
        moveTimer.stop();
        if (abs(ogY - handler->geometry().center().y()) >= 2)
            return;
        if(timer->isActive()) {
            switch (state) {
            case SidePanelState::Opening: { setState(SidePanelState::Closing); } break;
            case SidePanelState::Closing: { setState(SidePanelState::Opening); } break;
            default: break;
            }

        } else {
            switch (state) {
            case SidePanelState::Closed: { this->show(); setState(SidePanelState::Opening); } break;
            case SidePanelState::Opened: { this->show(); setState(SidePanelState::Closing); } break;
            default: break;
            }

            timeStart = std::chrono::system_clock::now();
            timer->start();
        }
    });

    connect(this, &SidePanel::stateChanged, handler, [this](SidePanelState state)
    {
        updateHandler(state, handler);
    });

    this->parentWidget()->installEventFilter(this);

    QTimer::singleShot(0, [this] {
        const auto geom = rectClosed(panelSize, parentRect()); this->setGeometry( geom ); animProgress = 0.0; updateHandlerRect(animProgress, geom);
    });
    //setGraphicsEffect(new QGraphicsOpacityEffect());
}

SidePanel::~SidePanel() {
    if(timer != nullptr) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }
    if(handler != nullptr) {
        delete handler;
        handler = nullptr;
    }
    if(parentWidget())
        removeEventFilter(this);
}

void SidePanel::openPanel() {
    timer->stop(); // Stop animation, if it's running
    this->show();
    const QRect new_geom = rectOpened(panelSize, parentRect());
    this->setGeometry( new_geom );
    animProgress = 1.0;
    updateHandlerRect(animProgress, new_geom);
    setState(SidePanelState::Opened);
}

void SidePanel::closePanel() {
    timer->stop(); // Stop animation, if it's running
    this->hide();
    const QRect new_geom = rectClosed(panelSize, parentRect());
    this->setGeometry( new_geom );
    animProgress = 0.0;
    updateHandlerRect(animProgress, new_geom);
    setState(SidePanelState::Closed);
}

void SidePanel::setOffsets(int topOff, int botOff, int leftOff) {
    top = topOff + 1;
    bottom = botOff + 1;
    left = leftOff;
    if (state > 1)
        closePanel();
    else
        openPanel();
}

void SidePanel::stopMoving() {
    moveTimer.stop();
}

void SidePanel::updateHandler(const SidePanelState state, QPushButton *btn) {
    QString str;
    switch (state) {
        case Opening:
            str.append(0x25B9);
            break;
        case Opened:
            str.append(0x25B8);
            break;
        case Closing:
            str.append(0x25C3);
            break;
        case Closed:
            str.append(0x25C2);
            break;
    }
    btn->setText(str);
}

void SidePanel::updateHandlerRect(const qreal progress, const QRect& geom) {
    QRect rect(0, 0, handler->size().width(), handler->size().height());
    rect.moveTopRight( {geom.left(), geom.center().y() - (handler->size().height()/2)} );
    handler->setGeometry( rect.translated(offset));
    QRect geo = handler->geometry();
    if (geo.top() < top)
        geo.translate(0, -geo.top() + top);
    if (geo.bottom() >= parentWidget()->height() - bottom)
        geo.translate(0, parentWidget()->height() - geo.bottom() - bottom);
    handler->setGeometry(geo);
}

void SidePanel::setState(const SidePanelState newstate) {
    if(state == newstate)
        return;
    state = newstate;
    emit stateChanged(state);
}

double SidePanel::scale(const double &valueIn, const double &baseMin, const double &baseMax, const double &limitMin, const double &limitMax) {
    return ((limitMax - limitMin) * (valueIn - baseMin) / (baseMax - baseMin)) + limitMin;
}

QPointF SidePanel::lerp(const qreal t, const QPointF &a, const QPointF &b) {
    return (1.0-t)*a + t*b;
}

QRectF SidePanel::lerp(const qreal t, const QRectF &a, const QRectF &b) {
    const QPointF topLeft   = lerp(t, a.topLeft(),     b.topLeft());
    const QPointF bottRight = lerp(t, a.bottomRight(), b.bottomRight());
    return QRectF(topLeft, bottRight);
}

QRect SidePanel::rectOpened(const int width, const QRect &parent_rect)
{
    QRect rect(0, 0, width, parent_rect.height());
    rect.moveTopRight(parent_rect.topRight());
    return rect;
}

QRect SidePanel::rectClosed(const int width, const QRect &parent_rect)
{
    QRect rect(0, 0, width, parent_rect.height());
    rect.moveTopLeft(parent_rect.topRight());
    return rect;
}

QRect SidePanel::parentRect() {
    QRect rect = parentWidget()->rect();
    rect.setTop(rect.top() + top);
    rect.setBottom(rect.bottom() - bottom);
    rect.setX(rect.x() - left);
    rect.setRight(rect.right() - left);
    return rect;
}

bool SidePanel::eventFilter(QObject *watched, QEvent *event) {
    if(event->type() == QEvent::Resize || event->type() == QEvent::Move) {
        switch (state) {
        case SidePanelState::Opened: { const auto geom = rectOpened(panelSize, parentRect()); this->setGeometry( geom ); this->updateHandlerRect(animProgress, geom); } break;
        case SidePanelState::Closed: { const auto geom = rectClosed(panelSize, parentRect()); this->setGeometry( geom ); this->updateHandlerRect(animProgress, geom); } break;
        }
    }
    return QScrollArea::eventFilter(watched, event);
}

void SidePanel::moveSlot() {
    QPoint qp = this->mapFromGlobal(QCursor::pos());
    if (qp != localPos && qp.y() >= 0 && qp.y() < parentWidget()->height()) {
        QPoint pt = qp - localPos;
        pt.setX(0);
        localPos = qp;
        offset += pt;
        QRect geo = handler->geometry().translated(pt);
        if (geo.top() < top)
            geo.translate(0, -geo.top() + top);
        if (geo.bottom() >= parentWidget()->height() - bottom)
            geo.translate(0, parentWidget()->height() - geo.bottom() - bottom);
        handler->setGeometry(geo);
    }
}

void SidePanel::resizeEvent(QResizeEvent *event) {
    QScrollArea::resizeEvent(event);
    updateHandlerRect(animProgress, this->geometry());
}

void SidePanel::add(Layer *layer) {
    mainVert->removeItem(spacer);
    tabs.push_back(new FrameTab(proxy));
    tabs.back()->refLayer(layer);
    mainVert->addLayout(tabs.back());
    for (int i = 0; i < tabs.size(); ++i)
        tabs[i]->updateNum(i);
    mainVert->addItem(spacer);
    setActive(tabs.size() - 1);
}

void SidePanel::remove(int delLayer) {
    mainVert->removeItem(tabs[delLayer]);
    delete tabs[delLayer];
    tabs.erase(tabs.begin() + delLayer);
    for (int i = 0; i < tabs.size(); ++i)
        tabs[i]->updateNum(i);
    if (delLayer == activeLayer)
        setActive(activeLayer);
    else if (delLayer == tabs.size() && tabs.size() > 0)
        setActive(--activeLayer);
}

void SidePanel::mousePressEvent(QMouseEvent *event) {
    int layer = (event->pos() - QPoint(0, 82 - this->verticalScrollBar()->value())).y() / 120;
    lastBtn = event->button();
    if (tabs.size() == 0 || layer < 0 || layer > tabs.size() || event->pos().y() < 82 || lastBtn != Qt::RightButton)
        return;
    selectedLayer = layer;
}

void SidePanel::mouseMoveEvent(QMouseEvent *event) {
    int layer = (event->pos() - QPoint(0, 82 - this->verticalScrollBar()->value())).y() / 120;
    cout << (event->button() == Qt::LeftButton) << endl;
    if (tabs.size() == 0 || layer < 0 || layer >= tabs.size() || event->pos().y() < 82 || lastBtn != Qt::RightButton)
        return;
    for (int i = 0; i < tabs.size(); ++i)
        mainVert->removeItem(tabs[i]);
    mainVert->removeItem(spacer);
    vector <FrameTab *> tCopy = tabs;
    int layer2 = selectedLayer;
    int adder = layer2 < layer ? 1 : -1;
    while (layer2 != layer) {
        std::swap(tCopy[layer2], tCopy[layer2 + adder]);
        layer2 += adder;
    }
    for (int i = 0; i < tabs.size(); ++i) {
        tCopy[i]->updateNum(i);
        mainVert->addLayout(tCopy[i]);
    }
    mainVert->addSpacerItem(spacer);
}

void SidePanel::mouseReleaseEvent(QMouseEvent *event) {
    int layer = (event->pos() - QPoint(0, 82 - this->verticalScrollBar()->value())).y() / 120;
    if (tabs.size() == 0 || layer < 0 || layer >= tabs.size() || event->pos().y() < 82 || event->button() != Qt::RightButton)
        return;
    cout << "here" << endl;
    toSwap = layer;
    for (int i = 0; i < tabs.size(); ++i)
        mainVert->removeItem(tabs[i]);
    mainVert->removeItem(spacer);
    int layer2 = selectedLayer;
    int adder = layer2 < layer ? 1 : -1;
    while (layer2 != layer) {
        std::swap(tabs[layer2], tabs[layer2 + adder]);
        layer2 += adder;
    }
    for (int i = 0; i < tabs.size(); ++i) {
        tabs[i]->updateNum(i);
        mainVert->addLayout(tabs[i]);
    }
    mainVert->addSpacerItem(spacer);
    emit(SidePanel::layerMoved());
}

void SidePanel::mouseDoubleClickEvent(QMouseEvent *event) {
    int layer = (event->pos() - QPoint(0, 82 - this->verticalScrollBar()->value())).y() / 120;
    if (tabs.size() == 0 || layer < 0 || layer >= tabs.size() || event->pos().y() < 82 || lastBtn != Qt::LeftButton)
        return;
    setActive(layer);
    emit(SidePanel::layerSet());
}

void SidePanel::setActive(int layer) {
    tabs[activeLayer]->setActive(false);
    tabs[layer]->setActive(true);
    activeLayer = layer;
}

int SidePanel::getActive() {
    return activeLayer;
}

pair <int, int> SidePanel::getToSwap() {
    return pair <int, int> (selectedLayer, toSwap);
}

