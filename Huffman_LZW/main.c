#include "huffman.h"
#include "compression.h"
#include "decompression.h"
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

off_t get_file_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return st.st_size;
    } else {
        perror("stat failed");
        return -1;
    }
}

int main(int argc, char *argv[]) {

	/* CHECKING FLAG: "c" FOR COMPRESSION, "d" FOR DECOMPRESSION */
	if(strcmp(argv[1], "-c1") == 0){	
		unsigned char a = 0;
		int fd1, fd2, freq[128], i, size = 0, f;
		char arr[128];
		
		/* OPENING INPUT FILE IN READ_ONLY MODE */
		fd1 = open(argv[2], O_RDONLY);
		if(fd1 == -1){
			perror("Open Failed For Input File:\n");
			exit(1);
		}

		/* CREATING OUTPUT FILE IN WRITE MODE */
		fd2 = open(argv[3], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		if(fd2 == -1){
			perror("Open Failed For Output File:\n");
			exit(1);
		}
	
		/* INITIALIZING FREQUENCY ARRAY WITH ZERO VALUES AND CHARACTER ARRAY WITH CORRESPONDING ASCII CHARACTERS */
		for(i = 0; i < 128; i++){
			freq[i] = 0; 
			arr[i] = (char)i;	
		}
		    	
		/* FREQUENCY ARRAY FOR ALL INPUT FILE CHARACTERS */		
		f = frequency(fd1, freq);

		/* CALCULATING NUMBER OF UNIQUE CHARACTERS IN THE FILE */
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
		off_t source_size = get_file_size(argv[2]);
                off_t compressed_size = get_file_size(argv[3]);

                if (source_size == -1 || compressed_size == -1) {
                    fprintf(stderr, "Failed to get file sizes\n");
                    exit(1);
                }

                printf("Source file size: %ld bytes\n", source_size);
                printf("Compressed file size: %ld bytes\n", compressed_size);

                if (compressed_size >= source_size) {
                    printf("Compressed file is larger or equal to the source file. Deleting the compressed file...\n");
                    if (unlink(argv[3]) == 0) {
                        printf("Compressed file deleted successfully.\n");
                    } else {
                        perror("Failed to delete compressed file");
                    }
                } else {
                    printf("Compression successful. Compressed file retained.\n");
                }
	}
	else if(strcmp(argv[1], "-uc1") == 0){
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
		decompressfile(fd1, fd2, f);
		close(fd1);
		close(fd2);	
	}
	else if(strcmp(argv[1], "-c2") == 0) {
		FILE *inputFile;
		FILE *outputFile;
		inputFile = fopen(argv[2], "r"); 
		outputFile = fopen(strcat(argv[2], ".lzw"), "w+b"); 
            
		if (outputFile == NULL || inputFile == NULL) {
			printf("Can't open files\n'"); return 0;
		}
            
		compress(inputFile, outputFile);
	}
	else if(strcmp(argv[1], "-uc2") == 0) {
		FILE *inputFile;
		FILE *outputFile;
		inputFile = fopen(argv[2], "rb");

		char temp[20]; int length = strlen(argv[2])-4;
		strncpy(temp, argv[2], length);
		temp[length] = '\0';
		outputFile = fopen(temp, "w");
            
		if (outputFile == NULL || inputFile == NULL) {
			printf("Can't open files\n'"); return 0;
		}
		decompress(inputFile, outputFile);
	}
	return 0;	
}
