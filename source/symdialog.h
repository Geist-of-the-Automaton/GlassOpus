#ifndef SYMDIALOG_H
#define SYMDIALOG_H

#include <QDialog>
#include <QPainter>

namespace Ui {
class SymDialog;
}

class SymDialog : public QDialog
{
        Q_OBJECT
public:
    SymDialog(QWidget *parent = nullptr);
    ~SymDialog();
    int getDiv();
    int getOfEvery();
    int getSkip();
    void setIndicies(int s1, int s2, int s3);

private slots:
    void divChange();
    void ofEveryChange();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::SymDialog *ui;
    int div, ofEvery, skip;

};


#endif // SYMDIALOG_H
