//
// Created by admin on 26.08.24.
//

#include "otp_static.h"

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <openssl/rand.h>
#include <cstdio>

namespace CCOTP {

	// Converts an OTPType enum to string.
	const char* otp_static::OTPType_asString(OTPType type) {
		switch (type) {
			case OTP: return "OTP";
			case TOTP: return "TOTP";
			case HOTP: return "HOTP";
		}
		return nullptr;
	}


	// Initializes an OTPData structure.
	OTPData* otp_static::otp_new(OTPData* data, const char* base32_secret, COTP_ALGO algo, uint32_t digits) {
		data->digits = digits ? digits : 6;
		data->interval = 0;
		data->count = 0;

		data->method = OTP;
		data->algo = algo;
		data->time = nullptr;

		data->base32_secret = &base32_secret[0];

		return data;
	}


	// Initializes an OTPData structure. Extends off of otp_new.
	OTPData* otp_static::totp_new(OTPData* data, const char* base32_secret, COTP_ALGO algo, COTP_TIME time, uint32_t digits, uint32_t interval) {
		OTPData* tdata = otp_new(data, base32_secret, algo, digits);
		tdata->interval = interval;
		tdata->time = time;
		tdata->method = TOTP;

		return data;
	}


	// Initializes an OTPData structure.
	OTPData* otp_static::hotp_new(OTPData* data, const char* base32_secret, COTP_ALGO algo, uint32_t digits, uint64_t count) {
		OTPData* hdata = otp_new(data, base32_secret, algo, digits);
		hdata->method = HOTP;
		hdata->count = count;

		return data;
	}


	// Semantic convenience method.
	void otp_static::otp_free(OTPData* data) {
		std::free(data);
	}


	// Un-base32's a base32 string stored inside an OTPData.
	COTPRESULT otp_static::otp_byte_secret(OTPData* data, char* out_str) {
		if (out_str == nullptr || std::strlen(data->base32_secret) % 8 != 0)
			return OTP_ERROR;

		size_t base32_length = std::strlen(data->base32_secret);
		size_t num_blocks = base32_length / 8;
		size_t output_length = num_blocks * 5;

		if (output_length == 0)
			return OTP_OK;

		for (size_t i = 0; i < num_blocks; i++) {
			unsigned int block_values[8] = { 0 };

			for (int j = 0; j < 8; j++) {
				char c = data->base32_secret[i * 8 + j];
				if (c == '=')
					break;

				int found = 0;
				for (int k = 0; k < 32; k++) {
					if (c == OTP_DEFAULT_BASE32_CHARS[k]) {
						block_values[j] = k;
						found = 1;
						break;
					}
				}

				if (!found) {
					return OTP_ERROR;
				}
			}

			out_str[i * 5] = (block_values[0] << 3) | (block_values[1] >> 2);
			out_str[i * 5 + 1] = (block_values[1] << 6) | (block_values[2] << 1) | (block_values[3] >> 4);
			out_str[i * 5 + 2] = (block_values[3] << 4) | (block_values[4] >> 1);
			out_str[i * 5 + 3] = (block_values[4] << 7) | (block_values[5] << 2) | (block_values[6] >> 3);
			out_str[i * 5 + 4] = (block_values[6] << 5) | block_values[7];
		}

		return OTP_OK;
	}


	// Converts an integer into an 8-byte array.
	COTPRESULT otp_static::otp_num_to_bytestring(uint64_t integer, char* out_str) {
		if (out_str == nullptr)
			return OTP_ERROR;

		size_t i = 7;
		while (integer != 0) {
			out_str[i] = integer & 0xFF;
			i--;
			integer >>= 8;
		}

		return OTP_OK;
	}


	// Generates a valid secured random base32 string.
	COTPRESULT otp_static::otp_random_base32(size_t len, char* out_str) {
		if (out_str == nullptr)
			return OTP_ERROR;

		len = len > 0 ? len : 16;

		unsigned char rand_buffer[len];
		if (RAND_bytes(rand_buffer, len) != 1)
			return OTP_ERROR;

		for (size_t i = 0; i < len; i++) {
			out_str[i] = OTP_DEFAULT_BASE32_CHARS[rand_buffer[i] % 32];
		}

		return OTP_OK;
	}


	// Compares a key against a generated key for a single specific timeblock.
	COTPRESULT otp_static::totp_compare(OTPData* data, const char* key, int64_t offset, uint64_t for_time) {
		char time_str[data->digits + 1];
		std::memset(time_str, 0, data->digits + 1);

		if (totp_at(data, for_time, offset, time_str) == 0)
			return OTP_ERROR;

		for (size_t i = 0; i < data->digits; i++) {
			if (key[i] != time_str[i])
				return OTP_ERROR;
		}

		return OTP_OK;
	}


	// Generates an OTP key using the totp algorithm.
	COTPRESULT otp_static::totp_at(OTPData* data, uint64_t for_time, int64_t offset, char* out_str) {
		return otp_generate(data, totp_timecode(data, for_time) + offset, out_str);
	}


	// Generates an OTP key using the totp algorithm with the current, unsecure time in seconds.
	COTPRESULT otp_static::totp_now(OTPData* data, char* out_str) {
		return otp_generate(data, totp_timecode(data, data->time()), out_str);
	}


	// Compares a key against a generated key for multiple timeblocks before and after a specific time.
	COTPRESULT otp_static::totp_verify(OTPData* data, const char* key, uint64_t for_time, int64_t valid_window) {
		if (key == nullptr || valid_window < 0)
			return OTP_ERROR;

		if (valid_window > 0) {
			for (int64_t i = -valid_window; i < valid_window + 1; i++) {
				int cmp = totp_compare(data, key, i, for_time);
				if (cmp == 1)
					return cmp;
			}
			return OTP_ERROR;
		}

		return totp_compare(data, key, 0, for_time);
	}


	// Calculate the time in seconds relative to for_time an OTP is valid for.
	uint64_t otp_static::totp_valid_until(OTPData* data, uint64_t for_time, int64_t valid_window) {
		return for_time + (data->interval * valid_window);
	}


	// Generates the timeblock for a time in seconds.
	uint64_t otp_static::totp_timecode(OTPData* data, uint64_t for_time) {
		if (data->interval <= 0)
			return OTP_ERROR;

		return for_time / data->interval;
	}


	// Compares a key against a generated key for a single counter.
	int otp_static::hotp_compare(OTPData* data, const char* key, uint64_t counter) {
		if (key == nullptr)
			return OTP_ERROR;

		char cnt_str[data->digits + 1];
		std::memset(cnt_str, 0, data->digits + 1);

		if (hotp_at(data, counter, cnt_str) == 0)
			return OTP_ERROR;

		for (size_t i = 0; i < data->digits; i++) {
			if (key[i] != cnt_str[i])
				return OTP_ERROR;
		}

		return OTP_OK;
	}


	// Generates an OTP key using the hotp algorithm.
	int otp_static::hotp_at(OTPData* data, uint64_t counter, char* out_str) {
		return otp_generate(data, counter, out_str);
	}


	// Generates an OTP key using the hotp algorithm and advances the counter.
	int otp_static::hotp_next(OTPData* data, char* out_str) {
		return otp_generate(data, data->count++, out_str);
	}


	// Generates an OTP (One Time Password).
	COTPRESULT otp_static::otp_generate(OTPData* data, uint64_t input, char* out_str) {
		if (out_str == nullptr)
			return OTP_ERROR;

		char byte_string[8 + 1];
		std::memset(byte_string, 0, 8 + 1);

		size_t bs_len = (std::strlen(data->base32_secret) / 8) * 5;
		char byte_secret[bs_len + 1];
		std::memset(byte_secret, 0, bs_len + 1);

		char hmac[64 + 1];
		std::memset(hmac, 0, 64 + 1);

		if (otp_num_to_bytestring(input, byte_string) == 0 || otp_byte_secret(data, byte_secret) == 0)
			return OTP_ERROR;

		int hmac_len = (*(data->algo))(byte_secret, bs_len, byte_string, hmac);
		if (hmac_len == 0)
			return OTP_ERROR;

		unsigned int offset = hmac[hmac_len - 1] & 0x0F;
		uint32_t code = (
				((hmac[offset] & 0x7F) << 24) |
				((hmac[offset + 1] & 0xFF) << 16) |
				((hmac[offset + 2] & 0xFF) << 8) |
				(hmac[offset + 3] & 0xFF)
		);

		std::memset(out_str, 0, sizeof(out_str));
		std::sprintf(out_str, "%0*u", data->digits, (code % (unsigned int)(std::pow(10, data->digits))));

		return OTP_OK;
	}


} // CCOTP