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
    int char_number = 0;
    hu_tree_node *huffman_tree;
    weight *w;
    huffman_code code;
    for(int i=0; i<256; i++) {
        if(times[i] != 0) {
            char_number++;
        }
    }
    huffman_tree = (hu_tree_node*)malloc(sizeof(hu_tree_node)*(2*char_number-1));
    w = (weight*)malloc(sizeof(weight)*char_number);
    (*code) = (char*)malloc(sizeof(char)*char_number);
    CreateWeight(w, times);
    CreateHuffmanTree(huffman_tree, w, char_number);
}
