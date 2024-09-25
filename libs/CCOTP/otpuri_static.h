//
// Created by admin on 26.08.24.
//

#ifndef STATIC_H
#define STATIC_H

#include "ccotp.h"

namespace CCOTP {

	class otpuri_static {
	public:
		/*
		Description:
			Returns the maximum expected length of an array needed to fill
			a buffer with an otpuri not including the null-termination.
		Returns:
			Length in bytes of an array to match an otpuri generation
		*/
		static size_t otpuri_strlen(OTPData* data, const char* issuer, const char* name, const char* digest);

		/*
		Description:
			Encodes all given data into url-safe data. Null-terminates
			returned string. Caller must free the returned pointer.
			Will treat embedded \0's as valid characters.
		Parameter:
			data is the optionally null-terminated string to encode
			length is the length in bytes of input string data
		Returns:
			Pointer to malloc'd url-safe data string
			error, 0
		*/
		static COTPRESULT otpuri_encode_url(const char* data, size_t length, char* output);

		/*
		Description:
			Builds a valid, url-safe URI which is used for applications such as QR codes.
		Parameter:
			issuer is the null-terminated string of the company name
			name is the null-terminated string of the username
			digest is the null-terminated string of the HMAC encryption algorithm
			output is the zero'd destination the function writes the URI to
		Returns:
			1 on success
			error, 0
		*/
		static COTPRESULT otpuri_build_uri(OTPData* data, const char* issuer, const char* name, const char* digest, char* output);
	};

} // CCOTP

#endif // OTPURI_STATIC_H
