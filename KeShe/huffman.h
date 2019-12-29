#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct huffman {
    int wegiht;
    int parent, lchild, rchild;
}hu_tree_node, *huffman_tree;

typedef struct weight {
    unsigned char ch;
    int weight;
}weight;

typedef char **huffman_code;

void CreateWeight(weight *w, int *times);
void InitHuffmanTree(huffman_tree ht, weight *w, int n);
void CreateHuffmanTree(huffman_tree ht, weight *w, int n);

#endif
