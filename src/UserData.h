
#ifndef USERDATA_H
#define USERDATA_H


#include <string>
#include <filesystem>
#include <vector>
#include "../ui_mainwindow.h"

class UserData {
public:
	inline static std::string uni_id;
	inline static std::string uni_password;
	inline static std::string seed2fa;

private:
	inline const static std::string CONFIG_FOLDER_NAME = "AutoConnectUniVPN";
	static std::vector<unsigned char> encode(const std::string &encrypt_password, const std::vector<unsigned char>& iv, bool with_uni_password);
	static std::string decode(const std::vector<unsigned char>& ciphertext, const std::string& password,
							  const std::vector<unsigned char>& iv);
	static std::filesystem::path config_folder();
	static std::filesystem::path config_file();
	static bool load_into_variables(const std::string& user_data_string, bool password_is_in_string = true);
    static bool delete_config_file();
    static bool delete_user_data_variables();

public:
	static bool exists_configfile();
	static bool is_password_stored_in_config_file();
	static bool load_from_ui(const Ui::MainWindow* ui);
	static bool get_uni_password_form_ui(const Ui::MainWindow* ui);
	static bool load_from_file(const std::string& encrypt_password, bool uni_password_is_in_file = false);
	static bool store_to_file(const std::string& encrypt_password, bool with_uni_password = true);
    static bool delete_all();

//	enum class LoadFromUIError {
//		UnknownFail,	// Todo: Do we need this?
//		NoId,
//		NoPassword,
//		NoSeed
//	};
};


#endif //USERDATA_H
