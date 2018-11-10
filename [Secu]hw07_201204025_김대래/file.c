#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

#define KEY_LENGTH 1024
#define BLOCK_SIZE 256
#define PUB_EXP 3

RSA* make_keypair();

int padding = RSA_PKCS1_PADDING;

int main(){
	RSA* keypair = make_keypair();
	
	char plainFileName[256];
	char encryptFileName[256];
	char decryptFileName[256];

	sprintf(plainFileName, "plain.txt");
	sprintf(encryptFileName, "plain.enc");
	sprintf(decryptFileName, "plain.enc.dec");

	FILE *input_FD = fopen(plainFileName, "rb");
	FILE *enc_output_FD = fopen(encryptFileName, "wb");
	FILE *dec_output_FD = fopen(decryptFileName, "wb");
	
	unsigned char *encrypted_buff = (unsigned char *)malloc(sizeof(unsigned char) * BLOCK_SIZE);
	unsigned char *decrypted_buff = (unsigned char *)malloc(sizeof(unsigned char) * BLOCK_SIZE);
	unsigned char *buff = (unsigned char *)malloc(sizeof(unsigned char) * BLOCK_SIZE);
	
	int t;
	while( 0 < (t = fread(buff, sizeof(unsigned char), BLOCK_SIZE, input_FD))){
		int encrypted_length, decrypted_length;
		printf("File Size : %d\n", t);
		encrypted_length = RSA_public_encrypt(t, buff, encrypted_buff, keypair, padding);
		fwrite(encrypted_buff, sizeof(unsigned char), encrypted_length, enc_output_FD);
		printf("File Size : %d\n", encrypted_length);

		decrypted_length = RSA_private_decrypt(encrypted_length, encrypted_buff, decrypted_buff, keypair, padding);
		fwrite(decrypted_buff, sizeof(unsigned char), decrypted_length, dec_output_FD);
	}

	fclose(input_FD);
	fclose(enc_output_FD);
	fclose(dec_output_FD);
	
	return 0;
}

RSA* make_keypair(){
	RSA *keypair = RSA_generate_key(KEY_LENGTH, PUB_EXP, NULL, NULL);

	//To get the C-string PEM form:
	BIO *pri = BIO_new(BIO_s_mem());
	BIO *pub = BIO_new(BIO_s_mem());

	PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
	PEM_write_bio_RSAPublicKey(pub, keypair);

	int pri_len = BIO_pending(pri);
	int pub_len = BIO_pending(pub);

	char* pri_key = malloc(pri_len + 1);
	char* pub_key = malloc(pub_len + 1);

	BIO_read(pri, pri_key, pri_len);
	BIO_read(pub, pub_key, pub_len);

	pri_key[pri_len] = '\0';
	pub_key[pub_len] = '\0';

	FILE *key_file;
	key_file = fopen("prkey.pem", "w");
	fprintf(key_file, "%s", pri_key);
	fclose(key_file);
	key_file = fopen("pukey.pem", "w");
	fprintf(key_file, "%s", pub_key);
	fclose(key_file);

	key_file = fopen("rsa.key", "w");
	RSA_print_fp(key_file, keypair, 0);
	fclose(key_file);

	return keypair;
}