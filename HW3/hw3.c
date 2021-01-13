#define FUSE_USE_VERSION 26

static const char* rofsVersion = "2008.09.24";

#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

void parse_array(cJSON *arrayItem)
{ 
	if(!arrayItem) {
		return;
		}
	char *key = arrayItem->string;
	if(key) {
		printf("key: %s \n", key);
		}
	char *value = arrayItem->valuestring;
	if(value) {
		printf("value: %s \n", value);
		}
	parse_array(arrayItem->child);
	parse_array(arrayItem->next);
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
	for(i = 0; i<keyCount; i++) { 
		cJSON *arrayItem = cJSON_GetArrayItem(json, i);
		parse_array(arrayItem);
		}	
    free(data);
	return 0;
}


