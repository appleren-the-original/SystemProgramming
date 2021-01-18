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

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello/hello";
static const char *hello_path_fol = "/hello";
void parse_array(cJSON *arrayItem)
{
	if(!arrayItem)
		return;
	char temp_dir_list[ 256 ][ 256 ];
	int temp_curr_dir_idx = 0;
	
	while(arrayItem) {
		if(arrayItem->next) {
			if(!arrayItem->next->child) {
				arrayItem->next = NULL;
			} else {
				arrayItem = arrayItem->next;
			}
		} else if (arrayItem->child && arrayItem->child->child) {
			if(arrayItem->string) {
				strncat(path, "/", 1);
				strncat(path, arrayItem->string, strlen(arrayItem->string));
				strcpy(temp_dir_list[temp_curr_dir_idx++], path);
			}
			arrayItem = arrayItem->child;
		} else if(arrayItem->child && arrayItem->child->valuestring) {
			cJSON *values = arrayItem->child;
			strncat(path, "/", 1);
			strncat(path, arrayItem->string, strlen(arrayItem->string));
			strcpy(temp_dir_list[temp_curr_dir_idx++], path);
			int oneTime = 0;
			while(values) {
				int i=0;
				int j=0; 
				for(i=0; i<temp_curr_dir_idx; i++) {
					for(j=0; j<curr_dir_idx; j++) {
						if (strcmp(temp_dir_list[i], dir_list[j]) == 0) {
							//printf("temp_dir_list: %s \n", temp_dir_list[i]);
							oneTime = 1;
							break;
						} 
					}
				if(oneTime == 0) {
					strcpy(dir_list[curr_dir_idx++],temp_dir_list[i]);
				} 
				oneTime = 0;	
				}
				strcpy(files_list[curr_file_idx], path);
				strncat(files_list[curr_file_idx], "/", 1);
				strncat(files_list[curr_file_idx++], values->string, strlen(values->string));
				
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
int pathfind(const char *path1, char *path2) {
	//printf("pathfind fonk %s\n", path1);
    int i1 = strlen(path1);

    int i2 = strlen(path2);
    if (i1 >= i2) return -1;
    int i = 0;
    int s = 0;
    for (i = 0; i < i1; i++) {
        if (path1[i] != path2[i]) return -1;
    }
    for (i = i1; i < i2; i++) {
        if (path2[i] == '/') s++;
        if (s > 1) return -1;
        
    }
	if (strcmp(path1, "/") == 0) {
		if (s >= 1) return -1;
        return 1;
    }
    return i1 + 1;
}

int isFile(const char* path) {
	int check = -1;
	int i=0;
	for(i=0; i<curr_file_idx; i++) {
		if(strcmp(path,files_list[i]) == 0) {
			check = i;
		}
	}
	return check;
	
}

int isDir(const char* path) {
	
	int check = -1;
	int i=0;
	for(i=0; i<curr_dir_idx; i++) {
		if(strcmp(path,dir_list[i]) == 0) {
			check = 1;
		}
	}
	return check;
}

static int json_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
	int link = 0;
	int i = 0;
	for(i=0; i<curr_dir_idx; i++) {
		if (pathfind(path, dir_list[i]) != -1) link++;
	}
	for(i=0; i<curr_file_idx; i++) {
		if (pathfind(path, files_list[i]) != -1) link++;
	}
    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0 || isDir(path) != -1) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = link;
    }
    else if (isFile(path) != -1) {
		
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(files_content[isFile(path)]);
    }
    else
        res = -ENOENT;

    return res;
}

static int json_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;

    if (strcmp(path, "/") != 0 && isDir(path) == -1)
        return -ENOENT;
    if(strcmp(path, "/") == 0 || isDir(path) != -1) {
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);
        int i=0;
		
		for(i=0; i<curr_dir_idx; i++) {
			int off = pathfind(path, dir_list[i]);
			if(off != -1) {
				printf("Burasi %d %s %s %s \n",off,path, dir_list[i], dir_list[i] + off);
				filler(buf, dir_list[i] + off, NULL, 0);
			}
		}
		for(i=0; i<curr_file_idx; i++) {
			int off = pathfind(path, files_list[i]);
			if (off != -1) {
				filler(buf, files_list[i] + off, NULL, 0);
			}
		}
		
    }
    else{
		return -EPERM;
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);
        int i=0;
		for(i=0; i<curr_file_idx; i++) {
			if(strcmp(path, files_list[i]) == 0) { 
				filler(buf, files_list[i+1]+1, NULL, 0);
			}
		}
    }
    return 0;
}

static int json_open(const char *path, struct fuse_file_info *fi)
{
    if (isDir(path) != -1)
        return -ENOENT;

    if ((fi->flags & 3) != O_RDONLY)
        return -EACCES;

    return 0;
}

static int json_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    size_t len;
    (void) fi;
    if (isDir(path) != -1)
        return -ENOENT;

    len = strlen(files_content[isFile(path)]);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, files_content[isFile(path)] + offset, size);
    } else
        size = 0;

    return size;
}


static struct fuse_operations json_oper = {
    .getattr	= json_getattr,
    .readdir	= json_readdir,
    .open		= json_open,
    .read		= json_read,
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
	printf("MAIN: %d \n", pathfind("/", "/club"));
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
		if (pathfind("/", dir_list[i]) != -1)
			printf("dir: %s \n", dir_list[i]);
	}
	
	/*i = 0;
	for(i=0; i<curr_file_idx; i++) {
		printf("files_list: %s \n", files_list[i]);
		printf("files_content: %s \n", files_content[i]);
	}*/
	
	
    free(data);
    
    return fuse_main(argc, argv, &json_oper, NULL);
}


