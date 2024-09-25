//
// Created by admin on 31.08.24.
//

#ifndef AUTOCONNECTUNIVPN_ENCDECEXCEPTIONS_H
#define AUTOCONNECTUNIVPN_ENCDECEXCEPTIONS_H


#include <exception>
#include <string>
#include <utility>

class DecryptionException : public std::exception {
private:
	std::string message;
public:
	explicit DecryptionException(std::string  msg) : message(std::move(msg)) {}

	[[nodiscard]] const char* what() const noexcept override {
		return message.c_str();
	}
};

class EncryptionException : public std::exception {
private:
	std::string message;
public:
	explicit EncryptionException(std::string  msg) : message(std::move(msg)) {}

	[[nodiscard]] const char* what() const noexcept override {
		return message.c_str();
	}
};


#endif //AUTOCONNECTUNIVPN_ENCDECEXCEPTIONS_H
