#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "dcomp_dict_lzw.h"

int leftoverd = 0;
int leftoverBitsd;

void dcomp_dict_ArrayAdd(int prefix, int character, int value) {
        dictionaryArray[value].prefix = prefix;
        dictionaryArray[value].character = character;
}

int dcomp_dict_ArrayPrefix(int value) {
        return dictionaryArray[value].prefix;
}

int dcomp_dict_ArrayCharacter(int value) {
        return dictionaryArray[value].character;
}

int readBinary(FILE * input) {
	
        int code = fgetc(input);    
    	if (code == EOF) return 0;

    	if (leftoverd > 0) {
    		code = (leftoverBitsd << 8) + code;
        
        	leftoverd = 0;
    	} 
    	else {
        	int nextCode = fgetc(input);
        
       		leftoverBitsd = nextCode & 0xF; 
        	leftoverd = 1;
        
        	code = (code << 4) + (nextCode >> 4);
    	}
    	return code;
}

void decompress(FILE * inputFile, FILE * outputFile) {
        int previousCode; int currentCode;
        int nextCode = 256; 

    	int firstChar;
    
    
    	previousCode = readBinary(inputFile);
    	if (previousCode == 0) {
   	        return;
    	}
    	fputc(previousCode, outputFile);
   
   	while ((currentCode = readBinary(inputFile)) > 0) { 
    
        	if (currentCode >= nextCode) {
                    fputc(firstChar = decode(previousCode, outputFile), outputFile); 
                } 
        else firstChar = decode(currentCode, outputFile); 
        
        
        if (nextCode < dictionarySize) dcomp_dict_ArrayAdd(previousCode, firstChar, nextCode++);
        
      
        previousCode = currentCode;
    	}
  
}

int decode(int code, FILE * outputFile) {
    	int character; int temp;

    	if (code > 255) { 
        	character = dcomp_dict_ArrayCharacter(code);
        	temp = decode(dcomp_dict_ArrayPrefix(code), outputFile);
    	} 	
    	else {
        	character = code; 
        	temp = code;
    	}
    	fputc(character, outputFile);
    
    	return temp;
}
