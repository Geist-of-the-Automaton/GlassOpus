#ifndef BRUSHPANEL_H
#define BRUSHPANEL_H

#include <QObject>
#include <QWidget>

namespace Ui {
class BrushPanel;
}

class BrushPanel : public QWidget
{
    Q_OBJECT
public:
    explicit BrushPanel(QWidget *parent = nullptr);

signals:

private:
    Ui::BrushPanel *ui;

};

#endif // BRUSHPANEL_H
