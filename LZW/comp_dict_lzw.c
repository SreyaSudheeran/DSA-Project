#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comp_dict_lzw.h"


int leftover = 0;
int leftoverBits;

void comp_dict_Init(){
	int i;
    	struct comp_DictNode *node;
	for (i = 0; i < 256; i++) { 
        	node = (struct comp_DictNode *)malloc(sizeof(struct comp_DictNode));
        	node->prefix = -1;
        	node->character = i;
        	comp_appendNode(node);
        }       
}
void comp_appendNode(struct comp_DictNode *node){
	if (dictionary != NULL) tail->next = node;
        else dictionary = node;
        tail = node;
        node->next = NULL;
}
void comp_dict_Destroy(){
	while (dictionary != NULL) {
	        struct comp_DictNode *node = dictionary;
        	dictionary = dictionary->next; 
        	free(node);
        }
}
int comp_dict_Lookup(int prefix, int character){
	struct comp_DictNode *node;
        for (node = dictionary; node != NULL; node = node->next) { // ...traverse forward
        	if (node->prefix == prefix && node->character == character) return node->value;
        }
        return -1;	
}
void comp_dict_Add(int prefix, int character, int value){
	struct comp_DictNode *node;
        node = (struct comp_DictNode *)malloc(sizeof(struct comp_DictNode));
    	node->value = value;
    	node->prefix = prefix;
    	node->character = character;
    	comp_appendNode(node);
}
void comp_writeBinary(FILE * output, int code){
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
    	comp_dict_Init();
    
    
    	while ((character = getc(inputFile)) != (unsigned)EOF) {
        
        
        	if ((index = comp_dict_Lookup(prefix, character)) != -1) prefix = index;
        	else { 
        	    	comp_writeBinary(outputFile, prefix);
        	    	if (nextCode < dictionarySize) comp_dict_Add(prefix, character, nextCode++);
        	    	prefix = character; 
        	}
    	}
    	comp_writeBinary(outputFile, prefix);
    	if (leftover > 0) fputc(leftoverBits << 4, outputFile);
    
    // free the dictionary here
    	comp_dict_Destroy();
}

