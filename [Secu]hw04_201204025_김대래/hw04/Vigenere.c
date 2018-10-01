#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int GetFileSize(FILE * fp);
void shift_right(int round);
char Decrypt(char* buff);
bool isLower(int ascii);

//define global variable keytable
char keytable[27] = "koegbpdwitncmfryqzulashxvj";

void main(){

	char inputFileName[256];
	char outputFileName[256];
	FILE *input_FD;
	FILE *output_FD;

	int mode;

	printf(">> Input file name : ");
	scanf("%s", inputFileName);

	sprintf(outputFileName, "decrypt.txt");

	input_FD = fopen(inputFileName, "rb");
	output_FD = fopen(outputFileName, "wb");

	int fileSize = GetFileSize(input_FD);
	char buff;
	int i;
	int iterator = 0;
	for ( i=0; i < fileSize; i++){
		fread(&buff, sizeof(char), 1, input_FD);
	
		//if plaintext is in English, decrypt it
		if(isLower((int)buff)){
			if(i > 0){
				//if shift 3 before
				if(iterator == 3){
					shift_right(4);
					buff = Decrypt(&buff);
					iterator = 4;
				}
				//if shift 4 before
				else if(iterator == 4){
					shift_right(3);
					buff = Decrypt(&buff);
					iterator = 3;
				}
			}
			//Index 0 -> Not shift
			else{
				buff = Decrypt(&buff);
				iterator = 4;
			}

		}
		//otherwise output it

		fwrite(&buff, sizeof(char), 1, output_FD);
	}
	fclose(output_FD);
	fclose(input_FD);

}

int GetFileSize(FILE * fp){
	int offset_bkup;
	int fsize;

	offset_bkup = ftell(fp);
	fseek(fp, 0, SEEK_END);
	fsize = ftell(fp);
	fseek(fp, offset_bkup, SEEK_SET);
	
	printf("File Size = %d\n", fsize);
	return fsize;
}

/* Shift Function : Shift the global keytable by taking argument(round) */
void shift_right(int round){
	int i, j;
	int temp = 0;
	for( i = 0; i < round; i++){
		temp = keytable[25];
		for( j = 24; j >= 0; j--){
			keytable[j+1] = keytable[j];
		}
		keytable[0] = temp;
	}
}

char Decrypt(char* buff){
	char dec_buff;
	int i;

	int table_size = sizeof(keytable);
	for(i=0; i<table_size; i++){
		if(keytable[i] == *buff){
			dec_buff = i+97;
		}
	}
	
	return dec_buff;
}

bool isLower(int ascii){
	if(ascii >= 97 && ascii <= 122){
		return true;
	}
	else{
		return false;
	}
}

