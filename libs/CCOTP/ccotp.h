//
// Created by admin on 26.08.24.
//

#ifndef CCOTP_H
#define CCOTP_H

#include "otp_static.h"
//#include "otpuri_static.h"
//#include <cstdint>

namespace CCOTP {

	class OTP {
	protected:
		OTPData* data;

	public:
		OTP(OTPData* data, const char* base32_secret, COTP_ALGO algo, uint32_t digits);
		~OTP();

		COTPRESULT generate(int64_t input, char* output);
		COTPRESULT byte_secret(char* out_str);
		static COTPRESULT num_to_bytestring(uint64_t integer, char* out_str);
		size_t uri_strlen(const char* issuer, const char* name, const char* digest);
		COTPRESULT build_uri(const char* issuer, const char* name, const char* digest, char* output);
		OTPData* data_struct();
		static COTPRESULT random_base32(size_t len, char* out_str);
	};


	class TOTP {
	protected:
		OTPData* data;

	public:
		TOTP(OTPData* data, const char* base32_secret, COTP_ALGO algo, COTP_TIME time, uint32_t digits, uint32_t interval);
		~TOTP();

		COTPRESULT at(uint64_t for_time, uint64_t offset, char* out_str);
		COTPRESULT now(char* out_str);
		COTPRESULT verify(const char* key, uint64_t for_time, int64_t valid_window);
		uint64_t valid_until(uint64_t for_time, int64_t valid_window);
		uint64_t timecode(uint64_t for_time);
		size_t uri_strlen(const char* issuer, const char* name, const char* digest);
		COTPRESULT build_uri(const char* issuer, const char* name, const char* digest, char* output);
		OTPData* data_struct();
		static COTPRESULT random_base32(size_t len, char* out_str);
	};


	class HOTP {
	protected:
		OTPData* data;

	public:
		HOTP(OTPData* data, const char* base32_secret, COTP_ALGO algo, uint32_t digits, uint64_t count);
		~HOTP();

		COTPRESULT at(uint64_t counter, char* out_str);
		COTPRESULT next(char* out_str);
		COTPRESULT compare(const char* key, uint64_t counter);
		size_t uri_strlen(const char* issuer, const char* name, const char* digest);
		COTPRESULT build_uri(const char* issuer, const char* name, const char* digest, char* output);
		OTPData* data_struct();
		static COTPRESULT random_base32(size_t len, char* out_str);
	};

} // CCOTP

#endif // CCOTP_H
