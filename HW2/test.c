#include <stdio.h>
#include "encryption.h"

// int len(char* word) {
// 	int res = 0;
// 	for(int i = 0; word[i] != '\0'; i++) {
// 		res++;
// 	}
// 	return res;
// }

int main(int argc, char* argv[]){
	char* msg = "oneringtorulethemall";
	char* alphabet = "abcdefghijklmnopqrstuvwxyz";
	// printf("%d \n", len(msg));
	char* key = "ceayf";
	char* m = malloc(20* sizeof(int));
	enc(alphabet, key, msg, m);
	printf("%s\n", m);
	free(m);
	// setKeys(key);
	// char* enc = encrypt(msg, 1);
	// char* dec = encrypt(enc, 0);
	// printf("%s\n", enc);
	// printf("%s\n", dec);
	//
	// char* msg2 = "alperensystem";
	// char* key2 = "ceayf";
	// setKeys(key2);
	// char* enc2 = encrypt(msg2, 1);
	// char* dec2 = encrypt(enc2, 0);
	// printf("%s\n", enc2);
	// printf("%s\n", dec2);

	return 0;
}
