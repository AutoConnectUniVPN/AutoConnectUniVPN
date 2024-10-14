#include "CiscoVPN.h"

#include "CiscoCommandGenerator.h"
#include "../CCOTP/totp.h"
#include "../CCOTP/totp_exceptions.h"

#ifdef __linux__
#include <linux/limits.h>
#endif


int CiscoVPN::connect(const std::string& uni_id, const std::string& uni_password, const std::string& seed_2fa, const CallbackOutputFunction& callback) {
	std::string command;

	// calculate 2fa code
	std::string code_2fa;
	try {code_2fa = totp_code(seed_2fa);}
	catch (const invalid_hex_character&) {
		callback(" > Invalid characters in 2FA seed!");
		callback(" > Please ensure that you only use 0-9 and a-f characters");
		return InvalidSeed;
	}
	callback(" >> 2FA Code Generated: " + code_2fa + "`n");


	// choose correct command depending on os
	#ifdef __linux__
		command = CiscoCommandGenerator::command_connect_linux(uni_id, uni_password, code_2fa);
	#elif defined(__WIN32)
		command = CiscoCommandGenerator::command_connect_windows(uni_id, uni_password, code_2fa);
	#elif defined(__APPLE__)
		command = CiscoCommandGenerator::command_connect_apple(uni_id, uni_password, code_2fa);
	#else
		This is not allowed ...
	#endif

	// run command
	FILE *output_stream = popen(command.c_str(), "r");	// On failure, popen set errno to indicate the error

	// read log and wait until close
	return read_log_and_close(output_stream, callback);
}


int CiscoVPN::disconnect(const CallbackOutputFunction& callback) {
	std::string command;

	// choose correct command depending on os
	#ifdef __linux__
		command = CiscoCommandGenerator::disconnect_linux();
	#elif defined(__WIN32)
		command = CiscoCommandGenerator::disconnect_windows();
	#elif defined(__APPLE__)
		command = CiscoCommandGenerator::disconnect_apple();
	#else
		This is not allowed ...
	#endif

	// run command
	FILE *output_stream = popen(command.c_str(), "r");

	// read log and wait until close
	return read_log_and_close(output_stream, callback);
}


CiscoVPN::ReturnValue CiscoVPN::status(const CallbackOutputFunction &callback) {
	std::string command;

	// choose correct command depending on os
	#ifdef __linux__
		command = CiscoCommandGenerator::status_linux();
	#elif defined(__WIN32)
		command = CiscoCommandGenerator::status_windows();
		#elif defined(__APPLE__)
		command = CiscoCommandGenerator::status_apple();
		#else
			This is not allowed ...
	#endif

	// run command
	FILE *output_stream = popen(command.c_str(), "r");

	// error handling
	if (output_stream == nullptr)
		return ReturnValue::PopenFailed;

	// read cisco output
	ReturnValue res = FailedToLoadStatus;
	char path[PATH_MAX];	// buffer to read cisco output
	while (fgets(path, PATH_MAX, output_stream) != nullptr) {
		std::string log_line = path;
		callback(log_line);
		analyse_status_log(log_line, res);
	}

	// close cisco program and stream
	const int cisco_exit_status = pclose(output_stream);	// On failure, pclose set errno to indicate the error
	if (cisco_exit_status == -1) {               // -1 means error by pclose(), so this is not cisco return type
        callback("Failed to close cisco application");
        return ReturnValue::PcloseFailed;        // if is not necessary, because PcloseFailed == -1, but it's nicer to read
    }
	// if could not extract status from log (e.g. not supported language of cisco)
	if (cisco_exit_status == 0 && res == FailedToLoadStatus) {
		callback("Failed to identify status. Read log for info");
		return ReturnValue::CouldNotAnalyseStatusLog;
	}
	// general error handling
    if (cisco_exit_status != 0 || res == FailedToLoadStatus) {
        callback("Failed to load status. Read log for more info");
        return ReturnValue::FailedToLoadStatus;
    }
    callback("Finished");
	return res;
}


int CiscoVPN::read_log_and_close(FILE* output_stream, const CallbackOutputFunction &callback) {
	// error handling
	if (output_stream == nullptr)
		return ReturnValue::PopenFailed;

	// read cisco output
	char path[PATH_MAX];	// buffer to read cisco output
	while (fgets(path, PATH_MAX, output_stream) != nullptr)
		callback(path);

	// close cisco program and stream
	const int cisco_exit_status = pclose(output_stream);	// On failure, pclose set errno to indicate the error
	if (cisco_exit_status == -1)		// -1 means error by pclose(), so this is not cisco return type
		return ReturnValue::PcloseFailed;		// if is not necessary, because PcloseFailed == -1, but it's nicer to read
	return cisco_exit_status;
}

void CiscoVPN::analyse_status_log(const std::string& log_line, CiscoVPN::ReturnValue &result) {
	// log is in language of os / cisco secure client -> language specific analysis
	if (log_line.find("state: Disconnected") != std::string::npos || log_line.find("state: Verbindung getrennt") != std::string::npos)
		result = Disconnected;
	if (result == Disconnected && (log_line.find("notice: Ready to connect") != std::string::npos || log_line.find("notice: Bereit zur Verbindung") != std::string::npos))
		result = DisconnectedReadyToConnect;
	if (log_line.find("state: Connected") != std::string::npos || log_line.find("state: Verbunden") != std::string::npos)
		result = Connected;
	// if disconnected, but no internet connection
	if (result == Disconnected && (log_line.find("No Network Connectivity") != std::string::npos || log_line.find("Keine Netzwerkverbindung") != std::string::npos))
		result = DisconnectedNoInternetConnection;
}

