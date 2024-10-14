//
// Created by admin on 27.08.24.
//

#include "UiManager.h"

#include <QMessageBox>
#include "UserData.h"
#include "../libs/CiscoVPN/LogProgressPhrases.h"

void UiManager::clear_log() const {
	ui->logOutputTextBrowser->clear();
	ui->logOutputTextBrowser->repaint();
}

void UiManager::on_to_log_button_clicked() const {
	hide_input_widgets();
	ui->toLogButton->setDisabled(true);
	ui->toDataInputButton->setEnabled(true);
	show_log_widgets();
}

void UiManager::on_to_data_input_button_clicked() const {
	ui->toDataInputButton->setDisabled(true);
	ui->toLogButton->setEnabled(true);
	hide_log_widgets();
	show_input_widgets();
}

void UiManager::reset_ui() const {
	ui->toDataInputButton->setDisabled(true);
	ui->toLogButton->setDisabled(true);
	show_navigation_widgets();
	show_log_widgets();
	hide_input_widgets();
	ui->connectButton->setDisabled(true);
	ui->disconnectButton->setDisabled(true);
	ui->connectButton->show();
	ui->disconnectButton->hide();
	ui->statusTextLabel->setText("Load Status ...");
	ui->statusProgressBar->setValue(0);
	ui->statusProgressBar->hide();
    update_hint_for_login_data_found();
}

void UiManager::show_input_widgets() const {
	ui->uniIDLabel->show();
	ui->uniPasswordLabel->show();
	ui->uniSeedLabel->show();
	ui->uniIDLineEdit->show();
	ui->uniPasswordLineEdit->show();
	ui->uniSeedLineEdit->show();
}

void UiManager::hide_input_widgets() const {
	ui->uniIDLabel->hide();
	ui->uniPasswordLabel->hide();
	ui->uniSeedLabel->hide();
	ui->uniIDLineEdit->hide();
	ui->uniPasswordLineEdit->hide();
	ui->uniSeedLineEdit->hide();
}

void UiManager::show_log_widgets() const {
	ui->logOutputTextBrowser->show();
}

void UiManager::hide_log_widgets() const {
	ui->logOutputTextBrowser->hide();
}

void UiManager::show_navigation_widgets() const {
	ui->toLogButton->show();
	ui->toDataInputButton->show();
}

void UiManager::hide_navigation_widgets() const {
	ui->toDataInputButton->hide();
	ui->toLogButton->hide();
}

void UiManager::update_progress_bar(const std::string_view& log) const {
	auto& progress_bar = ui->statusProgressBar;
	if (log.find(LogProgressPhrases::XX_1) != std::string::npos)
		if (progress_bar->value() < 1)
			progress_bar->setValue(1);
	if (log.find(LogProgressPhrases::XX_5) != std::string::npos)
		if (progress_bar->value() < 5)
			progress_bar->setValue(5);
	if (log.find(LogProgressPhrases::EN_8) != std::string::npos || log.find(LogProgressPhrases::DE_8) != std::string::npos)
		if (progress_bar->value() < 8)
			progress_bar->setValue(8);
	if (log.find(LogProgressPhrases::XX_10) != std::string::npos)
		if (progress_bar->value() < (int) CiscoVPN::ProgressBarResultValue::CannotContactHost)
			progress_bar->setValue((int) CiscoVPN::ProgressBarResultValue::CannotContactHost);
	if (log.find(LogProgressPhrases::EN_20) != std::string::npos || log.find(LogProgressPhrases::DE_20) != std::string::npos)
		if (progress_bar->value() < (int) CiscoVPN::ProgressBarResultValue::WrongUniIdOrPassword)
			progress_bar->setValue((int) CiscoVPN::ProgressBarResultValue::WrongUniIdOrPassword);
	if (log.find(LogProgressPhrases::XX_30) != std::string::npos)
		if (progress_bar->value() < (int) CiscoVPN::ProgressBarResultValue::WrongOTPCode)
			progress_bar->setValue((int) CiscoVPN::ProgressBarResultValue::WrongOTPCode);
	if (log.find(LogProgressPhrases::EN_40) != std::string::npos || log.find(LogProgressPhrases::DE_40) != std::string::npos)
		if (progress_bar->value() < 40)
			progress_bar->setValue(40);
	if (log.find(LogProgressPhrases::XX_44) != std::string::npos)
		if (progress_bar->value() < 44)
			progress_bar->setValue(44);
	if (log.find(LogProgressPhrases::XX_46) != std::string::npos)
		if (progress_bar->value() < 46)
			progress_bar->setValue(46);
	if (log.find(LogProgressPhrases::XX_50) != std::string::npos)
		if (progress_bar->value() < (int) CiscoVPN::ProgressBarResultValue::CannotAccept)
			progress_bar->setValue((int) CiscoVPN::ProgressBarResultValue::CannotAccept);
	if (log.find(LogProgressPhrases::XX_53) != std::string::npos)
		if (progress_bar->value() < (int) CiscoVPN::ProgressBarResultValue::CannotAcceptWithY)
			progress_bar->setValue((int) CiscoVPN::ProgressBarResultValue::CannotAcceptWithY);
	if (log.find(LogProgressPhrases::EN_56) != std::string::npos)  // no german counterpart
		if (progress_bar->value() < 56)
			progress_bar->setValue(56);
	if (log.find(LogProgressPhrases::XX_60) != std::string::npos)
		if (progress_bar->value() < 60)
			progress_bar->setValue(60);
	if (log.find(LogProgressPhrases::EN_64) != std::string::npos || log.find(LogProgressPhrases::DE_64) != std::string::npos)
		if (progress_bar->value() < 64)
			progress_bar->setValue(64);
	if (log.find(LogProgressPhrases::EN_68) != std::string::npos || log.find(LogProgressPhrases::DE_68) != std::string::npos)
		if (progress_bar->value() < 68)
			progress_bar->setValue(68);
	if (log.find(LogProgressPhrases::EN_72) != std::string::npos || log.find(LogProgressPhrases::DE_72) != std::string::npos)
		if (progress_bar->value() < 72)
			progress_bar->setValue(72);
	if (log.find(LogProgressPhrases::EN_76) != std::string::npos || log.find(LogProgressPhrases::DE_76) != std::string::npos)
		if (progress_bar->value() < 76)
			progress_bar->setValue(76);
	if (log.find(LogProgressPhrases::EN_77_1) != std::string::npos && log.find(LogProgressPhrases::EN_77_2) != std::string::npos || log.find(LogProgressPhrases::DE_77_1_unsure1) != std::string::npos || log.find(LogProgressPhrases::DE_77_1_unsure2) != std::string::npos)
		if (progress_bar->value() < (int) CiscoVPN::ProgressBarResultValue::ExitingCauseUpdateInProgress)
			progress_bar->setValue((int) CiscoVPN::ProgressBarResultValue::ExitingCauseUpdateInProgress);
	if (log.find(LogProgressPhrases::XX_80) != std::string::npos)
		if (progress_bar->value() < 80)
			progress_bar->setValue(80);
	if (log.find(LogProgressPhrases::EN_84) != std::string::npos || log.find(LogProgressPhrases::DE_84) != std::string::npos)
		if (progress_bar->value() < 84)
			progress_bar->setValue(84);
	if (log.find(LogProgressPhrases::EN_86) != std::string::npos || log.find(LogProgressPhrases::DE_86) != std::string::npos)
		if (progress_bar->value() < 86)
			progress_bar->setValue(86);
	if (log.find(LogProgressPhrases::EN_90) != std::string::npos || log.find(LogProgressPhrases::DE_90) != std::string::npos)
		if (progress_bar->value() < 90)
			progress_bar->setValue(90);
	if (log.find(LogProgressPhrases::EN_94) != std::string::npos || log.find(LogProgressPhrases::DE_94) != std::string::npos)
		if (progress_bar->value() < 94)
			progress_bar->setValue(94);
	if (log.find(LogProgressPhrases::EN_96) != std::string::npos || log.find(LogProgressPhrases::DE_96) != std::string::npos)
		if (progress_bar->value() < 96)
			progress_bar->setValue(96);
	if (log.find(LogProgressPhrases::EN_100_1) != std::string::npos || log.find(LogProgressPhrases::EN_100_1) != std::string::npos || log.find(LogProgressPhrases::XX_100_2) != std::string::npos)
		progress_bar->setValue((int) CiscoVPN::ProgressBarResultValue::Success);
}

void UiManager::update_log_widget(const std::string& log, bool prettify = false) const {
	auto prettified = QString::fromStdString(log);
	if (prettify) {
		prettified.remove("\n");
		prettified.remove("\r");
		prettified.remove("VPN> ");
		prettified.replace("`n", "\n");
	}
	ui->logOutputTextBrowser->append(prettified);
	ui->logOutputTextBrowser->repaint();
}

void UiManager::update_hint_for_login_data_found() const {
	if (UserData::exists_configfile()) {
		ui->uniIDLineEdit->setPlaceholderText("********");
		ui->uniPasswordLineEdit->setPlaceholderText(UserData::is_password_stored_in_config_file() ? "********" : "");
		ui->uniSeedLineEdit->setPlaceholderText("********");
	} else {
        ui->uniIDLineEdit->setPlaceholderText("");
        ui->uniPasswordLineEdit->setPlaceholderText("");
        ui->uniSeedLineEdit->setPlaceholderText("");
    }
    ui->uniIDLineEdit->repaint();
    ui->uniPasswordLineEdit->repaint();
    ui->uniSeedLineEdit->repaint();
}

void UiManager::on_connect_button_clicked() const {
	clear_log();
	hide_navigation_widgets();
	hide_input_widgets();
	ui->statusProgressBar->setValue(0);
	ui->statusProgressBar->show();
	ui->connectButton->setDisabled(true);
	show_log_widgets();
	ui->statusTextLabel->setText("Load login data ...");		// Todo: how to make this with translator
}

void UiManager::on_connected() const {
	ui->statusTextLabel->setText("Connected");
	ui->connectButton->hide();
	ui->disconnectButton->show();
	ui->disconnectButton->setDisabled(false);
}

void UiManager::on_disconnect_button_clicked() const {
	ui->disconnectButton->setDisabled(true);
	clear_log();
}

void UiManager::on_disconnected() const {
	ui->toDataInputButton->setEnabled(true);
	ui->toLogButton->setDisabled(true);
	show_navigation_widgets();
	show_log_widgets();
	hide_input_widgets();
	ui->disconnectButton->setDisabled(true);
	ui->disconnectButton->hide();
	ui->connectButton->setEnabled(true);
	ui->connectButton->show();
	ui->statusProgressBar->hide();
	ui->statusTextLabel->setText("Disconnected");
}


void UiManager::on_disconnected_failed(int cisco_exit_code) const {
	ui->disconnectButton->setEnabled(true);
	ui->statusTextLabel->setText("Disconnecting failed (" + QString::number(cisco_exit_code) + ")");
}

void UiManager::on_login_data_loading_failed(DataLoadingResult reason) const {
    on_login_data_loading_failed_default_reset();
	switch (reason) {
		case DataLoadingResult::UserCanceled:
            // default reset is enough
			break;
		case DataLoadingResult::NoConfigFileAndUiInput:
			on_login_data_loading_no_data_source();
			break;
		case DataLoadingResult::DecryptionFailed:
			on_login_data_loading_decryption_failed();
			break;
		default:
			throw std::runtime_error("This is an unknown login data loading result in UiManager.cpp");
	}

}

void UiManager::on_login_data_loading_failed_default_reset() const {
    ui->connectButton->setEnabled(true);
    ui->statusTextLabel->setText("Disconnected");
    ui->statusProgressBar->hide();
    hide_log_widgets();
    show_input_widgets();
    show_navigation_widgets();
}

void UiManager::on_login_data_loading_no_data_source() const {
	QMessageBox::warning(
			nullptr,
			"Failed", "The login data could not be loaded.\n\n"
					  "Reason: There is no config file to load from and it was not possible to load login data from the input fields.\n"
					  "Please make sure, that all input fields are filled in."
	);
}

void UiManager::on_login_data_loading_decryption_failed() const {
	QMessageBox::warning(
			nullptr,
			"Failed", "The login data could not be loaded.\n\n"
					  "Reason: Could not decrypt the config file.\n"
					  "Please make sure, that all input fields are filled in correctly."
	);
}

void UiManager::on_connection_failed(CiscoVPN::ReturnValue cisco_exit_code, CiscoVPN::ProgressBarResultValue progress_bar_result_value) const {
	on_connection_failed_default_reset();
	// first evaluate ALL progress bar results and then check exit cases
	switch (progress_bar_result_value) {
		case CiscoVPN::ProgressBarResultValue::NotEvenStarted:
			switch (cisco_exit_code) {
				case CiscoVPN::ReturnValue::PopenFailed:
					ui->statusTextLabel->setText("Connection Failed: Cannot connect to Cisco Client");
					break;
				case CiscoVPN::InvalidSeed:
					ui->statusTextLabel->setText("OTP code generation failed. Check the seed.");
					break;
				default:
					ui->statusTextLabel->setText("Connection Failed: Cannot connect to Cisco Client (" + QString::number(cisco_exit_code) + ")");
					break;
			}
			break;
		case CiscoVPN::ProgressBarResultValue::WrongUniIdOrPassword:
			ui->statusTextLabel->setText("Connection Failed: Wrong uni id or password (" + QString::number(cisco_exit_code) + ")");
			break;
		case CiscoVPN::ProgressBarResultValue::CannotContactHost:
			ui->statusTextLabel->setText("Connection Failed: Cannot contact host (" + QString::number(cisco_exit_code) + ")");
			break;
		case CiscoVPN::ProgressBarResultValue::WrongOTPCode:
			ui->statusTextLabel->setText("Connection Failed: Wrong OTP Code.\nPlease try again or check the seed (" + QString::number(cisco_exit_code) + ")");
			break;
		case CiscoVPN::ProgressBarResultValue::CannotAccept:
			ui->statusTextLabel->setText("Connection Failed: Cannot accept connection (" + QString::number(cisco_exit_code) + ")");
			break;
		case CiscoVPN::ProgressBarResultValue::CannotAcceptWithY:
			ui->statusTextLabel->setText("Connection Failed: Cannot enter connection (" + QString::number(cisco_exit_code) + ")");
			break;
		case CiscoVPN::ProgressBarResultValue::ExitingCauseUpdateInProgress:
			ui->statusTextLabel->setText("Cisco performs an update. Please try again (" + QString::number(cisco_exit_code) + ").");
			break;
		case CiscoVPN::ProgressBarResultValue::Success:
			// should never happen, because this is error handling
			break;
		default:
			ui->statusTextLabel->setText("Connection Failed: See log for more info (" + QString::number(cisco_exit_code) + ")");
			break;
	}
}

void UiManager::on_connection_failed_default_reset() const {
	show_navigation_widgets();
	ui->toLogButton->setDisabled(true);
	ui->connectButton->setEnabled(true);
	ui->statusTextLabel->setText("Connection Failed");
	ui->toDataInputButton->setEnabled(true);
}

void UiManager::on_status(CiscoVPN::ReturnValue exit_code) const {
	switch (exit_code) {
		case CiscoVPN::DisconnectedReadyToConnect:
			on_status_default_action();
			on_status_disconnected_ready_to_connect();
			break;
		case CiscoVPN::Disconnected:
			on_status_default_action();
			on_status_disconnected();
			break;
		case CiscoVPN::Connected:
			on_status_default_action();
			on_status_connected();
			break;
		case CiscoVPN::CouldNotAnalyseStatusLog:
			// do not perform default action
			on_status_failed_to_extract();
			break;
		case CiscoVPN::FailedToLoadStatus:
			on_status_default_action();
			on_status_failed();
			break;
		case CiscoVPN::DisconnectedNoInternetConnection:
			update_log_widget("\n\nPlease restart programm, when internet connection is available.");
			ui->statusTextLabel->setText("Disconnected: No internet connection");
			break;
		default:
			on_status_default_action();
			break;
	}
}

void UiManager::on_status_default_action() const {
	ui->connectButton->setEnabled(true);
	show_navigation_widgets();
	ui->toLogButton->setEnabled(false);
	ui->toDataInputButton->setEnabled(true);
}

void UiManager::on_status_disconnected_ready_to_connect() const {
	ui->toLogButton->setEnabled(true);
	ui->toDataInputButton->setEnabled(false);
    hide_log_widgets();
    show_input_widgets();
	ui->statusTextLabel->setText("Disconnected: Ready to connect");
}

void UiManager::on_status_disconnected() const {
	ui->statusTextLabel->setText("Disconnected");
}

void UiManager::on_status_connected() const {
	ui->connectButton->setDisabled(true);
	ui->connectButton->hide();
	ui->disconnectButton->setEnabled(true);
	ui->disconnectButton->show();
	hide_navigation_widgets();
	hide_input_widgets();
	show_log_widgets();
	ui->statusTextLabel->setText("Connected");
}

void UiManager::on_status_failed_to_extract() const {
	ui->statusTextLabel->setText("Unknown: Resetting VPN Connection ...");
}

void UiManager::on_status_failed() const {
	ui->statusTextLabel->setText("Status loading failed. Please read log");
}
