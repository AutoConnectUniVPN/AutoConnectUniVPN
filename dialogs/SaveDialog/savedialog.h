#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>
#include "../../mainwindow.h"

namespace Ui {
class SaveDialog;
}

class SaveDialog : public QDialog {
private:
	Q_OBJECT
	Ui::SaveDialog *ui;
	bool canceled = true;
	const QSize dialog_size = QSize(290, 130);	// for resizing after hiding password input fields

	void accept() override;
	void hide_password_input_fields();
	void show_password_input_fields();

private slots:
    void _on_save_uni_password_check_box_check_state_changed(int checkState);

public:
    explicit SaveDialog(MainWindow *parent = nullptr);
    ~SaveDialog();

	bool get_cancel_pressed() const;
	std::string get_encryption_password();
	bool get_save_uni_password();
	bool is_password_empty();
};

#endif // SAVEDIALOG_H
