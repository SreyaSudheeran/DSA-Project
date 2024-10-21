#include "huffman.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#define MAX 32
#define MAX_TREE_HT 100

int main(int argc, char *argv[]){

	// CHECKING PROPER NUMBER OF ARGUMENTS
	if(argc != 4){
		printf("Give Proper Arguments\n");
	}

	// CHECKING FLAG: "c" FOR COMPRESSION, "d" FOR DECOMPRESSION
	if(strcmp(argv[1], "c") == 0){	
		unsigned char a = 0;
		int fd1, fd2, freq[128], i, size = 0, f;
		char arr[128];
		
		// OPENING INPUT FILE IN READ_ONLY MODE
		fd1 = open(argv[2], O_RDONLY);
		if(fd1 == -1){
			perror("Open Failed For Input File:\n");
			exit(1);
		}

		// CREATING OUTPUT FILE IN WRITE MODE
		fd2 = open(argv[3], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		if(fd2 == -1){
			perror("Open Failed For Output File:\n");
			exit(1);
		}
	
		// INITIALIZING FREQUENCY ARRAY WITH ZERO VALUES AND CHARACTER ARRAY WITH CORRESPONDING ASCII CHARACTERS
		for(i = 0; i < 128; i++){
			freq[i] = 0; 
			arr[i] = (char)i;	
		}
		    	
		// FREQUENCY ARRAY FOR ALL INPUT FILE CHARACTERS		
		f = frequency(fd1, freq);

		// CALCULATING NUMBER OF UNIQUE CHARACTERS IN THE FILE
		for( i = 0; i < 127; i++){
			if(freq[i] != 0){
				size++;
			}
		}
		
		HuffmanCodes(fd2, arr, freq, size, f);
		close(fd1);
		fd1 = open(argv[2], O_RDONLY);
		if(fd1 == -1){
			perror("open failed:\n");
			exit(1);
	    	}
		compressfile(fd1, fd2, a);
		close(fd1);
		close(fd2);	
	}
	else if(strcmp(argv[1], "d") == 0){
		int size, fd1, fd2, f;
		fd1 = open(argv[2], O_RDONLY);
		if(fd1 == -1){
			perror("open failed:\n");
			exit(1);
	    	}
	    	fd2 = open(argv[3], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	    	if(fd2 == -1){
			perror("open failed:\n");
			exit(1);
		}
		read(fd1, &size, sizeof(int));
		read(fd1, &f, sizeof(int));
		AgainBuildHuffmanTree(fd1, size);
		tree_temp = tree;
		decompress(fd1, fd2, f);
		close(fd1);
		close(fd2);	
	}
	return 0;	
}
