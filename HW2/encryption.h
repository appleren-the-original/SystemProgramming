#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int key_len;
int* key_array;
int* key_encrypt;
int* key_decrypt;


int compare(const void* val1, const void* val2){
	return key_array[*(int*)val1] - key_array[*(int*)val2];
}

/**
 * This function sets the global key arrays' values. 
 */
void setKeys(char* key){
	key_len = strlen(key);
	int index[key_len];
	
	key_array = malloc(key_len * sizeof(int));
	for(int i=0; i<key_len; i++)
		key_array[i] = key[i];
		
	for(int i=0; i<key_len; i++)
		index[i] = i;
	qsort(index, key_len, sizeof(int), compare);
	
	key_encrypt = malloc(key_len * sizeof(int));
	key_decrypt = malloc(key_len * sizeof(int));
	for(int i=0; i<key_len; i++){
		key_encrypt[index[i]] = i;
		key_decrypt[i] = index[i];
	}
}



/**
 * Function for printing 2D arrays 
 */
void printmatrix(char** matrix, int nrows, int ncols){
	for(int i=0; i<nrows; i++){
		for(int j=0; j<ncols; j++){
			printf("%c ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

/**
 * @param msg	string to be encrypted
 * @param mode	boolean value, 1: encrypt & 0: decrypt
 * 				only used keys differ 
 * @return 		resulting encrypted / decrypted string
 * */
char* encrypt(char* msg, int mode){
	int len = strlen(msg);
	int ncols = key_len; // previously set by setKeys function call
	int nrows = len / ncols;
	if (len % ncols != 0) nrows++;
	
	
	// create a matrix for storing the message
	char** matrix;
	matrix = malloc(sizeof(char*) * nrows);
	for(int i=0; i<nrows; i++)
		matrix[i] = malloc(sizeof(char) * ncols);
	
	// build initial matrix with the message
	for(int i=0; i<nrows; i++){
		for(int j=0; j<ncols; j++){
			if (i*ncols + j >= len)
				matrix[i][j] = '0';
			else
				matrix[i][j] = msg[i*ncols + j];
		}
	}
	// printmatrix(matrix, nrows, ncols); // DEBUG
	
	// create encrypted message using initial matrix and encryption key
	char* msg_result = malloc(nrows * ncols);
	if (mode){ // encryption
		for(int i=0; i<nrows; i++){
			for(int j=0; j<ncols; j++){
				msg_result[i*ncols + j] = matrix[i][key_encrypt[j]];
			}
		}
	}
	else{ // decryption
		int break_flag = 0;
		for(int i=0; i<nrows; i++){
			for(int j=0; j<ncols; j++){
				if (matrix[i][key_decrypt[j]] == '0'){
					break_flag = 1;
					break;
				}
				msg_result[i*ncols + j] = matrix[i][key_decrypt[j]];
			}
			if (break_flag) break;
		}
	}
	
	// free the memory allocated for the matrix
	for(int i=0; i<nrows; i++)
		free(matrix[i]);
	free(matrix);


	return msg_result;
}



/*int main(int argc, char* argv[]){
	char* msg = "oneringtorulethemall";
	char* key = "ceayf";
	setKeys(key);
	char* enc = encrypt(msg, 1);
	char* dec = encrypt(enc, 0);
	printf("%s\n", enc);
	printf("%s\n", dec);
	
	char* msg2 = "alperensystem";
	char* key2 = "ceayf";
	setKeys(key2);
	char* enc2 = encrypt(msg2, 1);
	char* dec2 = encrypt(enc2, 0);
	printf("%s\n", enc2);
	printf("%s\n", dec2);
	
	
	return 0;
}*/
