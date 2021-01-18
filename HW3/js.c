#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
    // printf("%s %s %c\n", path1,path2, path2[i1]);
    if(strcmp(path1, "/") != 0 && path2[i1] != '/') return -1; 
    
    for (i = i1; i < i2; i++) {
        if (path2[i] == '/') s++;
        if (s > 1) return -1;
        
        
    }
	if (strcmp(path1, "/") == 0) {
		if (s >= 1) return -1;
        return 1;
    }
    return i1+1;
}


char dir_list[ 256 ][ 256 ];
int visits[256];
int curr_dir_idx = 0;



char files_list[ 256 ][ 256 ];
int curr_file_idx = 0;
char files_content[ 256 ][ 256 ];


void dfs(FILE *f,char *path, int off, int tabs, int virgul) {
    int i = 0;
    int child_num = 0;
    int k = 0;
    int v = 0;
    for (i = 0; i < curr_dir_idx; i++) {
        if (pathfind(path, dir_list[i])!= -1) child_num++;
    }
    //printf("%s %d\n", path, child_num);
    for (i = 0; i < tabs; i++) fprintf(f, "\t");
    if (strcmp(path, "/") == 0) fprintf(f,"{\n");
    else fprintf(f,"\"%s\": {\n", path+off);

    for (i = 0; i < curr_dir_idx; i++) {
        if (strcmp(path, dir_list[i]) == 0) continue;
        if ((off=pathfind(path, dir_list[i])) != -1 && visits[i] == 0) {
            visits[i] = 1;
            k++;
            if (k < child_num) v = 1;
            else v = 0;
            dfs(f,dir_list[i], off, tabs+1, v);
        }
    }
    int file_ch_num = 0;
    for (i = 0; i < curr_file_idx; i++) {
        if (pathfind(path, files_list[i]) != -1) file_ch_num++;
    }
    k = 0;
    for (i = 0; i < curr_file_idx; i++) {
        int off = 0;
        if ((off=pathfind(path, files_list[i])) != -1) {
            for (int j = 0; j < tabs+1; j++) fprintf(f,"\t");
            k++;
            
            if (k < file_ch_num)
                fprintf(f, "\"%s\": %s,\n", files_list[i] + off, files_content[i]);
            else 
                fprintf(f, "\"%s\": %s\n", files_list[i] + off, files_content[i]);
        }
    }


    for (i = 0; i < tabs; i++) fprintf(f, "\t");
    if(virgul)
    fprintf(f,"},\n");
    else fprintf(f,"}\n");
}


void js(char *file_name) {
    FILE *f = fopen(file_name, "w");
    //fprintf(f, " \"asdsadsad\"\n");
    dfs(f, "/", 0,0, 0);
    fclose(f);
}

char *substr(char* dest, char *main_str, int bas, int son) {
    for (int i = bas; i < son; i++) {
        *dest = *(main_str+i);
        dest++;
    }
    *dest = '\0';
    return dest;
}


int main() {
    strcpy(dir_list[0], "/class");
    strcpy(dir_list[1], "/class/teacher");
    strcpy(dir_list[2], "/class/mudur");
    strcpy(dir_list[3], "/class/teacher/student");
    strcpy(dir_list[4], "/class/mudur/teacher");
    strcpy(files_list[0], "/class/teacher/student/student1");
    strcpy(files_list[1], "/class/teacher/student/student2");
    strcpy(files_content[0], "1111");
    strcpy(files_content[1], "222");
    strcpy(files_content[1], "\0");
    strcpy(files_list[1], "\0");
    curr_dir_idx = 5;
    curr_file_idx = 2;
    
    char data2[1000];
    js("example2.json");
    //dfs("/", 0, 0, 0);
    
    
    
    
    
    
    
    
    // FILE *f = NULL;
	// long len = 0;
	// char *data = NULL;
	// char *line = "student1";
	// f = fopen("example.json", "rb");
	// fseek(f, 0, SEEK_END);
    // len = ftell(f);
    // fseek(f, 0, SEEK_SET);
	// printf("MAIN: %d \n", pathfind("/", "/club"));
    // data = (char*)malloc(len + 1);

    // fread(data, 1, len, f);
    // data[len] = '\0';

    // char destin[30];
    // int bir_onceki = 0;
    // for (int i = 0; i <= len; i++) {
    //     if (data[i] == '\n') {
    //         substr(destin, data, bir_onceki,i);
    //         bir_onceki = i;
    //         printf("%s", destin);
    //     }
    // }
    // fclose(f);
}