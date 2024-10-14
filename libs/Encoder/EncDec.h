
#ifndef ENCDEC_H
#define ENCDEC_H


#include <string>
#include <vector>

class EncDec {
public:
	static int encrypt(const unsigned char *plaintext, int plaintext_len, const unsigned char *key,
					   const unsigned char *iv, unsigned char *ciphertext);
	static int decrypt(const unsigned char *ciphertext, int ciphertext_len, const unsigned char *key,
					   const unsigned char *iv, unsigned char *plaintext);
	static int pbkdf_256_bit(const std::string& password, std::vector<unsigned char>& key);
	static int random_iv_128_bit(std::vector<unsigned char>& iv);
};


#endif //ENCDEC_H
