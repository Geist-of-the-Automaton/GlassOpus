#ifndef RASTERPANEL_H
#define RASTERPANEL_H

#include <QObject>
#include <QWidget>

namespace Ui {
class RasterPanel;
}

class RasterPanel : public QWidget
{
    Q_OBJECT
public:
    explicit RasterPanel(QWidget *parent = nullptr);

signals:

private:
    Ui::RasterPanel *ui;

};

#endif // RASTERPANEL_H
