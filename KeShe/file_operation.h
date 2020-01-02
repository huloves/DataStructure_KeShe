#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include "huffman.h"

int MyGetLine(int fd, char *buf);
void FileCharCount(char *fileName, int *times);
void TouchZipFile(char *filename, char *tarfilename, int *times);
void TouchUnZipFile(char *filename, char *untarfilename);
void TouchCodingFile(char *filename, char *codingfilename, int *times);
void SourceToCode(char *sourcefile, char *targetfile, huffman_code hc);
void SourceToCode_(char *sourcefile, char *targetfile, huffman_code hc);
void GetTimes(char *filename, int *times);
int GetSourceSize(char *filename);
int GetBit(int sourcefd);
char GetByte(int sourcefd);

#endif
