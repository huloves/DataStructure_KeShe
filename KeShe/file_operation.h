#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include "huffman.h"

int MyGetLine(int fd, char *buf);
void FileCharCount(char *fileName, int *times);
void TouchZipFile(char *filename, char *tarfilename, int *times);
void SourceToCode(char *sourcefile, char *targetfile, huffman_code hc);

#endif
