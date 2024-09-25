
#include "UserData.h"
#include "../libs/Encoder/EncDec.h"
#include "../libs/Encoder/EncDecExceptions.h"

#ifdef __linux__
	#include <unistd.h>
	#include <pwd.h>
#endif

#include <openssl/evp.h>
#include <cstring>
#include <fstream>
#include <iostream>
#ifdef __WIN32
    #include <sstream>
#endif

std::vector<unsigned char> UserData::encode(const std::string &encrypt_password, const std::vector<unsigned char>& iv, bool with_uni_password) {
	// Todo: check if member vars are not empty?

	// Todo: make this larger
	std::vector<unsigned char> ciphertext(128);
	std::vector<unsigned char> key(256);
	EncDec::pbkdf_256_bit(encrypt_password, key);

	/* Message to be encrypted */
	std::string plaintext_string = uni_id + (with_uni_password ? "\n" + uni_password : "") + "\n" + seed2fa;
	std::vector<unsigned char> plaintext_vec(plaintext_string.begin(), plaintext_string.end());
	unsigned char* plaintext = plaintext_vec.data();

	/* Encrypt the plaintext */
	int ciphertext_len = EncDec::encrypt (plaintext, strlen ((char *)plaintext), key.data(), iv.data(), ciphertext.data());
	ciphertext.resize(ciphertext_len);

	return ciphertext;
}

std::string UserData::decode(const std::vector<unsigned char>& ciphertext, const std::string& password, const std::vector<unsigned char>& iv) {
	// generate key from password
	std::vector<unsigned char> key;
	EncDec::pbkdf_256_bit(password, key);

	/* Decrypt the ciphertext */
	std::vector<unsigned char> decrypted_text(128);
	int decrypted_text_len = EncDec::decrypt(ciphertext.data(), ciphertext.size(), key.data(),
											 iv.data(), decrypted_text.data());

	/* Add a NULL terminator. We are expecting printable text */
	decrypted_text[decrypted_text_len] = '\0';
	return (const char *)decrypted_text.data();
}


bool UserData::load_from_ui(const Ui::MainWindow* ui) {
	uni_id = ui->uniIDLineEdit->text().toStdString();
	uni_password = ui->uniPasswordLineEdit->text().toStdString();
	seed2fa = ui->uniSeedLineEdit->text().remove("seed://").toStdString();
	// Todo: Also check, if uni id is ab123 and seed is hex?
	return !(uni_id.empty() || uni_password.empty() || seed2fa.empty());
}


bool UserData::get_uni_password_form_ui(const Ui::MainWindow *ui) {
	uni_password = ui->uniPasswordLineEdit->text().toStdString();
	return ! uni_password.empty();
}


bool UserData::load_from_file(const std::string& encrypt_password, bool uni_password_is_in_file) {
	if (std::ifstream input_file(config_file(), std::ios::binary); input_file.is_open()) {
		// detect char array size
		input_file.seekg(0, std::ios::end);
		long dataSize = input_file.tellg();
		input_file.seekg(0, std::ios::beg);

		std::vector<unsigned char> data(dataSize);
		input_file.read((char*)(data.data()), dataSize);
		input_file.close();

		// extract iv from data (first 128 bit)
		std::vector<unsigned char> iv(data.begin(), data.begin() + 16);

		// extract ciphertext (everything except iv)
		std::vector<unsigned char> ciphertext(data.begin() + 16, data.end());

		// decode
		std::string plaintext;
		try { plaintext = UserData::decode(ciphertext, encrypt_password, iv); }
		catch (const DecryptionException&) { return false; }

		// store to member variables
		if (! uni_password_is_in_file)		// Todo: Is this clean code?
			UserData::uni_password = encrypt_password;
		if (load_into_variables(plaintext, uni_password_is_in_file))
			return true;
		return false;
	}
	return false;
}



bool UserData::store_to_file(const std::string& encrypt_password, bool with_uni_password) {
	/* A 128 bit IV */
	std::vector<unsigned char> iv;
	EncDec::random_iv_128_bit(iv);

	// encrypt user data
	std::vector<unsigned char> ciphertext;
	try { ciphertext = UserData::encode(encrypt_password, iv, with_uni_password); }
	catch (const EncryptionException&) { return false; }

	// merge iv and ciphertext, to store it together
	iv.insert(iv.end(), ciphertext.begin(), ciphertext.end());

	// store to file
	if (std::ofstream file(config_file(), std::ios::binary); file.is_open()) {
		file.write((const char*)(iv.data()), iv.size());
		file.close();
		return true;
	}
	return false;
}

/* Returns path to programs data folder and create this folder in not exists */
std::filesystem::path UserData::config_folder() {
	std::filesystem::path path;

	// get path to programs data folder depending on os
#ifdef __linux__
	const char* home_dir = getenv("HOME");
	if (home_dir == nullptr)
		home_dir = getpwuid(getuid())->pw_dir;
	path = home_dir;
	path.append(".config").append(CONFIG_FOLDER_NAME);
#elif defined(__WIN32)
	const char* local_appdata = getenv("LOCALAPPDATA");
	if (local_appdata == nullptr)
		throw std::runtime_error("No environment variable LOCALAPPDATA");
	path = local_appdata;
	path.append(CONFIG_FOLDER_NAME);
#else
	Not supported
#endif
	// create dir if not exists and throw exception if not able to create dir
	if (!std::filesystem::exists(path) && !std::filesystem::create_directories(path))
		throw std::exception();

	return path;
}

std::filesystem::path UserData::config_file() {
	return config_folder().append("user_data.bin");
}

bool UserData::load_into_variables(const std::string &user_data_string, bool password_is_in_string) {
    std::istringstream iss(user_data_string);
    std::string line;

    // read lines and store them to member variables
    int row_counter = 0;
    while (std::getline(iss, line)) {
        switch (row_counter) {
            case 0:
                UserData::uni_id = line;
                break;
            case 1:
				password_is_in_string ? UserData::uni_password = line : UserData::seed2fa = line;
                break;
            case 2:
                UserData::seed2fa = line;
                break;
            default:	// too much data (there must be only 3 lines)
                return false;
        }
        ++row_counter;
    }

    // check if all data was loaded
    if (password_is_in_string && row_counter < 3 || ! password_is_in_string && row_counter >= 3)
		return false;
	return true;
}

bool UserData::exists_configfile() {
	return std::filesystem::exists(config_file());
}

bool UserData::is_password_stored_in_config_file() {
	if (! exists_configfile())
		return false;
	const uintmax_t FILESIZE_WITHOUT_PASSWORD = 64;
	return std::filesystem::file_size(config_file()) > FILESIZE_WITHOUT_PASSWORD;
}

bool UserData::delete_all() {
    return delete_config_file() && delete_user_data_variables();
}

bool UserData::delete_config_file() {
    if (! exists_configfile())
        return true;
    return std::filesystem::remove(config_file());
}

bool UserData::delete_user_data_variables() {
    // idea: override data -> is there a better way to do it?
    uni_id.assign(uni_id.size(), 'a');
    uni_password.assign(uni_password.size(), 'a');
    seed2fa.assign(seed2fa.size(), 'a');
    uni_id.clear();
    uni_password.clear();
    seed2fa.clear();
    return true;
}

