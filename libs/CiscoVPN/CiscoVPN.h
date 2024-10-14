
#ifndef CiscoVPN_H
#define CiscoVPN_H

#include <string>
#include <functional>

typedef std::function<void(std::string)> CallbackOutputFunction;

class CiscoVPN {
public:
	/* This enum describes the exit code of the cisco programm and the connection between this program and
	 * cisco (e.g. popen()). Additionally there is the ProgressBarResultValue enum, which analyses the cisco
	 * log output.
	 * */
	enum ReturnValue {					// negative is usually for self defined exit code
		CouldNotAnalyseStatusLog = -9,	// -> so this is custom and could conflict with real cisco exit code ...
		FailedToLoadStatus = -8,
		DisconnectedNoInternetConnection = -7,
		DisconnectedReadyToConnect = -6,
		Disconnected = -5,
		Connected = -4,
		InvalidSeed = -3,
		PopenFailed = -2,
		PcloseFailed = -1,			// see popen manual for choose of -1
		Success = 0,				// real cisco exit code
		WrongLoginData = 65280,		// real cisco exit code for wrong login data, but maybe also for more
		WrongLoginDataLinux = 255,	// real cisco exit code for wrong login data, but maybe also for more
		CiscoUpdate = 9,			// real cisco exit code after aborting because update was performed ()
	};

	/* This enum exists to indicate the success of the connection setup with the Uni VPN.
	 * There is only a success, if the progress bar was set to the maximum value (100). Otherwise, the value describes
	 * the last processing point of the Cisco program, which is read out via the log. If Cisco could not complete this
	 * processing point, this means that Cisco failed at this point. This is the reason, why we can use this value as
	 * indicator.
	 * */
	enum class ProgressBarResultValue : int {
		NotEvenStarted = 0,
		CannotContactHost = 10,
		WrongUniIdOrPassword = 20,
		WrongOTPCode = 30,
		CannotAccept = 50,
		CannotAcceptWithY = 53,
		ExitingCauseUpdateInProgress = 77,
		Success = 100
	};

private:
	static int read_log_and_close(FILE* output_stream, const CallbackOutputFunction& callback);
	static void analyse_status_log(const std::string& log_line, ReturnValue& result);

public:
	static int connect(const std::string& uni_id, const std::string& uni_password, const std::string& seed_2fa, const CallbackOutputFunction& callback);
	static int disconnect(const CallbackOutputFunction& callback);
	static ReturnValue status(const CallbackOutputFunction& callback);
};


#endif //CiscoVPN_H
