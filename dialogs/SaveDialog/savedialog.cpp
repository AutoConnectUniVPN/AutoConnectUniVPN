#include "savedialog.h"
#include "ui_savedialog.h"

#include <QMessageBox>
#include <iostream>


SaveDialog::SaveDialog(MainWindow *parent) : QDialog(parent), ui(new Ui::SaveDialog) {
    // setup dialog
	ui->setupUi(this);
	this->resize(dialog_size);
    setModal(true); // blocks parent while showing
    setWindowTitle("Save User Data");
	hide_password_input_fields();
	// make check button clickable
	auto checkButton = connect(
			ui->saveUniPasswordCheckBox, SIGNAL(stateChanged(int)), this,
			SLOT(_on_save_uni_password_check_box_check_state_changed(int))
	);
	this->adjustSize();
}

SaveDialog::~SaveDialog() {
    delete ui;
}

void SaveDialog::accept() {
	// verify password
	if (ui->encryptionPasswordLineEdit->text() != ui->encryptionPasswordVerifyLineEdit->text()) {
		QMessageBox::warning(nullptr, "Bad verify", "Entered passwords do not match");
		return;
	}
	// let confirm, if user wants to use default password
	if (is_password_empty() && ui->saveUniPasswordCheckBox->isChecked()) {
		auto res = QMessageBox::question(this, "No Password?", "Are you sure you want to use the\nmore vulnerable default password?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
		if (res == QMessageBox::No)
			return;
	}
	// close
	canceled = false;
	QDialog::accept();
}

bool SaveDialog::get_cancel_pressed() const {
	return canceled;
}

std::string SaveDialog::get_encryption_password() {
	return ui->encryptionPasswordLineEdit->text().toStdString();
}

bool SaveDialog::get_save_uni_password() {
	return ui->saveUniPasswordCheckBox->isChecked();
}

bool SaveDialog::is_password_empty() {
	return ui->encryptionPasswordLineEdit->text().isEmpty();
}


void SaveDialog::_on_save_uni_password_check_box_check_state_changed(int checkState) {
	// show or hide password input fields
	if (checkState == Qt::CheckState::Checked) {
		show_password_input_fields();
	} else {
		hide_password_input_fields();
		this->resize(dialog_size);
	}
	// make sure, resize has an effect
	this->adjustSize();
}

void SaveDialog::hide_password_input_fields() {
	ui->PasswordLabel->hide();
	ui->VerifyLabel->hide();
	ui->encryptionPasswordLineEdit->hide();
	ui->encryptionPasswordVerifyLineEdit->hide();
}

void SaveDialog::show_password_input_fields() {
	ui->PasswordLabel->show();
	ui->VerifyLabel->show();
	ui->encryptionPasswordLineEdit->show();
	ui->encryptionPasswordVerifyLineEdit->show();
}

