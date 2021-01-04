#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <errno.h>


#include "vault_ioctl.h"


int main(int argc, char* argv[]){
	int fd;
	int res;
	char *input, *output;
	char *ioctl_clear_test_string;
	char *new_key;
	int slen;
	int length;
	
	fd = open("/dev/vault0", O_RDWR);
	if(fd < 0) {
		printf("Cannot open device\n");
		return -1;
	}
	
	
	length = 100;
	
	
	
	/**
	 * Write Test 
	 */
	printf("\n########## WRITE ##########\n");
	input = malloc(length);
	strcpy(input, "System Programming Project 2. This is a test file.");
	slen = strlen(input);
	res = pwrite(fd, input, slen, 0);
	if (res == -1)
		printf("Cannot write, Error code: %d (%s)\n", errno, strerror(errno));
	else {
		printf("characters written: %d\n", res);
		printf("Written value:\t%s\n", input);
	}
	free(input); input = NULL;
	
	
	
	/**
	 * Read Test 
	 */
	printf("\n########## READ ##########\n");
	output = malloc(length);
	//lseek(fd, 0, SEEK_SET);
	res = pread(fd, output, length, 0);
	if (res == -1)
		printf("Cannot read, Error code: %d (%s)\n", errno, strerror(errno));
	else {
		printf("characters read: %d\n", res);
		printf("read value:\t%s\n", output);	
	}
	free(output); output = NULL;
	
	
	
	/**
	 * IOCTL - Clear Test 
	 */
	printf("\n########## CLEAR ##########\n");
	ioctl_clear_test_string = malloc(length);
	res = ioctl(fd, VAULT_IOC_CLEAR);
	printf("Clear ioctl result: %d\n", res);
	
	
	
	/**
	 * Read from empty device Test 
	 */
	printf("\n########## READ AFTER CLEAR ##########\n");
	res = pread(fd, ioctl_clear_test_string, length, 0);
	if (res == -1)
		printf("Cannot read, Error code: %d (%s)\n", errno, strerror(errno));
	else {
		printf("characters read: %d\n", res);
		printf("read value: %s\n", ioctl_clear_test_string);	
	}
	free(ioctl_clear_test_string); ioctl_clear_test_string = NULL;
	
	
	
	/**
	 * IOCTL - [SetKey -> Write -> Read] Test 
	 */
	printf("\n########## SETKEY ##########\n");
	new_key = malloc(5); memset(new_key, 0, 5);
	strcpy(new_key, "caeyf");
	//new_key = "caeyf";
	res = ioctl(fd, VAULT_IOC_SETKEY, new_key);
	printf("SetKey ioctl result: %d\n", res);
	printf("New key is set to: %s\n", new_key);
	free(new_key); new_key = NULL;
	
	printf("\n########## WRITE & READ with NEW KEY ##########\n");
	input = malloc(length); memset(input, 0, length);
	strcpy(input, "IOCTL SetKey test string.");
	slen = strlen(input);
	res = pwrite(fd, input, slen, 0);
	if (res == -1)
		printf("Cannot write, Error code: %d (%s)\n", errno, strerror(errno));
	else {
		printf("characters written: %d\n", res);
		printf("Written value:\t%s\n", input);
	}
	free(input); input = NULL;
	
	output = malloc(length); memset(output, 0, length);
	res = pread(fd, output, length, 0);
	if (res == -1)
		printf("Cannot read, Error code: %d (%s)\n", errno, strerror(errno));
	else {
		printf("characters read: %d\n", res);
		printf("read value:\t%s\n", output);	
	}
	free(output); output = NULL;
	
	
	
	/**
	 * IOCTL - [Write -> SetKey -> Read] Test 
	 */
	printf("\n########## WRITE -> SETKEY -> READ ##########\n");
	
	input = malloc(length); memset(input, 0, length);
	strcpy(input, "IOCTL SetKey test string.");
	slen = strlen(input);
	res = pwrite(fd, input, slen, 0);
	if (res == -1)
		printf("Cannot write, Error code: %d (%s)\n", errno, strerror(errno));
	else {
		printf("characters written: %d\n", res);
		printf("Written value:\t%s\n", input);
	}
	free(input); input = NULL;
	
	new_key = malloc(5); memset(new_key, 0, 5);
	strcpy(new_key, "great");
	res = ioctl(fd, VAULT_IOC_SETKEY, new_key);
	printf("SetKey ioctl result: %d\n", res);
	printf("New key is set to: %s\n", new_key);
	free(new_key); new_key = NULL;
	
	output = malloc(length); memset(output, 0, length);
	res = pread(fd, output, length, 0);
	if (res == -1)
		printf("Cannot read, Error code: %d (%s)\n", errno, strerror(errno));
	else {
		printf("characters read: %d\n", res);
		printf("read value:\t%s\n", output);	
	}
	free(output); output = NULL;
	
	
	printf("\n");
	return 0;
}
