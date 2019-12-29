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
        ht[i].wegiht = w[i].weight;
        ht[i].parent = 0;
        ht[i].lchild = 0;
        ht[i].rchild = 0;
    }
    for(i=n; i<m; i++) {
        ht[i].wegiht = 0;
        ht[i].parent = 0;
        ht[i].lchild = 0;
        ht[i].rchild = 0;
    }
}

void CreateHuffmanTree(huffman_tree ht, weight *w, int n) {
    InitHuffmanTree(ht, w, n);
}
