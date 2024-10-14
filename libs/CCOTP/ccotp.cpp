//
// Created by admin on 26.08.24.
//

#include "ccotp.h"

#include "otpuri_static.h"

namespace CCOTP {
	/* OTP Definition */
	OTP::OTP(OTPData *data, const char *base32_secret, COTP_ALGO algo, uint32_t digits) {
		this->data = otp_static::otp_new(data, base32_secret, algo, digits);
	}

	OTP::~OTP() {
		data = nullptr;
	}

	CCOTP::COTPRESULT OTP::generate(int64_t input, char *output) {
		return otp_static::otp_generate(data, input, output);
	}

	COTPRESULT OTP::byte_secret(char *out_str) {
		return otp_static::otp_byte_secret(data, out_str);
	}

	COTPRESULT OTP::num_to_bytestring(uint64_t integer, char *out_str) {
		return otp_static::otp_num_to_bytestring(integer, out_str);
	}

	size_t OTP::uri_strlen(const char *issuer, const char *name, const char *digest) {
		return otpuri_static::otpuri_strlen(data, issuer, name, digest);
	}

	COTPRESULT OTP::build_uri(const char *issuer, const char *name, const char *digest, char *output) {
		return otpuri_static::otpuri_build_uri(data, issuer, name, digest, output);
	}

	OTPData *OTP::data_struct() {
		return data;
	}

	COTPRESULT OTP::random_base32(size_t len, char *out_str) {
		return otp_static::otp_random_base32(len, out_str);
	}


	/* TOTP Definition */
	TOTP::TOTP(OTPData *data, const char *base32_secret, COTP_ALGO algo, COTP_TIME time, uint32_t digits,
			   uint32_t interval) {
		this->data = otp_static::totp_new(data, base32_secret, algo, time, digits, interval);
	}

	TOTP::~TOTP() {
		data = nullptr;
	}

	COTPRESULT TOTP::at(uint64_t for_time, uint64_t offset, char *out_str) {
		return otp_static::totp_at(data, for_time, offset, out_str);
	}

	COTPRESULT TOTP::now(char *out_str) {
		return otp_static::totp_now(data, out_str);
	}

	COTPRESULT TOTP::verify(const char *key, uint64_t for_time, int64_t valid_window) {
		return otp_static::totp_verify(data, key, for_time, valid_window);
	}

	uint64_t TOTP::valid_until(uint64_t for_time, int64_t valid_window) {
		return otp_static::totp_valid_until(data, for_time, valid_window);
	}

	uint64_t TOTP::timecode(uint64_t for_time) {
		return otp_static::totp_timecode(data, for_time);
	}

	size_t TOTP::uri_strlen(const char *issuer, const char *name, const char *digest) {
		return otpuri_static::otpuri_strlen(data, issuer, name, digest);
	}

	COTPRESULT TOTP::build_uri(const char *issuer, const char *name, const char *digest, char *output) {
		return otpuri_static::otpuri_build_uri(data, issuer, name, digest, output);
	}

	OTPData *TOTP::data_struct() {
		return data;
	}

	COTPRESULT TOTP::random_base32(size_t len, char *out_str) {
		return otp_static::otp_random_base32(len, out_str);
	}


	/* HOTP Definitions */
	HOTP::HOTP(OTPData *data, const char *base32_secret, COTP_ALGO algo, uint32_t digits, uint64_t count) {
		this->data = otp_static::hotp_new(data, base32_secret, algo, digits, count);
	}

	HOTP::~HOTP() {
		data = nullptr;
	}

	COTPRESULT HOTP::at(uint64_t counter, char *out_str) {
		return otp_static::hotp_at(data, counter, out_str);
	}

	COTPRESULT HOTP::next(char *out_str) {
		return otp_static::hotp_next(data, out_str);
	}

	COTPRESULT HOTP::compare(const char *key, uint64_t counter) {
		return otp_static::hotp_compare(data, key, counter);
	}

	size_t HOTP::uri_strlen(const char *issuer, const char *name, const char *digest) {
		return otpuri_static::otpuri_strlen(data, issuer, name, digest);
	}

	COTPRESULT HOTP::build_uri(const char *issuer, const char *name, const char *digest, char *output) {
		return otpuri_static::otpuri_build_uri(data, issuer, name, digest, output);
	}

	OTPData *HOTP::data_struct() {
		return data;
	}

	COTPRESULT HOTP::random_base32(size_t len, char *out_str) {
		return otp_static::otp_random_base32(len, out_str);
	}
} // CCOTP

