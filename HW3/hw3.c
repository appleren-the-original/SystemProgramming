#define FUSE_USE_VERSION 26

static const char* rofsVersion = "2008.09.24";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
// #include <fuse.h>
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
    return 0;
}


