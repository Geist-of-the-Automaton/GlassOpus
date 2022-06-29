#ifndef TEXTPANEL_H
#define TEXTPANEL_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <drawtext.h>

enum textType {tText = 0, tFont = 1, tSize = 2, tColor = 3, tTaw = 4, tWord = 5, tLetter = 6, tBold = 7, tItal = 8, tUndr = 9, tOver = 10, tStrk = 11};
using std::pair;

namespace Ui {
class TextPanel;
}

class TextPanel : public QWidget
{
    Q_OBJECT
public:
    explicit TextPanel(QWidget *parent = nullptr);
    void updatePanel(pair <textType, unsigned int> dets);
    void updatePanel(QString text);
    void updatePanel(DrawText text);
    void resetPanel();
    pair <textType, unsigned int> getData();
    QString getText();
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void on_content_textChanged();
    void on_fonts_currentIndexChanged(int index);
    void on_sizeSB_valueChanged(int value);
    void on_sizeS_valueChanged(int value);
    void on_colorSB1_valueChanged(int value);
    void on_colorS1_valueChanged(int value);
    void on_tawSB_valueChanged(int value);
    void on_tawS_valueChanged(int value);
    void on_wordSB_valueChanged(int value);
    void on_wordS_valueChanged(int value);
    void on_letterSB_valueChanged(int value);
    void on_letterS_valueChanged(int value);
    void on_bold_toggled(bool which);
    void on_ital_toggled(bool which);
    void on_undr_toggled(bool which);
    void on_over_toggled(bool which);
    void on_strk_toggled(bool which);

signals:
    void actionGiven();

private:
    void changeColor();

    Ui::TextPanel *ui;
    bool vals[5] = {false};

    pair <textType, unsigned int> ret;
    QPoint a;

};

#endif // TEXTPANEL_H
