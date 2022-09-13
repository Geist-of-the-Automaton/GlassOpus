#ifndef HISTODIALOG_H
#define HISTODIALOG_H

#include <QDialog>
#include <QPainter>
#include <QFileDialog>

#include <graphics.h>

using graphics::Color;

namespace Ui {
class HistoDialog;
}

class HistoDialog : public QDialog
{
        Q_OBJECT
public:
    HistoDialog(QWidget *parent = nullptr);
    ~HistoDialog();
    void setWork(QImage *toProcess);

private slots:
    void process();
    void matchChanged();
    void on_clipSB_valueChanged(double value);
    void on_clipS_valueChanged(int value);
    void on_xdivS_valueChanged(int value);
    void on_xdivSB_valueChanged(int value);
    void on_ydivS_valueChanged(int value);
    void on_ydivSB_valueChanged(int value);
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::HistoDialog *ui;
    QImage *qi, processed, toMatch;
    QImage histos[4];

};

#endif // HISTODIALOG_H
