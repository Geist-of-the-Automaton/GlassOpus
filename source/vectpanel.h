#ifndef VECTPANEL_H
#define VECTPANEL_H

#include <QObject>
#include <QWidget>

namespace Ui {
class VectPanel;
}

class VectPanel : public QWidget
{
    Q_OBJECT
public:
    explicit VectPanel(QWidget *parent = nullptr);

signals:

private:
    Ui::VectPanel *ui;

};

#endif // VECTPANEL_H
