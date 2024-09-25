
#include "EncDec.h"
#include "EncDecExceptions.h"

// tells the compiler, that this has to be read as C compiled code
extern "C" {
	#include <openssl/evp.h>
	#include <openssl/kdf.h>
	#include <openssl/params.h>
	#include "openssl/err.h"
	#include "openssl/ssl3.h"
	#include "openssl/rand.h"
}

/* Is e.g. called if cipher_text_length wrong */
[[noreturn]] void handleDecryptionErrors()
{
	const char* err_str = ERR_error_string(ERR_get_error(), nullptr);
	throw DecryptionException(err_str);
}

[[noreturn]] void handleEncryptionErrors()
{
	const char* err_str = ERR_error_string(ERR_get_error(), nullptr);
	throw EncryptionException(err_str);
}


int EncDec::encrypt(const unsigned char *plaintext, int plaintext_len, const unsigned char *key, const unsigned char *iv,
					unsigned char *ciphertext) {
	EVP_CIPHER_CTX *ctx;

	int len;

	int ciphertext_len;

	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new()))
		handleEncryptionErrors();

	/*
	 * Initialise the encryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits
	 */
	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv))
		handleEncryptionErrors();

	/*
	 * Provide the message to be encrypted, and obtain the encrypted output.
	 * EVP_EncryptUpdate can be called multiple times if necessary
	 */
	if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		handleEncryptionErrors();
	ciphertext_len = len;

	/*
	 * Finalise the encryption. Further ciphertext bytes may be written at
	 * this stage.
	 */
	if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
		handleEncryptionErrors();
	ciphertext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}


int EncDec::decrypt(const unsigned char *ciphertext, int ciphertext_len, const unsigned char *key, const unsigned char *iv,
				unsigned char *plaintext) {
	EVP_CIPHER_CTX *ctx;

	int len;

	int plaintext_len;

	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new()))
		handleDecryptionErrors();

	/*
	 * Initialise the decryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits
	 */
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv))
		handleDecryptionErrors();

	/*
	 * Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary.
	 */
	if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		handleDecryptionErrors();
	plaintext_len = len;

	/*
	 * Finalise the decryption. Further plaintext bytes may be written at
	 * this stage.
	 */
	if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
		handleDecryptionErrors();
	plaintext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}

int EncDec::pbkdf_256_bit(const std::string& password, std::vector<unsigned char>& key) {
	const int iter = 100000;			// recursion depth of hash function
	const unsigned int key_length = 32; // 32 * 8 byte = 256 bit - length is important
	key.resize(key_length);
	// generate key derived by password
	return PKCS5_PBKDF2_HMAC_SHA1(password.c_str(), password.length(),
								  nullptr, 0, iter, key_length, key.data());
}

int EncDec::random_iv_128_bit(std::vector<unsigned char> &iv) {
	iv.resize(16);
	return RAND_bytes(iv.data(), 16);
}
