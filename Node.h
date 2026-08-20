#ifndef NODE_H
#define NODE_H

#include <stdint.h>


class Node {
public:
    uint8_t symbol;
    uint64_t frequency;
    Node *left, *right;

    Node(uint8_t s, uint64_t f);
    bool is_leaf() const;
};

Node* node_join(Node* left, Node* right);

#endif