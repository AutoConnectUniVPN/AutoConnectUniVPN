//
// Created by admin on 27.08.24.
//

#ifndef AUTOCONNECTUNIVPN_UIMANAGER_H
#define AUTOCONNECTUNIVPN_UIMANAGER_H


#include "../ui_mainwindow.h"
#include "DataLoadingResult.h"
#include "../libs/CiscoVPN/CiscoVPN.h"

class UiManager {
private:
	Ui::MainWindow* ui;

	void on_login_data_loading_no_data_source() const;
	void on_login_data_loading_decryption_failed() const;
	void on_login_data_loading_failed_default_reset() const;
	void on_connection_failed_default_reset() const;
	void on_status_default_action() const;
	void on_status_disconnected_ready_to_connect() const;
	void on_status_disconnected() const;
	void on_status_connected() const;
	void on_status_failed_to_extract() const;
	void on_status_failed() const;

public:
	explicit UiManager(Ui::MainWindow* _ui) : ui(_ui) {};
	void clear_log() const;
	void on_to_log_button_clicked() const;
	void on_to_data_input_button_clicked() const;
	void reset_ui() const;
	void show_input_widgets() const;
	void hide_input_widgets() const;
	void show_log_widgets() const;
	void hide_log_widgets() const;
	void show_navigation_widgets() const;
	void hide_navigation_widgets() const;
	void update_progress_bar(const std::string_view& log) const;
	void update_log_widget(const std::string& log, bool prettify) const;
	void update_hint_for_login_data_found() const;
	void on_connect_button_clicked() const;
	void on_connected() const;
	void on_connection_failed(CiscoVPN::ReturnValue cisco_exit_code, CiscoVPN::ProgressBarResultValue progress_bar_result_value) const;
	void on_disconnect_button_clicked() const;
	void on_disconnected() const;
	void on_disconnected_failed(int cisco_exit_code) const;
	void on_login_data_loading_failed(DataLoadingResult reason) const;
	void on_status(CiscoVPN::ReturnValue exit_code) const;
};


#endif //AUTOCONNECTUNIVPN_UIMANAGER_H
