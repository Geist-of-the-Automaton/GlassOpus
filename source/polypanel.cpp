#include "polypanel.h"
#include "ui_polypanel.h"

PolyPanel::PolyPanel(QWidget *parent) : QWidget(parent), ui(new Ui::PolyPanel)
{
    ui->setupUi(this);
}
