#include "huffman.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
