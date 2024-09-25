//
// Created by admin on 26.08.24.
//

#ifndef OTP_STATIC_H
#define OTP_STATIC_H

#include <cstdlib>
#include <cstdint>

namespace CCOTP {

	// OTPRESULT can either be 1 (success) or 0 (error)
	typedef int COTPRESULT;

	#define OTP_OK		((COTPRESULT) 1)
	#define OTP_ERROR	((COTPRESULT) 0)

	/*
	Used for differentiation on which
	method you are using. Necessary
	when you go to generate a URI.
	*/
	typedef enum OTPType
	{
		OTP, TOTP, HOTP
	} OTPType;

	/*
	Must compute HMAC using passed arguments,
	output as char array through output.

	key is base32 secret key.
	input is input number as string.
	output is an output char buffer of the resulting HMAC operation.

	Must return 0 if error, or the length in bytes of the HMAC operation.
	*/
	typedef int (*COTP_ALGO)(const char* key, int key_length, const char* input, char* output);

	/*
	Must return the current time in seconds.
	*/
	typedef uint64_t (*COTP_TIME)();

	/*
	Holds data for use by the cotp module.

	If you know what you are doing,
	feel free to initialize this yourself.
	*/
	typedef struct OTPData
	{
		uint32_t digits;
		uint32_t interval; // TOTP exclusive
		uint64_t count;

		OTPType method;
		COTP_ALGO algo;
		COTP_TIME time;

		const char* base32_secret;
	} OTPData;


	class otp_static {
	private:
		// Todo: Really privat?
		static const char* OTPType_asString(OTPType type);

	public:
		/*
		Default characters used in BASE32 digests.
		For use with otp_random_base32()
		*/
		constexpr static const char OTP_DEFAULT_BASE32_CHARS[32] =
				{
						'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
						'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
						'U', 'V', 'W', 'X', 'Y', 'Z', '2', '3', '4', '5',
						'6', '7'
				};

		/*
		Struct initialization functions
		*/
		static OTPData* otp_new(OTPData* data, const char* base32_secret, COTP_ALGO algo, uint32_t digits);
		static OTPData* totp_new(OTPData* data, const char* base32_secret, COTP_ALGO algo, COTP_TIME time, uint32_t digits, uint32_t interval);
		static OTPData* hotp_new(OTPData* data, const char* base32_secret, COTP_ALGO algo, uint32_t digits, uint64_t count);

		/*
		OTP free function
		*/
		static void otp_free(OTPData* data);

		/*
		OTP functions
		*/
		static COTPRESULT otp_generate(OTPData* data, uint64_t input, char* out_str);
		static COTPRESULT otp_byte_secret(OTPData* data, char* out_str);
		static COTPRESULT otp_num_to_bytestring(uint64_t integer, char* out_str);
		static COTPRESULT otp_random_base32(size_t len, char* out_str);

		/*
		TOTP functions
		*/
		static COTPRESULT totp_compare(OTPData* data, const char* key, int64_t offset, uint64_t for_time);
		static COTPRESULT totp_at(OTPData* data, uint64_t for_time, int64_t offset, char* out_str);
		static COTPRESULT totp_now(OTPData* data, char* out_str);
		static COTPRESULT totp_verify(OTPData* data, const char* key, uint64_t for_time, int64_t valid_window);
		static uint64_t totp_valid_until(OTPData* data, uint64_t for_time, int64_t valid_window);
		static uint64_t totp_timecode(OTPData* data, uint64_t for_time);

		/*
		HOTP functions
		*/
		static COTPRESULT hotp_compare(OTPData* data, const char* key, uint64_t counter);
		static COTPRESULT hotp_at(OTPData* data, uint64_t counter, char* out_str);
		static COTPRESULT hotp_next(OTPData* data, char* out_str);
	};

} // CCOTP

#endif //OTP_STATIC_H
