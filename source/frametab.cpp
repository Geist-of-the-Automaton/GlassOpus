#include "frametab.h"


FrameTab::FrameTab(QWidget *parent) : QHBoxLayout(parent) {
    numLb = new QLabel(parent);
    numLb->setText("00");
    numLb->setAlignment(Qt::AlignCenter);
    imgLb = new QLabel(parent);
    visPb = new QPushButton(parent);
    imgLb->setMinimumSize(200, 113);
    imgLb->setMaximumSize(200, 113);
    imgLb->setAlignment(Qt::AlignCenter);
    imgLb->setContentsMargins(0, 0, 0, 0);
    numLb->setMinimumSize(20, 113);
    numLb->setMaximumSize(20, 113);
    visPb->setMinimumSize(60, 113);
    visPb->setMaximumSize(60, 113);
    //imgLb->installEventFilter(this);
    connect(visPb, &QAbstractButton::pressed, this, [this] {
        bool vis = !ref->isVisible();
        visPb->setText(QString().append(vis ? 0x25C9 : 0x25EF));
        visPb->setFont(QFont(nullptr, vis ? 48 : 24));
        QPalette pal = visPb->palette();
        pal.setColor(QPalette::Button, vis ? QColor(96, 192, 96) : QColor(192, 96, 96));
        pal.setColor(QPalette::ButtonText, vis ? QColor(96, 192, 96) : QColor(192, 96, 96));
        visPb->setAutoFillBackground(true);
        visPb->setPalette(pal);
        visPb->update();
        ref->setVisibility(vis);
    });
    addSpacerItem(new QSpacerItem(5, 113, QSizePolicy::Expanding, QSizePolicy::Minimum));
    addWidget(numLb);
    addSpacerItem(new QSpacerItem(5, 113, QSizePolicy::Expanding, QSizePolicy::Minimum));
    addWidget(imgLb);
    addSpacerItem(new QSpacerItem(5, 113, QSizePolicy::Expanding, QSizePolicy::Minimum));
    addWidget(visPb);
    addSpacerItem(new QSpacerItem(5, 113, QSizePolicy::Expanding, QSizePolicy::Minimum));
}

FrameTab::~FrameTab() {
    delete imgLb;
    delete visPb;
    delete numLb;
}

void FrameTab::refLayer(Layer *layer) {
    ref = layer;
    updateView();
    connect(ref, &Layer::viewUpdated, this, [this] {
         updateView();
    });
    bool vis = ref->isVisible();
    visPb->setText(QString().append(vis ? 0x25C9 : 0x25EF));
    visPb->setFont(QFont(nullptr, vis ? 48 : 24));
    QPalette pal = visPb->palette();
    pal.setColor(QPalette::Button, vis ? QColor(96, 192, 96) : QColor(192, 96, 96));
    pal.setColor(QPalette::ButtonText, vis ? QColor(96, 192, 96) : QColor(192, 96, 96));
    visPb->setAutoFillBackground(true);
    visPb->setPalette(pal);
    visPb->update();
}

void FrameTab::updateView() {
    QImage qi = ref->render().scaledToWidth(200);
    if (qi.height() > 113)
        qi.scaledToHeight(113);
    imgLb->setPixmap(QPixmap::fromImage(qi));
}

void FrameTab::updateNum(int num) {
    numLb->setText(QString::number(num));
}

void FrameTab::setActive(bool active) {
    if (active) {
        QPalette pal = numLb->palette();
        pal.setColor(QPalette::Window, QColor(220, 220, 200));
        numLb->setAutoFillBackground(true);
        numLb->setPalette(pal);
    }
    else
        numLb->setPalette(imgLb->palette());
    numLb->update();
}
