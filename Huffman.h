#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "Node.h"
#include "Code.h"
#include <fstream>

class Huffman {
private:
    Node* root;
    Code table[ALPHABET];
    void build_codes(Node* n, Code &c);
    void delete_tree(Node* n);

public:
    Huffman();
    ~Huffman();
    void construct_tree(uint64_t hist[]);
    void dump_tree(Node* n, std::ofstream &out);
    void reconstruct_tree(uint8_t* tree_dump, uint32_t size);
    Code get_code(uint8_t s) const { return table[s]; }
    Node* get_root() const { return root; }
};

#endif