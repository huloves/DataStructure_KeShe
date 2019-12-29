#include "zip.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "file_operation.h"

void CompressFile(char *filename) {
    //统计文件FileName各个字符出现的频率
    int times[256];
    FileCharCount(filename, times);
}
