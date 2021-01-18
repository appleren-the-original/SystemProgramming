#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
look if path1 is parent directory for path2

*/

int pathfind(char *path1, char *path2) {
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
    if (path1 == "/") 
    {   
        if (s >= 1) return -1;
        return 1;
    }
    return i1 + 1;
}


int main() {
    char *path1 = "/";
    char *path2 = "/class";
    char *path3 = "/club";
    char *path4 = "/class/teacher";
    char *path5 = "/club/teacher";
    printf("path1, path2: %d\n", pathfind(path2, path4));
    printf("path1, path3: %d\n", pathfind(path1, path3));
    printf("path1, path4: %d\n", pathfind(path1, path4));
    printf("path1, path5: %d\n", pathfind(path1, path5));
}   
