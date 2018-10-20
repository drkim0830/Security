#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>
#include <openssl/aes.h>


#define BLOCK_SIZE 64
#define KEY_SIZE 8

unsigned int des(unsigned char*, unsigned char*, unsigned int, int);
unsigned int aes_cbc(unsigned char* , unsigned char*, unsigned int, int);

int main(){
	char inputFileName[256];
	char outputFileName[256];
	char *key = (char *)malloc(sizeof(char) * KEY_SIZE);
	FILE *input_FD;
	FILE *output_FD;

	int mode;
	int crypto_algo;

	printf("encrypt[1]\tdecrypt[2] : ");
	scanf("%d", &mode);

	printf("file : ");
	scanf("%s", inputFileName);

	printf("key : ");
	scanf("%s", key);
	
	printf("aes-cbc[1]\tdes[2] : ");
	scanf("%d", &crypto_algo);

	if( mode == 1 ){
		sprintf(outputFileName,"plain.enc");
	}
	else if( mode == 2 ){
		sprintf(outputFileName, "plain.enc.dec");
	}
	else{
		printf("[!] Mode Error!\n");
		exit(1);
	}

	input_FD = fopen(inputFileName, "rb");
	output_FD = fopen(outputFileName, "wb");

	char *buff = (char *)malloc(sizeof(char) * BLOCK_SIZE);
	int t;
	
	while( 0 < (t = fread(buff, sizeof(char), BLOCK_SIZE, input_FD))){
		int result = 0;
		printf("\nt = %d\n", t);
		if(crypto_algo == 1){
			result = aes_cbc(buff, key, t, mode);
		}
		else if(crypto_algo == 2){
			result = des(buff, key, t, mode);
		}
		printf("\n%d\n", result);
		fwrite(buff, sizeof(char), result, output_FD);
		memset(buff, 0, sizeof(char)*BLOCK_SIZE);
	}
	
	fclose(output_FD);
	fclose(input_FD);

	return 0;
}

unsigned int des(unsigned char* msg, unsigned char* key, unsigned int msg_len, int mode)
{
	DES_key_schedule des_ks;
	DES_cblock	des_key = {0, };
	DES_cblock iv = {0, };
	unsigned int i, result, padding;

	unsigned char block_in[BLOCK_SIZE] = {0, };
	unsigned char block_out[BLOCK_SIZE] = {0, };

	DES_string_to_key(key, &des_key);
	DES_set_key_checked(&des_key, &des_ks);

	memcpy(block_in, msg, msg_len);

	if( mode == 1 ){
		if( msg_len < BLOCK_SIZE ){
			padding = BLOCK_SIZE - msg_len;
			block_in[BLOCK_SIZE - 1] = padding;
		}
		DES_ncbc_encrypt(block_in, block_out, BLOCK_SIZE, &des_ks, &iv, DES_ENCRYPT);
		result = BLOCK_SIZE;
	}
	else if( mode == 2 ){
		DES_ncbc_encrypt(block_in, block_out, BLOCK_SIZE, &des_ks, &iv, DES_DECRYPT);
		padding = block_out[BLOCK_SIZE - 1];
		result = BLOCK_SIZE - padding;
	}
	memcpy(msg, block_out,BLOCK_SIZE);

	return result;
}

unsigned int aes_cbc(unsigned char* msg, unsigned char* key, unsigned int msg_len, int mode){
	AES_KEY enc_key, dec_key;
	unsigned char iv[BLOCK_SIZE] = {0, };
	unsigned int result, padding;

	unsigned char block_in[BLOCK_SIZE] = {0, };
	unsigned char block_out[BLOCK_SIZE] = {0, };

	AES_set_encrypt_key(key, 128, &enc_key);
	AES_set_decrypt_key(key, 128, &dec_key);

	memcpy(block_in, msg, msg_len);
	if(mode == 1){
		if(msg_len < BLOCK_SIZE){
			padding = BLOCK_SIZE - msg_len;
			block_in[BLOCK_SIZE - 1] = padding;
		}
		AES_cbc_encrypt(block_in, block_out, BLOCK_SIZE, &enc_key, iv, AES_ENCRYPT);
		result = BLOCK_SIZE;
	}
	else if ( mode == 2 ){
		AES_cbc_encrypt(block_in, block_out, BLOCK_SIZE, &dec_key, iv, AES_DECRYPT);
		padding = block_out[BLOCK_SIZE - 1];
		result = BLOCK_SIZE - padding;
	}
	memcpy(msg, block_out, BLOCK_SIZE);

	return result;
}
