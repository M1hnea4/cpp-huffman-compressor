#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <map>
#include "Huffman.h"
#include "defines.h"

void generate_codes(Node* root, std::string code, std::map<uint8_t, std::string>& huffmanCodes) {
    if (!root) return;

    if (!root->left && !root->right) {
        huffmanCodes[root->symbol] = code;
    }

    generate_codes(root->left, code + "0", huffmanCodes);
    generate_codes(root->right, code + "1", huffmanCodes);
}

int main() {
    char option[15];
    std::string input_path, output_path;
    Huffman huff;

    std::cout << "Enter option (-h, -c, -d, -codes): ";
    if (!(std::cin >> option)) return 0;

    if (strcmp(option, "-h") == 0) {
        std::cout << "\n=== Huffman Compression Tool ===\n";
        std::cout << "  -h      : Show this help message\n";
        std::cout << "  -c      : Compress a text file\n";
        std::cout << "  -d      : Decompress a .huf file\n";
        std::cout << "  -codes  : Display the Huffman codes for a specific text file\n";
        std::cout << "================================\n";
    }
    else if (strcmp(option, "-codes") == 0) {
        std::cout << "Source file: "; std::cin >> input_path;
        std::ifstream in(input_path, std::ios::binary);
        if (!in) { std::cerr << "Error: File not found!\n"; return 1; }

        std::vector<uint8_t> content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();

        uint64_t hist[256] = {0};
        for (uint8_t b : content) hist[b]++;
        hist[0]++; hist[255]++;

        huff.construct_tree(hist);

        std::map<uint8_t, std::string> codes;
        generate_codes(huff.get_root(), "", codes);

        std::cout << "\n[Generated Huffman Codes]\n";
        for (auto const& pair : codes) {
            if (pair.first != 0 && pair.first != 255) {
                std::cout << "Symbol: '" << (char)pair.first << "' (ASCII: " << (int)pair.first << ") -> Code: " << pair.second << "\n";
            }
        }
    }
    else if (strcmp(option, "-c") == 0) {
        std::cout << "Source file: "; std::cin >> input_path;
        std::cout << "Output file: "; std::cin >> output_path;

        std::ifstream in(input_path, std::ios::binary);
        if (!in) { std::cerr << "Error: File not found!\n"; return 1; }

        std::vector<uint8_t> content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();

        uint64_t hist[256] = {0};
        for (uint8_t b : content) hist[b]++;
        hist[0]++; hist[255]++;

        huff.construct_tree(hist);
        std::ofstream out(output_path, std::ios::binary);

        uint32_t tree_size = 0;
        out.write(reinterpret_cast<char*>(&tree_size), sizeof(tree_size));

        long pos_before = out.tellp();
        huff.dump_tree(huff.get_root(), out);
        long pos_after = out.tellp();

        tree_size = pos_after - pos_before;
        out.seekp(0, std::ios::beg);
        out.write(reinterpret_cast<char*>(&tree_size), sizeof(tree_size));
        out.seekp(0, std::ios::end);

        std::map<uint8_t, std::string> codes;
        generate_codes(huff.get_root(), "", codes);

        uint8_t bit_buffer = 0;
        int bit_count = 0;

        for (uint8_t b : content) {
            std::string code = codes[b];
            for (char bit : code) {
                if (bit == '1') {
                    bit_buffer |= (1 << (7 - bit_count));
                }
                bit_count++;

                if (bit_count == 8) {
                    out.write(reinterpret_cast<char*>(&bit_buffer), 1);
                    bit_buffer = 0;
                    bit_count = 0;
                }
            }
        }

        if (bit_count > 0) {
            out.write(reinterpret_cast<char*>(&bit_buffer), 1);
        }

        out.close();
        std::cout << "Success! File encoded using bitwise compression.\n";
    }
    else if (strcmp(option, "-d") == 0) {
        std::cout << "Compressed file: "; std::cin >> input_path;
        std::ifstream in(input_path, std::ios::binary);
        if (!in) { std::cerr << "Error: File not found!\n"; return 1; }

        uint32_t tree_size = 0;
        in.read(reinterpret_cast<char*>(&tree_size), sizeof(tree_size));

        std::vector<uint8_t> tree_buffer(tree_size);
        in.read(reinterpret_cast<char*>(tree_buffer.data()), tree_size);

        huff.reconstruct_tree(tree_buffer.data(), tree_size);
        Node* root = huff.get_root();

        if (root) {
            std::vector<char> compressed_bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            in.close();

            std::cout << "\n[DECOMPRESSION SUCCESS]\nDecoded Text: ";

            Node* current = root;

            for (char c : compressed_bytes) {
                uint8_t byte = static_cast<uint8_t>(c);

                for (int i = 7; i >= 0; --i) {
                    bool bit = (byte >> i) & 1;

                    if (bit == 0 && current->left) current = current->left;
                    else if (bit == 1 && current->right) current = current->right;

                    if (!current->left && !current->right) {
                        if (current->symbol != 0 && current->symbol != 255) {
                            std::cout << (char)current->symbol;
                        }
                        current = root;
                    }
                }
            }
            std::cout << "\n--------------------------\n";
        } else {
            std::cout << "Error: Tree could not be reconstructed.\n";
            in.close();
        }
    }
    else {
        std::cout << "Invalid option. Type -h for help.\n";
    }

    return 0;
}