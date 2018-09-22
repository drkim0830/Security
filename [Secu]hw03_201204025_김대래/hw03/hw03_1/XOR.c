#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int GetFileSize(FILE * fp);
char Encrypt(char* buff, int key_modified);
char Decrypt(char* buff, int key_modified);
bool isUpper(int ascii);
bool isLower(int ascii);


void main(){
	char inputFileName[256];
	char outputFileName[256];
	FILE *input_FD;
	FILE *output_FD;

	int mode;
	char key[26];

	printf(">> Input file name : ");
	scanf("%s", inputFileName);

	printf(">> Input mod [0 : Encrypt, 1 : Decrypt] :");
	scanf("%d", &mode);

	printf(">> Input key : ");
	scanf("%s", key);

	if ( mode == 0 ){
		sprintf(outputFileName, "encrypt.txt");
	}
	else if ( mode == 1 ){
		sprintf(outputFileName, "decrypt.txt");
	}
	else{
		printf("[!] Mode Error!\n");
		exit(1);
	}

	input_FD = fopen(inputFileName, "rb");
	output_FD = fopen(outputFileName, "wb");

	int fileSize = GetFileSize(input_FD);
	char buff;
	int i;

	int len;
	int key_modified;

	for ( i=0; i < fileSize; i++){
		fread(&buff, sizeof(char), 1, input_FD);

		len = i % strlen(key);
		key_modified = key[len];

		if (mode == 0){
			buff = Encrypt(&buff, key_modified);
		}
		else if (mode == 1){
			buff = Decrypt(&buff, key_modified);
		}

		fwrite(&buff, sizeof(char), 1, output_FD);
	}
	fclose(output_FD);
	fclose(input_FD);

}

int GetFileSize(FILE * fp)
{
	int offset_bkup;
	int fsize;

	offset_bkup = ftell(fp);
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	fseek(fp, offset_bkup, SEEK_SET);

	printf("File Size = %d\n", fsize);
	return fsize;
}

char Encrypt(char* buff, int key_modified){
	char enc_buff;
	enc_buff = *buff ^ key_modified;
	
	return enc_buff;
}

char Decrypt(char* buff, int key_modified){
	char dec_buff;
	dec_buff = *buff ^ key_modified;
	
	return dec_buff;
}

bool isUpper(int ascii){
	if(ascii >=65 && ascii <=90)
		return true;
	else
		return false;
}

bool isLower(int ascii){
	if(ascii >=97 && ascii <=122)
		return true;
	else
		return false;
}
