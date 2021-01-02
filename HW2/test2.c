#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encryption.h"


char *key = "abcd";
int karray[4] = {1, 2, 3, 4};
int *key_array = karray;




int main() {
    
    
    
    
    char *word = "oneringtoruleemall";
    key = malloc(7*sizeof(char));
    memset(key, 0, 6*sizeof(char));
    key = "cefya";
    set_keyArray(&key_array, key);
    // printf("%s \n", key);
    // for (int i = 0; i < 5; i++) {
    //     printf("%d ", key_array[i]);
    // }
    char *res, *res2;
    enc(key_array, key, word, &res);
    dec(key_array, key, res, &res2);
    
    
    

    printf("%s %d \n", res2, len(res2));

    free(res2);
    free(res);
    free(key);
    free(key_array);
}