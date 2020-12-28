#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int* key_array;
int* key_encrypt;
int* key_decrypt;


int compare(const void* val1, const void* val2){
	return key_array[*(int*)val1] - key_array[*(int*)val2];
}

void setEncryptionKey(char* key){
	int len = strlen(key);
	int index[len];
	
	key_array = malloc(len * sizeof(int));
	for(int i=0; i<len; i++)
		key_array[i] = key[i];
		
	for(int i=0; i<len; i++)
		index[i] = i;
	qsort(index, len, sizeof(int), compare);
	
	key_encrypt = malloc(len * sizeof(int));
	for(int i=0; i<len; i++)
		key_encrypt[index[i]] = i;
}



void printmatrix(char** matrix, int nrows, int ncols){
	for(int i=0; i<nrows; i++){
		for(int j=0; j<ncols; j++){
			printf("%c ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

char* encrypt(char* msg, char* key){
	int len = strlen(msg);
	int ncols = strlen(key);
	int nrows = len / ncols + 1;
	if (len % ncols == 0) nrows--;
	
	char* msg_encrypted = malloc(nrows * ncols);
	
	// create a matrix for storing the message
	char** matrix;
	matrix = malloc(sizeof(char*) * nrows);
	for(int i=0; i<nrows; i++){
		matrix[i] = malloc(sizeof(char) * ncols);
	}
	
	// build initial matrix with the message
	for(int i=0; i<nrows; i++){
		for(int j=0; j<ncols; j++){
			if (i*ncols + j >= len)
				matrix[i][j] = '0';
			else
				matrix[i][j] = msg[i*ncols + j];
		}
	}
	printmatrix(matrix, nrows, ncols);
	
	// create encrypted message using initial matrix and encryption key
	setEncryptionKey(key);
	for(int i=0; i<nrows; i++){
		for(int j=0; j<ncols; j++){
			msg_encrypted[i*ncols + j] = matrix[i][key_encrypt[j]];
		}
	}
	
	return msg_encrypted;
}

char* decrypt(char* msg, char* key){
	return "at";
}



int main(int argc, char* argv[]){
	char* msg = "oneringtorulethemall";
	char* key = "ceayf";
	char* res = encrypt(msg, key);
	printf("%s\n", res);
	
	char* msg2 = "alperensystem";
	char* key2 = "ceayf";
	char* res2 = encrypt(msg2, key2);
	printf("%s\n", res2);
	
	
	return 0;
}
