//
// Created by admin on 26.08.24.
//

#include "otpuri_static.h"

#include <cstring>
#include <cinttypes>
#include <cstdio>

namespace CCOTP {
	size_t otpuri_static::otpuri_strlen(OTPData *data, const char *issuer, const char *name, const char *digest) {
		return std::strlen(issuer) * 2 * 3
			   + std::strlen(name) * 3
			   + std::strlen(data->base32_secret) * 3
			   + std::strlen(digest) * 3
			   + 100;
	}

	COTPRESULT otpuri_static::otpuri_encode_url(const char *data, size_t length, char *output) {
		if (data == nullptr || output == nullptr)
			return OTP_ERROR;

		static const char to_test[] = "\"<>#%@{}|\\^~[]` ?&";

		size_t output_i = 0;
		for (size_t i = 0; i < length; i++) {
			output[output_i] = data[i];
			if (data[i] < 0x20 || data[i] >= 0x7F) {
				output_i += snprintf(output + output_i, 4, "%%%.2X", static_cast<unsigned char>(data[i]));
				output_i--;
			} else {
				for (size_t j = 0; j < 18; j++) {
					if (to_test[j] == data[i]) {
						output_i += snprintf(output + output_i, 4, "%%%.2X", static_cast<unsigned char>(data[i]));
						output_i--;
						break;
					}
				}
			}
			output_i++;
		}

		return OTP_OK;
	}

	COTPRESULT otpuri_static::otpuri_build_uri(OTPData *data, const char *issuer, const char *name, const char *digest,
											   char *output) {
		if (issuer == nullptr || name == nullptr || digest == nullptr || output == nullptr)
			return OTP_ERROR;

		std::strcat(output, "otpuri://");
		switch (data->method) {
			case TOTP:
				std::strcat(output, "totp");
				break;
			case HOTP:
				std::strcat(output, "hotp");
				break;
			default:
				std::strcat(output, "otp");
				break;
		}

		std::strcat(output, "/");

		char cissuer[std::strlen(issuer) * 3 + 1];
		std::memset(cissuer, 0, std::strlen(issuer) * 3 + 1);
		otpuri_encode_url(issuer, std::strlen(issuer), cissuer);
		std::strcat(output, cissuer);

		std::strcat(output, ":");

		char cname[std::strlen(name) * 3 + 1];
		std::memset(cname, 0, std::strlen(name) * 3 + 1);
		otpuri_encode_url(name, std::strlen(name), cname);
		std::strcat(output, cname);

		std::strcat(output, "?secret=");
		char csecret[std::strlen(data->base32_secret) * 3 + 1];
		std::memset(csecret, 0, std::strlen(data->base32_secret) * 3 + 1);
		otpuri_encode_url(data->base32_secret, std::strlen(data->base32_secret), csecret);
		std::strcat(output, csecret);

		std::strcat(output, "&issuer=");
		std::strcat(output, cissuer);

		std::strcat(output, "&algorithm=");
		char cdigest[std::strlen(digest) * 3 + 1];
		std::memset(cdigest, 0, std::strlen(digest) * 3 + 1);
		otpuri_encode_url(digest, std::strlen(digest), cdigest);
		std::strcat(output, cdigest);

		std::strcat(output, "&digits=");
		char cdigits[21];
		std::memset(cdigits, 0, 21);
		std::snprintf(cdigits, 21, "%" PRIu32, data->digits);
		std::strcat(output, cdigits);

		switch (data->method)
		{
			case TOTP:
				std::strcat(output, "&period=");
				char cperiod[21];
				std::memset(cperiod, 0, 21);
				std::snprintf(cperiod, 21, "%" PRIu32, data->interval);
				std::strcat(output, cperiod);
				break;
			case HOTP:
				std::strcat(output, "&counter=");
				char ccounter[21];
				std::memset(ccounter, 0, 21);
				std::snprintf(ccounter, 21, "%" PRIu64, data->count);
				std::strcat(output, ccounter);
				break;
			default:
				break;
		}

		return OTP_OK;
	}
} // CCOTP