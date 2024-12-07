
#include "compression.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<math.h>



int leftover = 0;
int leftoverBits;
struct comp_DictNode *dictionary, *tail;

void Dinit(){
	int i;
    	struct comp_DictNode *node;
	for (i = 0; i < 256; i++) { 
        	node = (struct comp_DictNode *)malloc(sizeof(struct comp_DictNode));
        	node->prefix = -1;
        	node->character = i;
        	appendNode(node);
        }       
}
void appendNode(struct comp_DictNode *node){
	if (dictionary != NULL) tail->next = node;
        else dictionary = node;
        tail = node;
        node->next = NULL;
}
void destroy(){
	while (dictionary != NULL) {
	        struct comp_DictNode *node = dictionary;
        	dictionary = dictionary->next; 
        	free(node);
        }
}
int lookup(int prefix, int character){
	struct comp_DictNode *node;
        for (node = dictionary; node != NULL; node = node->next) { 
        	if (node->prefix == prefix && node->character == character) return node->value;
        }
        return -1;	
}
void Dict_Add(int prefix, int character, int value){
	struct comp_DictNode *node;
        node = (struct comp_DictNode *)malloc(sizeof(struct comp_DictNode));
    	node->value = value;
    	node->prefix = prefix;
    	node->character = character;
    	appendNode(node);
}
void writeBinary(int fd2, int code){
	if (leftover > 0) {
	        int previousCode = (leftoverBits << 4) + (code >> 8);
        
        	if (write(fd2, &previousCode, 1) != 1 || write(fd2, &code, 1) != 1) {
			perror("Error writing to output file");
			exit(EXIT_FAILURE);
		}
        
       		leftover = 0; // no leftover now
    	} 
    	else {
        	leftoverBits = code & 0xF; // save leftover, the last 00001111
        	leftover = 1;
        	
        	int highByte = code >> 4;
		if (write(fd2, &highByte, 1) != 1) {
			perror("Error writing to output file");
			exit(EXIT_FAILURE);
		}
	}
}


void compress(int fd1, int fd2){
	   
	unsigned char buffer;
	int prefix;

	if (read(fd1, &buffer, 1) != 1) {
		return; // File is empty
	}

	prefix = buffer;
    	int character;

	int nextCode;
        int index;
    
   
    	nextCode = 256; 
    	Dinit();
    
    
    	while (read(fd2, &buffer, 1) == 1) {
		character = buffer;

		if ((index = lookup(prefix, character)) != -1) {
		prefix = index;
		} else {
     		       writeBinary(fd2, prefix);
			if (nextCode < dictionarySize)
				Dict_Add(prefix, character, nextCode++);
			prefix = character;
		}
	}
    	writeBinary(fd2, prefix);
    	if (leftover > 0) {
		int leftoverByte = leftoverBits << 4;
		if (write(fd2, &leftoverByte, 1) != 1) {
			perror("Error writing to output file");
			exit(EXIT_FAILURE);
		}
	}
    
    // free the dictionary here
    	destroy();
}

