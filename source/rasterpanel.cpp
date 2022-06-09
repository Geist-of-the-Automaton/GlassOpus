#include "rasterpanel.h"
#include "ui_rasterpanel.h"

RasterPanel::RasterPanel(QWidget *parent) : QWidget(parent), ui(new Ui::RasterPanel)
{
    ui->setupUi(this);
}
