#include "Huffman.h"
#include <queue>
#include <vector>
#include <stack>


Huffman::Huffman() : root(nullptr) {}


Huffman::~Huffman() {
    delete_tree(root);
}

void Huffman::delete_tree(Node* n) {
    if (n) {
        delete_tree(n->left);
        delete_tree(n->right);
        delete(n);
    }
}
void Huffman::construct_tree(uint64_t hist[]) {
    struct NodeComp {
        bool operator()(Node* l, Node* r) { return l->frequency > r->frequency; }
    };

    std::priority_queue<Node*, std::vector<Node*>, NodeComp> pq;

    for (int i = 0; i < ALPHABET; i++) {
        if (hist[i] > 0) {
            pq.push(new Node((uint8_t)i, hist[i]));
        }
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        pq.push(node_join(left, right));
    }

    if (!pq.empty()) {
        root = pq.top();
        Code c;
        build_codes(root, c);
    }
}

void Huffman::build_codes(Node* n, Code &c) {
    if (n) {
        if (n->is_leaf()) {
            table[n->symbol] = c;
        } else {
            c.push_bit(0);
            build_codes(n->left, c);
            uint8_t dummy; c.pop_bit(dummy);

            c.push_bit(1);
            build_codes(n->right, c);
            c.pop_bit(dummy);
        }
    }
}

void Huffman::dump_tree(Node* n, std::ofstream &out) {
    if (n) {
        if (n->is_leaf()) {
            out.put('L');
            out.put(n->symbol);
        } else {
            dump_tree(n->left, out);
            dump_tree(n->right, out);
            out.put('I');
        }
    }
}

void Huffman::reconstruct_tree(uint8_t* buffer, uint32_t size) {
    std::stack<Node*> s;

    for (uint32_t i = 0; i < size; ++i) {
        if (buffer[i] == 'L') {
            if (i + 1 < size) {
                uint8_t symbol = buffer[++i];
                Node* newNode = new Node(symbol, 0);
                newNode->left = newNode->right = nullptr;
                s.push(newNode);
            }
        }
        else if (buffer[i] == 'I') {
            if (s.size() >= 2) {
                Node* right = s.top(); s.pop();
                Node* left = s.top(); s.pop();

                Node* parent = new Node(0, 0);
                parent->left = left;
                parent->right = right;
                s.push(parent);
            }
        }
    }

    if (!s.empty()) {
        this->root = s.top();
    } else {
        this->root = nullptr;
    }
}