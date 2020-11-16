// #define _GNU_SOURCE
#include <stdio.h>
#include <string.h>


int n_gram(char* str_1, int size_1, char* str_2, int size_2, int n);

int main(){
	int n, len=0, lens1, lens2;
	char s1[100], s2[100], *line;
	FILE* fp = fopen("input_tab.txt", "r");
	
	if (fp == NULL){
		printf("File cannot be opened.\n");
		return 1;
	}
	
	while (getline(&line, &len, fp) != -1){
		// printf("%s", line);
		sscanf(line, "%d %s %s", &n, s1, s2);
		lens1 = strlen(s1); lens2 = strlen(s2);
		
		int res = n_gram(s1, lens1, s2, lens2, n);
		printf(">similarity: %d\n", res);
	}
	
	fclose(fp);
	
	return 0;
}

