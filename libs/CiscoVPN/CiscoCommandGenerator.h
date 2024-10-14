//
// Created by Admin on 22.08.2024.
//

#ifndef CISCOCOMMANDGENERATOR_H
#define CISCOCOMMANDGENERATOR_H
#include <string>


class CiscoCommandGenerator {
    static const std::string cisco_path_linux;
    static const std::string cisco_path_windows;
    static const std::string cisco_path_apple;

    static const std::string vpn_server_address;

public:
    static std::string command_connect_linux(const std::string& uni_id, const std::string& uni_password, const std::string& code_2fa);
    static std::string command_connect_windows(const std::string& uni_id, const std::string& uni_password, const std::string& code_2fa);
    static std::string command_connect_apple(const std::string& uni_id, const std::string& uni_password, const std::string& code_2fa);

    static std::string disconnect_linux();
    static std::string disconnect_windows();
    static std::string disconnect_apple();

    static std::string status_linux();
    static std::string status_windows();
    static std::string status_apple();
};



#endif //CISCOCOMMANDGENERATOR_H
