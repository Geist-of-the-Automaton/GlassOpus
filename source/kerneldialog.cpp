#include "kerneldialog.h"
#include "ui_kerneldialog.h"

KernelDialog::KernelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KernelDialog)
{
    lock = 0;
    ui->setupUi(this);
    setWindowTitle("Convolution - Kernel Application");
    connect(ui->qcb1, SIGNAL(currentIndexChanged(int)), this, SLOT(comboChange()));
}

KernelDialog::~KernelDialog() {
    delete ui;
}

void KernelDialog::setWork(QImage *toProcess) {
    qi = toProcess;
    processed = qi->scaled(qi->width() / 3, qi->height() / 3, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    proDisp = processed.copy();
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
    QDir qdir(QDir::currentPath() + Kernel_Loc);
    QStringList items, temp = qdir.entryList();
    temp.pop_front();
    temp.pop_front();
    items.push_back("Box Blur");
    items.push_back("Cone Blur");
    for (QString &qs : temp) {
        string str = qs.toStdString();
        str = str.substr(0, str.find_last_of("."));
        items.push_back(str.c_str());
    }
    ui->qcb1->clear();
    ui->qcb1->addItems(items);
    ui->qcb1->setCurrentIndex(0);
    ui->spinbox->setValue(1);
    ui->spinbox->setEnabled(true);
    kd = KernelData(false, Filtering::getBoxBlur(1));
    //kerPrompt.setWhatsThis("Kernals allow for a variety of effects to be applied to the image");
    process();
}

void KernelDialog::on_spinbox_valueChanged(int value) {
    if (lock) {
        ui->spinbox->setValue(val);
        return;
    }
    lock = 1;
    val = value;
    int index = ui->qcb1->currentIndex();
    if (index == 0)
        kd.second = Filtering::getBoxBlur(val);
    else if (index == 1)
        kd.second = Filtering::getConeBlur(val);
    process();
    lock = 0;
}

void KernelDialog::comboChange() {
    int index = ui->qcb1->currentIndex();
    ui->spinbox->setValue(1);
    if (index > 1) {
        ui->spinbox->setEnabled(false);
        string fileName = (QDir::currentPath() + Kernel_Loc).toStdString() + ui->qcb1->currentText().toStdString() + ".txt";
        kd = ImgSupport::loadKernel(fileName);
        Kernel k = kd.second;
        if (!(k.size() == 1 && k[0].size() == 1 && k[0][0] == 1.0)) {
            //could not load
        }
    }
    else {
        ui->spinbox->setEnabled(true);
        Kernel k;
        int index = ui->qcb1->currentIndex();
        if (index == 0)
            k = Filtering::getBoxBlur(1);
        else if (index == 1)
            k = Filtering::getConeBlur(1);
        kd = KernelData(false, k);
    }
    process();
}

void KernelDialog::process() {
    proDisp = processed.copy();
    QProgressDialog qpd("Updating Views", "", 0, 0, this, Qt::WindowType::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    qpd.setCancelButton(nullptr);
    Filtering::applyKernel(&qpd, &proDisp, kd);
    ui->label->setPixmap(QPixmap::fromImage(proDisp));
}

void KernelDialog::on_buttonBox_accepted() {
    QProgressDialog qpd("Updating Views", "", 0, 0, this, Qt::WindowType::FramelessWindowHint);
    qpd.setCancelButton(nullptr);
    long long time = stdFuncs::getTime();
    Filtering::applyKernel(&qpd, qi, kd);
    cout << stdFuncs::getTime(time) << endl;
    done(1);
}

void KernelDialog::on_buttonBox_rejected() {
    done(0);
}

