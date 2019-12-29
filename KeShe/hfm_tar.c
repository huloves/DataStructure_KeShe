#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "zip.h"

int main(int argc, char *argv[])
{
    if(argc < 3) {
        printf("./MingLing [-option] FileName\n");
        printf("-option: -c, -u\n");
        printf("-c: 压缩文件\n");
        printf("-u: 解压文件\n");
        exit(1);
    }

    char option = argv[1][1];
    switch(option) {
    case 'c':
        CompressFile(argv[2]);
    }

    return 0;
}
