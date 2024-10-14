//
// Created by admin on 15.08.24.
//

#ifndef TOTP_H
#define TOTP_H

#include <string>

std::string totp_code(const std::string& secret_as_hex);

#endif //TOTP_H
