#define ALPHABET "abcdefghijklmnopqrstuvwxyz"



int len(char* word) {
	int res = 0;
	int i = 0;
	for(i = 0; word[i] != '\0'; i++) {
		res++;
	}
	return res;
}

void set_keyArray(int **key_array, char* key) {
	int newl_ = len(key);
	int alp_l = len(ALPHABET);
	int index = 1;
	int i = 0;
	for (i = 0; i < alp_l; i++) {
		int j = 0;
		for (j = 0; j < newl_; j++) {
			if(ALPHABET[i] == key[j]) {
				*(*key_array+j) = index++;
			}
		}
	}
	(*key_array) = kmalloc(newl_*sizeof(int), GFP_KERNEL);
	memset(*key_array, 0, newl_*sizeof(int));
		
}



int enc(int* key_array, char* key, char* word, char** res) {

	int key_l = len(key);
	int word_l = len(word);
	

	int pad_num = (key_l - word_l%key_l)%key_l;
	// printf("%d word length \n", word_l);
	// printf("%d pad_nun \n", pad_num);
	

	int times = (word_l + pad_num)/key_l;
	int i = 0;
	(*res) = kmalloc((word_l + pad_num + 1)*sizeof(char), GFP_KERNEL);
	for(i = 0; i < times; i++){
		int j = 0;
		for(j = 0; j < key_l; j++) {
			char harf = 'x';
			int ind = i*key_l + j;
			int ind_res = i*key_l + key_array[j]-1;
			if (ind_res >= word_l) {
				harf = '0';
			}
			else {
				harf = word[ind_res];
			}
			*(*res+ind) = harf;
			
		}
	}
	//(*res) = kmalloc((word_l + pad_num + 1)*sizeof(char), GFP_KERNEL);
	*(*res + (word_l + pad_num)) = '\0';

	// printf("%d leng res\n", len((*res)));
	return pad_num;
}

void dec(int* key_array, char* key, char* word, char** res){
	int word_l = len(word);
	
	int key_l = len(key);
	int times = word_l/key_l;
	int i = 0;
	(*res) = kmalloc((word_l+1)*sizeof(char), GFP_KERNEL);
	for(i = 0; i < times; i++) {
		int j = 0;
		for(j = 0; j < key_l; j++){
			int ind = i*key_l + j;
			int ind_res = i*key_l + key_array[j] - 1;
			*(*res+ind_res) = word[ind];
		}
	}
	
	*(*res + word_l) = '\0';
	
	return ;
}



















//
// int key_len;
// int* key_array;
// int* key_encrypt;
// int* key_decrypt;
//
//
// int compare(const void* val1, const void* val2){
// 	return key_array[*(int*)val1] - key_array[*(int*)val2];
// }
//
// /**
//  * This function sets the global key arrays' values.
//  */
// void setKeys(char* key){
// 	key_len = strlen(key);
// 	int index[key_len];
//
// 	key_array = malloc(key_len * sizeof(int));
// 	for(int i=0; i<key_len; i++)
// 		key_array[i] = key[i];
//
// 	for(int i=0; i<key_len; i++)
// 		index[i] = i;
// 	qsort(index, key_len, sizeof(int), compare);
//
// 	key_encrypt = malloc(key_len * sizeof(int));
// 	key_decrypt = malloc(key_len * sizeof(int));
// 	for(int i=0; i<key_len; i++){
// 		key_encrypt[index[i]] = i;
// 		key_decrypt[i] = index[i];
// 	}
// }
//
//
//
// /**
//  * Function for printing 2D arrays
//  */
// void printmatrix(char** matrix, int nrows, int ncols){
// 	for(int i=0; i<nrows; i++){
// 		for(int j=0; j<ncols; j++){
// 			printf("%c ", matrix[i][j]);
// 		}
// 		printf("\n");
// 	}
// 	printf("\n");
// }
//


// /**
//  * @param msg	string to be encrypted
//  * @param mode	boolean value, 1: encrypt & 0: decrypt
//  * 				only used keys differ
//  * @return 		resulting encrypted / decrypted string
//  * */
 void encrypt(int* key_array, int key_len, char* msg, int mode, char** result){
 	int i, j;
 	char** matrix;
 	char* msg_result;
 	
 	int slen = len(msg);
 	int ncols = key_len; // previously set by setKeys function call
 	int nrows = slen / ncols;
 	if (slen % ncols != 0) nrows++;
	
 	// create a matrix for storing the message
 	matrix = kmalloc(sizeof(char*) * nrows, GFP_KERNEL);
 	for(i=0; i<nrows; i++)
 		matrix[i] = kmalloc(sizeof(char) * ncols, GFP_KERNEL);

 	// build initial matrix with the message
 	for(i=0; i<nrows; i++){
 		for(j=0; j<ncols; j++){
 			if (i*ncols + j >= slen)
 				matrix[i][j] = '0';
 			else
 				matrix[i][j] = msg[i*ncols + j];
 		}
 	}
 	// printmatrix(matrix, nrows, ncols); // DEBUG

 	// create encrypted message using initial matrix and encryption key
 	msg_result = kmalloc(nrows * ncols, GFP_KERNEL);
 	if (mode){ // encryption
 		for(i=0; i<nrows; i++){
 			for(j=0; j<ncols; j++){
 				msg_result[i*ncols + j] = matrix[i][key_array[j]];
 			}
 		}
 	}
 	else{ // decryption
 		int break_flag = 0;
 		for(i=0; i<nrows; i++){
 			for(j=0; j<ncols; j++){
 				if (matrix[i][key_array[j]] == '0'){
 					break_flag = 1;
 					break;
 				}
 				msg_result[i*ncols + j] = matrix[i][key_array[j]];
 			}
 			if (break_flag) break;
 		}
 	}

 	// free the memory allocated for the matrix
 	for(i=0; i<nrows; i++)
 		kfree(matrix[i]);
 	kfree(matrix);

	*result = msg_result;
 	return;
}



/*int main(int argc, char* argv[]){
	char* msg = "oneringtorulethemall";
	char* key = "ceayf";
	setKeys(key);
	char* enc = encrypt(msg, 1);
	char* dec = encrypt(enc, 0);
	printf("%s\n", enc);
	printf("%s\n", dec);

	char* msg2 = "alperensystem";
	char* key2 = "ceayf";
	setKeys(key2);
	char* enc2 = encrypt(msg2, 1);
	char* dec2 = encrypt(enc2, 0);
	printf("%s\n", enc2);
	printf("%s\n", dec2);


	return 0;
}*/
