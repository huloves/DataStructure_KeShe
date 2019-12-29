#include "zip.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "file_operation.h"
#include "huffman.h"

void CompressFile(char *filename) {
    //统计文件FileName各个字符出现的频率
    int times[256] = {0};
    FileCharCount(filename, times);
    //建立哈弗曼树
    const int char_number = 256;
    hu_tree_node *huffman_tree;
    huffman_code code;
    huffman_tree = (hu_tree_node*)malloc(sizeof(hu_tree_node)*(2*char_number-1));
    (*code) = (char*)malloc(sizeof(char)*char_number);
    CreateHuffmanTree_Hash(huffman_tree, times, char_number);
}
