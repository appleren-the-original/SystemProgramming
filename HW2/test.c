#include <stdio.h>
#include "encryption.h"


int main(int argc, char* argv[]){
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
}
