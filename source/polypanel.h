#ifndef POLYPANEL_H
#define POLYPANEL_H

#include <QObject>
#include <QWidget>


namespace Ui {
class PolyPanel;
}

class PolyPanel : public QWidget
{
    Q_OBJECT
public:
    explicit PolyPanel(QWidget *parent = nullptr);

signals:

private:
    Ui::PolyPanel *ui;

};

#endif // POLYPANEL_H
