#include "infodialog.h"
#include "ui_infodialog.h"

InfoDialog::InfoDialog(QWidget *parent) : QDialog(parent), ui(new Ui::InfoDialog) {
    ui->setupUi(this);
    setModal(true); // blocks parent while showing
    setWindowTitle("Info");
    ui->versionLabel->setText("Version " + (QString(PROJECT_VERSION)));
}

InfoDialog::~InfoDialog() {
    delete ui;
}
