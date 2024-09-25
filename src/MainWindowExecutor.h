//
// Created by admin on 20.09.24.
//

#ifndef AUTOCONNECTUNIVPN_MAINWINDOWEXECUTOR_H
#define AUTOCONNECTUNIVPN_MAINWINDOWEXECUTOR_H


#include "../ui_mainwindow.h"
#include "UiManager.h"

class MainWindowExecutor {
private:
    Ui::MainWindow* ui;

    DataLoadingResult get_login_data() const;
    DataLoadingResult try_to_load_login_data_from_file(bool uni_password_is_in_config_file = true) const;
    int connect_to_cisco_vpn() const;
	int disconnect_cisco_vpn() const;
	bool save_login_data() const;

public:
    UiManager ui_manager;

    explicit MainWindowExecutor(Ui::MainWindow* _ui) : ui(_ui), ui_manager(ui) {};
    void init_ui();
    CiscoVPN::ReturnValue load_cisco_vpn_status() const;
    void on_connect_button_clicked() const;
    void on_disconnect_button_clicked() const;
    void on_action_reset_triggered() const;
};


#endif //AUTOCONNECTUNIVPN_MAINWINDOWEXECUTOR_H
