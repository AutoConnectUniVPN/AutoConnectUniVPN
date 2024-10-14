//
// Created by Admin on 22.08.2024.
//

#include "CiscoCommandGenerator.h"


const std::string CiscoCommandGenerator::cisco_path_linux = R"(/opt/cisco/secureclient/bin/vpn)";
const std::string CiscoCommandGenerator::cisco_path_windows = R"(C:\Program Files (x86)\Cisco\Cisco Secure Client\vpncli.exe)";
const std::string CiscoCommandGenerator::cisco_path_apple = R"()";

const std::string CiscoCommandGenerator::vpn_server_address = "vpn-ac.urz.uni-heidelberg.de";


std::string CiscoCommandGenerator::command_connect_linux(const std::string& uni_id, const std::string& uni_password, const std::string& code_2fa) {
    // use pid and uuidgen, to make sure, input does not match delimiter
    const std::string nl = "\n";  // new line
    std::string command = cisco_path_linux + " -s << 'EOF$$_UUID:$(uuidgen)'\n"
                                             "connect vpn-ac.urz.uni-heidelberg.de\n"
                                             + uni_id + nl
                                             + uni_password + nl
                                             + code_2fa + nl
                                             + "y\nEOF$$_UUID:$(uuidgen)";
    return command;
}

std::string CiscoCommandGenerator::command_connect_windows(const std::string& uni_id, const std::string& uni_password, const std::string& code_2fa) {
    const std::string nl = "`n"; // new line
    std::string command = R"(PowerShell -WindowStyle hidden -Command "echo 'connect )"
                        + vpn_server_address + R"(' ')"
                        + uni_id + R"(' ')"
                        + uni_password + R"(' ')"
                        + code_2fa + R"(' ')"
                        + R"(y' | & ')" + cisco_path_windows + R"(' -s")";
    return command;
}

std::string CiscoCommandGenerator::command_connect_apple(const std::string& uni_id, const std::string& uni_password, const std::string& code_2fa) {
    throw "Not implemented yet";
}


std::string CiscoCommandGenerator::disconnect_linux() {
    return "\"" + cisco_path_linux + "\" disconnect";
}

std::string CiscoCommandGenerator::disconnect_windows() {
	return R"(PowerShell -WindowStyle hidden -Command "& \")" + cisco_path_windows + R"(\" disconnect")";
}

std::string CiscoCommandGenerator::disconnect_apple() {
    throw "Not implemented yet";
}


std::string CiscoCommandGenerator::status_linux() {
	return "\"" + cisco_path_linux + "\" status";
}

std::string CiscoCommandGenerator::status_windows() {
	return R"(PowerShell -WindowStyle hidden -Command "& \")" + cisco_path_windows + R"(\" status")";
}

std::string CiscoCommandGenerator::status_apple() {
	throw "Not implemented yet";
}
