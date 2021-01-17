#define FUSE_USE_VERSION 26

static const char* rofsVersion = "2008.09.24";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include "cJSON.h"

char dir_list[ 256 ][ 256 ];
int curr_dir_idx = 0;

char files_list[ 256 ][ 256 ];
int curr_file_idx = 0;

char files_content[ 256 ][ 256 ];
int curr_file_content_idx = 0;
char path[256];

void parse_array(cJSON *arrayItem)
{
	if(!arrayItem)
		return;
	while(arrayItem) {
		if(arrayItem->next) {
			if(!arrayItem->next->child) {
				arrayItem->next = NULL;
			} else {
				arrayItem = arrayItem->next;
			}
		} else if (arrayItem->child && arrayItem->child->child) {
			if(arrayItem->string) {
				
				strncat(path, arrayItem->string, strlen(arrayItem->string));
				strncat(path, "/", 1);
			}
			arrayItem = arrayItem->child;
		} else if(arrayItem->child && arrayItem->child->valuestring) {
			cJSON *values = arrayItem->child;
			
			strncat(path, arrayItem->string, strlen(arrayItem->string));
			strncat(path, "/", 1);
			while(values) {
				strcpy(dir_list[curr_dir_idx], path);
				strncat(dir_list[curr_dir_idx++], values->string, strlen(values->string));
				
				strcpy(files_list[curr_file_idx++], values->string);
				strcpy(files_content[curr_file_content_idx++], values->valuestring);
				
				values = values->next;
			}
			arrayItem->child = NULL;
			arrayItem = arrayItem->child;
		} else {
			arrayItem->child = NULL;
			arrayItem = arrayItem->child;
		}
	}	
	memset(path, '\0', sizeof(path));
}

int isFile(const char *path) {
	printf("isFile: %s\n", path);
	path++;
	
	int i=0;
    for(i=0; i<curr_file_idx; i++) { 
		if (strcmp(path, files_list[i]) == 0) {
			printf("isFile i: %d\n", i);
			return i;
			}
	}
	printf("isFile i: %d\n", -1);
	return -1;
}

static int json_getattr(const char *path, struct stat *stbuf)
{
	printf("getattr: %s \n", path);
    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else if (isFile(path) != -1) {
		
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        printf("getattr: %s\n", files_content[isFile(path)]);
        stbuf->st_size = strlen(files_content[isFile(path)]); 
    }
    else
        res = -ENOENT;

    return res;
}


static int json_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
	printf("readdir attr: %s\n", path);
    (void) offset;
    (void) fi;

    //if (strcmp(path, "/") != 0)
        //return -ENOENT;
	
    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    
		int i=0;
		for(i=0; i<curr_dir_idx; i++) { 
			filler(buf, dir_list[i], NULL, 0);
		}
		for(i=0; i<curr_dir_idx; i++) { 
			filler(buf, files_list[i], NULL, 0);
		}
	

    return 0;
}


static int json_open(const char *path, struct fuse_file_info *fi)
{
	printf("json_open attr: %s\n", path);
	int check = -1;
	int i=0;
    for(i=0; i<curr_dir_idx; i++) { 
		if (strcmp(path, dir_list[i]) == 0)
			check = 1;
	}
	
	if(check != -1)
		return -ENOENT;

    if ((fi->flags & 3) != O_RDONLY)
        return -EACCES;

    return 0;
}

static int json_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
	
    printf("jjson_read attr %s:\n", path);
    size_t len;
    (void) fi;
    int check = -1;
    int i=0;
    for(i=0; i<curr_dir_idx; i++) { 
		if (strcmp(path, dir_list[i]) == 0)
			check = i;
	}
	
	if(check == -1)
		return -ENOENT;

    len = strlen(files_content[check]);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, files_content[check] + offset, size);
    } else
        size = 0;

    return size;
}

static struct fuse_operations json_oper = {
    .getattr	= json_getattr,
    .readdir	= json_readdir,
    .open	= json_open,
    .read	= json_read,
};
int main(int argc, char *argv[])
{
	FILE *f = NULL;
	long len = 0;
	char *data = NULL;
	
	f = fopen("example.json", "rb");
	fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
	
    data = (char*)malloc(len + 1);

    fread(data, 1, len, f);
    data[len] = '\0';
    
    fclose(f);
    
    cJSON *json = cJSON_Parse(data);
    int keyCount = cJSON_GetArraySize(json);
    
    int i = 0;
	while(json->child) {
		parse_array(json);
	}
    
    i = 0;
	for(i=0; i<curr_dir_idx; i++) {
		printf("dir: %s \n", dir_list[i]);
		printf("files_name: %s \n", files_list[i]);
		printf("files_content: %s \n\n", files_content[i]);
	}
    free(data);
    
    return fuse_main(argc, argv, &json_oper, NULL);
}


