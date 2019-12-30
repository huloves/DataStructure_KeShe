#include "zip.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "file_operation.h"
#include "huffman.h"
#include "my_error.h"

#define FILENAME_SIZE 256

void CompressFile(char *filename) {
    //统计文件FileName各个字符出现的频率
    int times[256] = {0};
    FileCharCount(filename, times);
    //建立哈弗曼树
    const int char_number = 256;
    hu_tree_node *huffman_tree;
    //huffman_code code;
    huffman_tree = (hu_tree_node*)malloc(sizeof(hu_tree_node)*(2*char_number-1));
    CreateHuffmanTree_Hash(huffman_tree, times, char_number);
    //建立哈弗曼编码
    huffman_code code;
    CreateHuffmanCode_Hash(huffman_tree, code, char_number);
    //创建压缩文件，写入压缩文件的头部，返回压缩文件名
    char *tarfilename;
    tarfilename = (char*)malloc(sizeof(char)*(strlen(filename)+256));
    TouchZipFile(filename, tarfilename, times);
    //源文件字符编码并写入目标文件
    SourceToCode(filename, tarfilename, code);
}

void UnZipFile(char *filename) {
    //创建目标文件
    char *untarfilename;
    untarfilename = (char*)malloc(sizeof(char)*(strlen(filename)+256));
    TouchUnZipFile(filename, untarfilename);
    printf("目标文件创建完成\n");
    //获取times和源文件大小
    int times[256] = {0};
    int source_size;
    GetTimes(filename, times);
    printf("获得times\n");
    source_size = GetSourceSize(filename);
    printf("获得sourcesize:%d\n", source_size);
    //建立哈弗曼树
    const int char_number = 256;
    hu_tree_node *huffman_tree;
    huffman_tree = (hu_tree_node*)malloc(sizeof(hu_tree_node)*(2*char_number-1));
    CreateHuffmanTree_Hash(huffman_tree, times, char_number);
    Translation(huffman_tree, times, char_number, filename, untarfilename, source_size);
}
