#ifndef BUGREPORTDIALOG_H
#define BUGREPORTDIALOG_H

#include <QDialog>

namespace Ui {
class BugReportDialog;
}

class BugReportDialog : public QDialog {
    Q_OBJECT

public:
    explicit BugReportDialog(QWidget *parent = nullptr);
    ~BugReportDialog();

private:
    Ui::BugReportDialog *ui;
};

#endif // BUGREPORTDIALOG_H
