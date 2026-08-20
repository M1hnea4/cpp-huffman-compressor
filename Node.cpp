#include "Node.h"

Node::Node(uint8_t s, uint64_t f) : symbol(s), frequency(f), left(nullptr), right(nullptr) {}

bool Node::is_leaf() const {
    return (left == nullptr && right == nullptr);
}

Node* node_join(Node* left, Node* right) {
    Node* parent = new Node('$', left->frequency + right->frequency);
    parent->left = left;
    parent->right = right;
    return parent;
}