
#include "decompression.h"
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


int leftoverd = 0;
int leftoverBitsd;
dcomp_DictElement dictionaryArray[4095];

void ArrayAdd(int prefix, int character, int value) {
        dictionaryArray[value].prefix = prefix;
        dictionaryArray[value].character = character;
}

int dcomp_dict_ArrayPrefix(int value) {
        return dictionaryArray[value].prefix;
}

int dcomp_dict_ArrayCharacter(int value) {
        return dictionaryArray[value].character;
}

int readBinary(int fd1) {
	
        unsigned char buffer[2];
	ssize_t bytesRead = read(fd1, buffer, 1);
	if (bytesRead <= 0) return 0;

	int code = buffer[0];
    	if (leftoverd > 0) {
    		code = (leftoverBitsd << 8) + code;
        
        	leftoverd = 0;
    	} 
    	else {
        	bytesRead = read(fd1, &buffer[1], 1);
		if (bytesRead <= 0) return code;

		leftoverBitsd = buffer[1] & 0xF;
        	leftoverd = 1;
        
        	code = (code << 4) + (buffer[1] >> 4);
    	}
    	return code;
}

void decompress(int fd1, int fd2) {
        int previousCode; int currentCode;
        int nextCode = 256; 

    	int firstChar;
    
    
    	previousCode = readBinary(fd1);
    	if (previousCode == 0) {
   	        return;
    	}
    	write(fd2, &previousCode, 1);
   
   	while ((currentCode = readBinary(fd1)) > 0) { 
    
        	if (currentCode >= nextCode) {
			firstChar = decode(previousCode, fd2);
			write(fd2, &firstChar, 1); 
                } 
        else firstChar = decode(currentCode, fd2); 
        
        
        if (nextCode < dictionarySize) ArrayAdd(previousCode, firstChar, nextCode++);
        
      
        previousCode = currentCode;
    	}
  
}

int decode(int code, int fd2) {
    	int character; int temp;

    	if (code > 255) { 
        	character = dcomp_dict_ArrayCharacter(code);
        	temp = decode(dcomp_dict_ArrayPrefix(code), fd2);
    	} 	
    	else {
        	character = code; 
        	temp = code;
    	}
    	write(fd2, &character, 1);
    
    	return temp;
}
