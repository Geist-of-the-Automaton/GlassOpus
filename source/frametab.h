#ifndef FRAMETAB_H
#define FRAMETAB_H

#include <QObject>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <layer.h>

class FrameTab : public QHBoxLayout
{
    Q_OBJECT
public:
    explicit FrameTab(QWidget *parent = nullptr);
    ~FrameTab();
    void refLayer(Layer *layer);
    void updateView();
    void updateNum(int num);
    void setActive(bool active);

private:
    QLabel *numLb, *imgLb;
    QPushButton *visPb;
    Layer *ref;

};

#endif // FRAMETAB_H
