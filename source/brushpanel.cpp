#include "brushpanel.h"
#include "ui_brushpanel.h"

BrushPanel::BrushPanel(QWidget *parent) : QWidget(parent), ui(new Ui::BrushPanel)
{
    ui->setupUi(this);
}
