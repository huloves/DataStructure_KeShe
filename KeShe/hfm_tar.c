#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "zip.h"

int main(int argc, char *argv[])
{
    if(argc != 3) {
        printf("./MingLing [-option] FileName\n");
        printf("-option: -c, -u\n");
        printf("-c: 压缩\n");
        printf("-u: 解压\n");
        printf("-o: 编码\n");
        printf("-d: 解码\n");
        exit(1);
    }

    char option = argv[1][1];
    switch(option) {
    case 'c':
        CompressFile(argv[2]);
        break;
    case 'u':
        UnZipFile(argv[2]);
        break;
    case 'o':
        CodingFile(argv[2]);
        break;
    case 'd':
        DecodeFile(argv[2]);
        break;
    default:
        printf("选项错误\n");
        break;
    }

    return 0;
}
