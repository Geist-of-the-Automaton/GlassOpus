#include "textpanel.h"
#include "ui_textpanel.h"

TextPanel::TextPanel(QWidget *parent) : QWidget(parent), ui(new Ui::TextPanel)
{
    ui->setupUi(this);
}
