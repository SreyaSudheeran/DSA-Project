#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compression.h"


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
void writeBinary(FILE * output, int code){
	if (leftover > 0) {
	        int previousCode = (leftoverBits << 4) + (code >> 8);
        
        	fputc(previousCode, output);
        	fputc(code, output);
        
       		leftover = 0; // no leftover now
    	} 
    	else {
        	leftoverBits = code & 0xF; // save leftover, the last 00001111
        	leftover = 1;
        	
        	fputc(code >> 4, output);
	}
}


void compress(FILE *inputFile, FILE *outputFile){
	   
        int prefix = getc(inputFile);
    	if (prefix == EOF) {
        	return;
    	}
    	int character;

	int nextCode;
        int index;
    
   
    	nextCode = 256; 
    	Dinit();
    
    
    	while ((character = getc(inputFile)) != (unsigned)EOF) {
        
        
        	if ((index = lookup(prefix, character)) != -1) prefix = index;
        	else { 
        	    	writeBinary(outputFile, prefix);
        	    	if (nextCode < dictionarySize) Dict_Add(prefix, character, nextCode++);
        	    	prefix = character; 
        	}
    	}
    	writeBinary(outputFile, prefix);
    	if (leftover > 0) fputc(leftoverBits << 4, outputFile);
    
    // free the dictionary here
    	destroy();
}

