
#ifndef TOTP_EXCEPTIONS_H
#define TOTP_EXCEPTIONS_H


#include <exception>
#include <string>


class invalid_hex_character : public std::exception {
private:
	std::string message;
public:
	explicit invalid_hex_character(std::string  msg) : message(std::move(msg)) {}

	[[nodiscard]] const char* what() const noexcept override {
		return message.c_str();
	}
};

#endif //TOTP_EXCEPTIONS_H
