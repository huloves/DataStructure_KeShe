#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct huffman {
    int weight;
    int parent, lchild, rchild;
}hu_tree_node, *huffman_tree;

typedef struct weight {
    unsigned char ch;
    int weight;
}weight;

typedef char *huffman_code[256];

void CreateWeight(weight *w, int *times);
void InitHuffmanTree(huffman_tree ht, weight *w, int n);
void InitHuffmanTree_Hash(huffman_tree ht, int *times, int char_number);
void CreateHuffmanTree(huffman_tree ht, weight *w, int n);
void CreateHuffmanTree_Hash(huffman_tree ht, int *times, int char_number);
void CreateHuffmanCode_Hash(huffman_tree ht, huffman_code hc, int char_number);

void select_first(huffman_tree ht, int n, int *s1, int *s2);

#endif
