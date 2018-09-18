#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int GetFileSize(FILE * fp);
char Encrypt(char* buff, int key);
char Decrypt(char* buff, int key);
bool isUpper(int ascii);
bool isLower(int ascii);

int main(void)
{
	char inputFileName[256];
	char outputFileName[256];
	FILE * input_FD;
	FILE * output_FD;
	int mode;
	int key;

	//Input File Name
	printf(">> Input File Name : ");
	scanf("%s", inputFileName);

	//Input Mode
	printf(">> Input Mode [ 0 : Encrypt, 1 : Decrypt ] : ");
	scanf("%d", &mode);

	//Input Key
	printf(">> Input Key : ");
	scanf("%d", &key);

	//Make Output File Name
	if ( mode == 0 ){
		sprintf(outputFileName, "encrypt.txt");
	}
	else if ( mode == 1 ){
		sprintf(outputFileName, "decrpyt.txt");
	}
	else {
		printf("[!] Mode Error !\n");
		exit(1);
	}

	input_FD = fopen(inputFileName, "rb");
	output_FD = fopen(outputFileName, "wb");
	int fileSize = GetFileSize(input_FD);
	char buff;
	int i;
	for ( i = 0; i < fileSize; i++ ){
		//Read input file data to buffer (size of char=1)
		fread(&buff, sizeof(char), 1, input_FD);

		if ( mode == 0 ){
			//Encrypt
			buff = Encrypt(&buff, key);
		}
		else if ( mode == 1 ){
			//Decrypt
			buff = Decrypt(&buff, key);
		}

		//Write file
		fwrite(&buff, sizeof(char), 1, output_FD);
	}
	fclose(output_FD);
	fclose(input_FD);

	return 0;
}

int GetFileSize(FILE * fp){
	int offset_bkup;
	int fsize;

	//present location backup
	offset_bkup = ftell(fp);
	//end of file
	fseek(fp, 0, SEEK_END);
	//size of bite,get last location value
	fsize = ftell(fp);
	//return to backup offset fp
	fseek(fp, offset_bkup, SEEK_SET);

	printf("File Size = %d\n", fsize);
	return fsize;
}

char Encrypt(char* buff, int key){
	/*
	 * Encryption shuld add the key value
	 */
	char enc_buff;
	int buffToInt = (int)*buff;	//buff character's ASCII Num
	
	//If out of range(uppercase and lowercase), Minus 26
	if( isUpper(buffToInt) ){
		enc_buff = *buff + key;
		if ( !isUpper((int)enc_buff) ){
			enc_buff -= 26;
		}
	}
	else if( isLower(buffToInt) ){
		enc_buff = *buff + key;
		if ( !isLower((int)enc_buff) ){
			enc_buff -= 26;
		}
	}
	else{
		return *buff;
	}
	
	return enc_buff;
}

char Decrypt(char* buff, int key){
	/*
	 *Decryption should subtract the key value
	 */
	char dec_buff;
	
	int buffToInt = (int)*buff;	//buff character's ASCII Num

	//If out of range(uppercase and lowercase), plus 26
	if( isUpper(buffToInt) ){
		dec_buff = *buff - key;
		if ( !isUpper((int)dec_buff) ){
			dec_buff += 26;
		}
	}
	else if( isLower(buffToInt) ){
		dec_buff = *buff - key;
		if ( !isLower((int)dec_buff) ){
			dec_buff += 26;
		}
	}
	else{
		return *buff;
	}
	return dec_buff;
}

bool isUpper(int ascii) {
	if( ascii >= 65 && ascii <= 90){
		return true;
	}
	return false;
}

bool isLower(int ascii) {
	if( ascii >= 97 && ascii <= 123){
		return true;
	}
	return false;
}
