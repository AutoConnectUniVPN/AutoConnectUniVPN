#include "totp.h"
#include "ccotp.h"
#include "totp_exceptions.h"

#include <cstdlib>
#include <cstring>
#include <chrono>
#include <bitset>
#include <stdexcept>

extern "C"
{
#include <openssl/evp.h>
#include <openssl/hmac.h>
}

using namespace CCOTP;

static const int32_t SHA1_BYTES   = 160 / 8;	// 20


// byte_secret is unbase32 key
// byte_string is data to be HMAC'd
// returns 0 for failure otherwise the length of the string
int hmac_algo_sha1(const char* byte_secret, int key_length, const char* byte_string, char* out)
{
    // Output len
    unsigned int len = SHA1_BYTES;

    unsigned char const* result = HMAC(
            EVP_sha1(),										// algorithm
            (unsigned char*)byte_secret, key_length,	// key
            (unsigned char*)byte_string, 8,			// data
            (unsigned char*)out,								// output
            &len												// output length
    );

    // Return the HMAC success
    return result == nullptr ? 0 : len;
}


uint64_t get_current_time() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto dur = now.time_since_epoch();
    return duration_cast<std::chrono::seconds>(dur).count();
}


std::string binary_to_base32(const std::string& binary_string) {
    std::string base_32_string;
    const char* base32Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

    int binary_length = binary_string.length();
    int base_32_length = (binary_length + 4) / 5; // calculate base32 length

    for (int i = 0; i < base_32_length; ++i) {
        int byte = 0;
        for (int j = 0; j < 5; ++j) {
            int k = i * 5 + j;
            if (k < binary_length) {
                byte |= (binary_string[k] - '0') << (4 - j);
            }
        }
		base_32_string += base32Alphabet[byte % 32];
        byte /= 32;
    }

    // pad with '=' if necessary
    int padding = (5 - binary_length % 5) % 5;
    for (int i = 0; i < padding; ++i) {
		base_32_string += '=';
    }

    return base_32_string;
}

std::string hex_to_binary(const std::string& hex_string) {
    std::string binary_string;
    for (char c : hex_string) {
        int value = 0;
        if (c >= '0' && c <= '9') {
            value = c - '0';
        } else if (c >= 'A' && c <= 'F') {
            value = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            value = c - 'a' + 10;
        } else {
            throw invalid_hex_character("Converting to base 32 failed. '" + hex_string + "' contains invalid hex character.");
        }
        binary_string += std::bitset<4>(value).to_string();
    }
    return binary_string;
}


std::string hex_to_base32(const std::string& hex_string) {
    std::string binary_string = hex_to_binary(hex_string);
    std::string base32_string = binary_to_base32(binary_string);
    return base32_string;
}




std::string totp_code(const std::string& secret_as_hex) {
    const int DIGITS = 6;
    const int INTERVAL = 30;

    // data struct with properties of the totp code generation
    OTPData empty_otp_data;
    memset(&empty_otp_data, 0, sizeof(OTPData));

    std::string secret_base_32 = hex_to_base32(secret_as_hex);

    // totp object, to generate and verify codes
    class TOTP totp(&empty_otp_data, secret_base_32.c_str(), hmac_algo_sha1, get_current_time, DIGITS, INTERVAL);

    // result variable to store code
    char totp_code[DIGITS+1];
    memset(totp_code, 0, DIGITS+1);

    // compute code
    int result_msg = totp.now(totp_code);

    // check result
    if(result_msg == OTP_ERROR) {
        return "";
    }
    return totp_code;
}
