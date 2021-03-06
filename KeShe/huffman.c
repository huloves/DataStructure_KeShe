#include "huffman.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "file_operation.h"
#include "my_error.h"

void CreateWeight(weight *w, int *times) {
    for(int i=0; i<256; i++) {
        if(times[i] != 0) {
            w->ch = i;
            w->weight = times[i];
        }
    }
}

void InitHuffmanTree(huffman_tree ht, weight *w, int n) {
    int m = 2*n-1;
    int i;
    for(i=0; i<n; i++) {
        ht[i].weight = w[i].weight;
        ht[i].parent = 0;
        ht[i].lchild = 0;
        ht[i].rchild = 0;
    }
    for(i=n; i<m; i++) {
        ht[i].weight = 0;
        ht[i].parent = 0;
        ht[i].lchild = 0;
        ht[i].rchild = 0;
    }
}

void InitHuffmanTree_Hash(huffman_tree ht, int *times, int char_number) {
    int m = 2*char_number-1;
    int i;
    for(i=0; i<char_number; i++) {
        ht[i].weight = times[i];
        ht[i].parent = 0;
        ht[i].lchild = 0;
        ht[i].rchild = 0;
    }
    for(i=char_number; i<m; i++) {
        ht[i].weight = 0;
        ht[i].parent = 0;
        ht[i].lchild = 0;
        ht[i].rchild = 0;
    }

}

void CreateHuffmanTree(huffman_tree ht, weight *w, int n) {
    InitHuffmanTree(ht, w, n);
}

void CreateHuffmanTree_Hash(huffman_tree ht, int *times, int char_number) {
    InitHuffmanTree_Hash(ht, times, char_number);
    int m = 2*char_number-1;
    int s1, s2;
    for(int i = char_number; i<m; i++) {
        select_first(ht, i, &s1, &s2);
        ht[i].weight = ht[s1].weight+ht[s2].weight;
        ht[i].lchild = s1;
        ht[i].rchild = s2;
        ht[s1].parent = i;
        ht[s2].parent = i;
        //printf("%d %d-------%d %d\n", s1, s2, ht[s1].parent, ht[s2].parent);
    }
}

void select_first(huffman_tree ht, int n, int *s1, int *s2) {
    *s1 = 0;
    *s2 = 0;

    int min = 1000;
    for(int i=0; i<n; i++) {
        //printf("parent = %d | weight = %d\n", ht[i].parent, ht[i].weight);
        if(ht[i].parent == 0){
            if(ht[i].weight < min) {
                min = ht[i].weight;
                *s1 = i;
            }
        }
    }
    min = 1000;
    for(int i=0; i<n; i++) {
        if(i == *s1) {
            continue;
        }
        if(ht[i].parent == 0){
            if(ht[i].weight < min) {
                min = ht[i].weight;
                *s2 = i;
            }
        }
    }
    //printf("s1 = %d | s2 = %d\n", *s1, *s2);
}

void CreateHuffmanCode_Hash(huffman_tree ht, huffman_code hc, int char_number) {
    char *cd;
    int start;
    cd = (char*)malloc(sizeof(char)*char_number);
    cd[char_number-1] = '\0';
    for(int i=0; i<char_number; i++) {
        start = char_number-1;
        int c = i;
        int p = ht[i].parent;
        while(p != 0) {
            start--;
            if(ht[p].lchild == c) cd[start] = '0';
            else cd[start] = '1';
            c = p;
            p = ht[p].parent;
        }
        hc[i] = (char*)malloc(sizeof(char)*(char_number-start));
        strcpy(hc[i], &cd[start]);
    }
    free(cd);
}

void Translation(huffman_tree ht, int *times, int char_number, char *source_file, char *target_file, int source_size) {
    //打开压缩文件和目标文件
    int source_fd, target_fd;
    source_fd = open(source_file, O_RDONLY);
    if(source_fd == -1) {
        my_error("open", __LINE__-2);
        exit(1);
    }
    target_fd = open(target_file, O_WRONLY);
    if(target_fd == -1) {
        my_error("open", __LINE__-2);
        exit(1);
    }
    //压缩文件的文件指针指向第四行
    char buf[1024];
    for(int i=0; i<3; i++) {
        MyGetLine(source_fd, buf);
    }
    //读取编码，进行译码
    int m = 2*char_number-1;
    int x, c, j;
    for(x=0; x<source_size; x++) {
        j = m-1;
        while(ht[j].lchild != 0 || ht[j].rchild != 0) {
            if(GetBit(source_fd)) {
                j = ht[j].rchild;
            }
            else {
                j = ht[j].lchild;
            }
        }
        buf[0] = j;
        buf[1] = '\0';
        write(target_fd, buf, 1);
    }
}

void Translation_(huffman_tree ht, int *times, int char_number, char *source_file, char *target_file, int source_size) {
    //打开压缩文件和目标文件
    int source_fd, target_fd;
    source_fd = open(source_file, O_RDONLY);
    if(source_fd == -1) {
        my_error("open", __LINE__-2);
        exit(1);
    }
    target_fd = open(target_file, O_WRONLY);
    if(target_fd == -1) {
        my_error("open", __LINE__-2);
        exit(1);
    }
    //压缩文件的文件指针指向第四行
    char buf[1024];
    for(int i=0; i<3; i++) {
        MyGetLine(source_fd, buf);
    }
    //读取编码，进行译码
    int m = 2*char_number-1;
    int x, j;
    for(x=0; x<source_size; x++) {
        j = m-1;
        while(ht[j].lchild != 0 || ht[j].rchild != 0) {
            if(GetByte(source_fd) == '1') {
                j = ht[j].rchild;
            }
            else {
                j = ht[j].lchild;
            }
        }
        buf[0] = j;
        //printf("buf[0]:%d\n", buf[0]);
        write(target_fd, buf, 1);
    }
}

void PrintCode(huffman_code hc, int *times) {
    for(int i=0; i<256; i++) {
        if(times[i] != 0) {
            printf("%c:%s\n", i, hc[i]);
        }
    }
}

void PrintHuffmanTree(huffman_tree ht) {
    printf("number\t\tweight\t\t\tparent\t\t\tLchild\t\t\tRchild\n");
    for(int i=0; i<2*256-1; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\n", i, ht[i].weight, ht[i].parent, ht[i].lchild, ht[i].rchild);
    }
}

void PrintTree_TreeShape(huffman_tree ht, int xiabiao, int level) {
    if(ht[xiabiao].lchild == 0) {
        for(int i=0; i<level; i++) {
            printf("|");
            printf("     ");
        }
        printf("|--- ");
        printf("%d\n", xiabiao);
        PrintTree_TreeShape(ht, ht[xiabiao].lchild, level+1);
        PrintTree_TreeShape(ht, ht[xiabiao].rchild, level+1);
    }
}

