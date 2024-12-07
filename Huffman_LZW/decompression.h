#include<stdio.h>

typedef struct{
    int prefix; // prefix for byte > 255
    int character; // the last byte of the string
} dcomp_DictElement;

extern dcomp_DictElement dictionaryArray[4095];
#define dictionarySize 4095

void ArrayAdd(int prefix, int character, int value);
int dcomp_dict_ArrayPrefix(int value);
int dcomp_dict_ArrayCharacter(int value);
int readBinary(int fd1);
int decode(int code, int fd2);
void compress(int fd1, int fd2);
void decompress(int fd1, int fd2);

