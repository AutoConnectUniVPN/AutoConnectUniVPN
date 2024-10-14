#include "bugreportdialog.h"
#include "ui_bugreportdialog.h"

BugReportDialog::BugReportDialog(QWidget *parent) : QDialog(parent), ui(new Ui::BugReportDialog) {
    ui->setupUi(this);
    setModal(true); // blocks parent while showing
    setWindowTitle("Bug Report");
}

BugReportDialog::~BugReportDialog() {
    delete ui;
}
