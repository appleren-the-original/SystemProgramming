#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


#define VAULT_IOC_MAGIC 'k'
#define VAULT_IOC_SETKEY _IOW(VAULT_IOC_MAGIC, 0, char*)
#define VAULT_IOC_CLEAR _IO(VAULT_IOC_MAGIC, 1)
#define VAULT_IOC_MAXNR 1


int main(int argc, char* argv[]){
	int fd;
	int res;
	char* str;
	int slen;
	
	fd = open("/dev/vault0", O_RDWR);
	if(fd < 0) {
		printf("Cannot open device\n");
		return -1;
	}
	
	str = "alperen ahmet yusuf system programming.";
	slen = strlen(str);
	res = write(fd, str, slen);
	printf("characters written: %d\n", res);
	
	str = "(if you see this, something is wrong.)";
	//lseek(fd, 0, SEEK_SET);
	//res = read(fd, str, slen);
	res = pread(fd, str, slen, 0);
	printf("characters read: %d\n", res);
	printf("read value: %s\n", str);
	
	
	return 0;
}
