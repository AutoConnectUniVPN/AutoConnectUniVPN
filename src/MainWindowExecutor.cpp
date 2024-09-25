//
// Created by admin on 20.09.24.
//

#include "MainWindowExecutor.h"
#include <QInputDialog>
#include <QMessageBox>
#include "UserData.h"
#include "../dialogs/SaveDialog/savedialog.h"
#include <QThread>


void MainWindowExecutor::on_connect_button_clicked() const{
    // update ui
    ui_manager.on_connect_button_clicked();

    // load login
    auto data_loading_result = get_login_data();
    if (data_loading_result != DataLoadingResult::LoadedFromUi && data_loading_result != DataLoadingResult::LoadedFromFile) {
        ui_manager.on_login_data_loading_failed(data_loading_result);
        return;
    }

    // connect to uni vpn
    ui->statusTextLabel->setText("Connecting ...");		// Todo: how to make this with translator
    const int cisco_exit_code = connect_to_cisco_vpn();
    if (cisco_exit_code != CiscoVPN::ReturnValue::Success) {
        ui_manager.on_connection_failed((CiscoVPN::ReturnValue) cisco_exit_code, (CiscoVPN::ProgressBarResultValue) ui->statusProgressBar->value());
        return;
    }
    ui_manager.on_connected();

    // save passwort if not loaded from file
    if (data_loading_result == DataLoadingResult::LoadedFromUi)
        save_login_data();
    // Todo: use return value and give User feedback
}

DataLoadingResult MainWindowExecutor::get_login_data() const {
    // get login data
    if (! UserData::load_from_ui(ui)) {
        if (UserData::is_password_stored_in_config_file()) {
            return try_to_load_login_data_from_file();
        } else {
            bool loaded = UserData::get_uni_password_form_ui(ui);
            if (loaded) {
                const bool decrypted = UserData::load_from_file(UserData::uni_password);
                if (! decrypted)
                    // TODO ERROR
                    return DataLoadingResult::DecryptionFailed;
                else
                    return DataLoadingResult::LoadedFromFile;
            }
            else
                return try_to_load_login_data_from_file(false);
        }
    }
    return DataLoadingResult::LoadedFromUi;
}

DataLoadingResult MainWindowExecutor::try_to_load_login_data_from_file(bool uni_password_is_in_config_file) const {
    // can not load if there is no file to load from
    if (! UserData::exists_configfile())
        return DataLoadingResult::NoConfigFileAndUiInput;

    // try to load without default password
    // if (! need_uni_password) do
    // Todo: implement

    // get password, to decrypt
    bool ok;
    QString decryption_password = QInputDialog::getText(
            ui->rootWidget, "Password",
            uni_password_is_in_config_file ? "Enter encryption password:" : "Enter uni password:",
            QLineEdit::Password, "", &ok
    );

    // analyse feedback
    if (! ok) {
        // canceled
        return DataLoadingResult::UserCanceled;
    } else if (decryption_password.isEmpty()) {
        if (uni_password_is_in_config_file) {
            // no password entered -> use default password
            decryption_password = "123Zauberei";
        } else {
            QMessageBox::critical(ui->rootWidget, "Wrong Password", "The uni password must not be empty");
            return try_to_load_login_data_from_file(false);
        }
    }

    // try to decrypt
    const bool decrypted = UserData::load_from_file(decryption_password.toStdString(), uni_password_is_in_config_file);
    if (decrypted)
        return DataLoadingResult::LoadedFromFile;
    return DataLoadingResult::DecryptionFailed;
}

int MainWindowExecutor::connect_to_cisco_vpn() const {
    const int exit_code = CiscoVPN::connect(
            UserData::uni_id, UserData::uni_password, UserData::seed2fa,
            [this](const std::string& log) {
                this->ui_manager.update_progress_bar(log);
                this->ui_manager.update_log_widget(log, true);
            });
    return exit_code;
}

bool MainWindowExecutor::save_login_data() const {
    // open dialog for password input and save options
    SaveDialog save_dialog;
    save_dialog.show();
    save_dialog.exec();  // wait until closed
    if (save_dialog.get_cancel_pressed())
        return false;
    // choose encryption password
    std::string encryption_password = save_dialog.is_password_empty()
                                      ? (save_dialog.get_save_uni_password() ? "123Zauberei" : UserData::uni_password)
                                      : save_dialog.get_encryption_password();
    // save login data
    return UserData::store_to_file(encryption_password, save_dialog.get_save_uni_password());
}

void MainWindowExecutor::on_disconnect_button_clicked() const {
    ui_manager.on_disconnect_button_clicked();
    disconnect_cisco_vpn();
}

int MainWindowExecutor::disconnect_cisco_vpn() const {
	int exit_code = CiscoVPN::disconnect([this](const std::string& log) {
		this->ui_manager.update_log_widget(log, true);
	});
	if (exit_code != CiscoVPN::ReturnValue::Success)
		ui_manager.on_disconnected_failed(exit_code);
	else
		ui_manager.on_disconnected();
	return exit_code;
}

void MainWindowExecutor::init_ui() {
    ui_manager.reset_ui();
}

CiscoVPN::ReturnValue MainWindowExecutor::load_cisco_vpn_status() const {
    const CiscoVPN::ReturnValue exit_code = CiscoVPN::status([this](const std::string& log) {
        this->ui_manager.update_log_widget(log, true);
    });
	QThread::sleep(1);
	ui_manager.on_status(exit_code);
	if (exit_code == CiscoVPN::ReturnValue::CouldNotAnalyseStatusLog) {
		ui_manager.update_log_widget("\n\n", false);
		disconnect_cisco_vpn();
	}
    return exit_code;
}

void MainWindowExecutor::on_action_reset_triggered() const {
    auto res = QMessageBox::question(ui->rootWidget, "Reset", "Delete all user data?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (res == QMessageBox::Yes) {
        if (UserData::delete_all())
            QMessageBox::information(ui->rootWidget, "Succeeded", "All data deleted");
        else
            QMessageBox::information(ui->rootWidget, "Failed", "Failed to delete data. See \"help\" to delete the data manually");
        ui_manager.update_hint_for_login_data_found();
    }
}
