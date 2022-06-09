#ifndef TEXTPANEL_H
#define TEXTPANEL_H

#include <QObject>
#include <QWidget>

namespace Ui {
class TextPanel;
}

class TextPanel : public QWidget
{
    Q_OBJECT
public:
    explicit TextPanel(QWidget *parent = nullptr);

signals:

private:
    Ui::TextPanel *ui;

};

#endif // TEXTPANEL_H
