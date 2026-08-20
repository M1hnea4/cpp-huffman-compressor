# 🗜️ Huffman Codec Engine

![C++](https://img.shields.io/badge/C%2B%2B-11%2F14-00599C?style=for-the-badge&logo=c%2B%2B)
![Status](https://img.shields.io/badge/Status-Completed-success?style=for-the-badge)

A high-performance, lossless data compression and decompression CLI tool implemented from scratch in C++. This project demonstrates low-level systems programming, utilizing the **Huffman Coding** algorithm, explicit bitwise operations, and custom binary serialization.

## 📖 Overview

Unlike standard academic implementations that stop at tree construction, this tool acts as a fully functional codec. It analyzes file entropy, generates optimal prefix codes, packs data at the **bit level** (bypassing standard byte boundaries), and securely serializes the Huffman tree metadata into a custom `.huf` binary format for independent decompression.

## 🧠 System Architecture

### 1. Compression Pipeline (`-c`)
1. **Frequency Analysis:** Reads the input file and builds a frequency histogram.
2. **Min-Heap Construction:** Uses a custom priority queue to build the Huffman Tree.
3. **Code Generation:** Assigns optimal binary prefix codes to each character.
4. **Tree Serialization:** Dumps the tree structure using a pre-order traversal sequence.
5. **Bit Packing:** Encodes the file by appending variable-length prefix codes into an 8-bit buffer.

### 2. Decompression Pipeline (`-d`)
1. **Header Parsing:** Extracts the tree size from the `.huf` file.
2. **Tree Deserialization:** Reads the metadata and reconstructs the exact Huffman Tree.
3. **Bit-stream Decoding:** Reads the compressed file byte-by-byte, unpacking individual bits to traverse the tree.

## 📦 Custom `.huf` File Format

| Offset | Size (Bytes) | Content Description |
| :--- | :--- | :--- |
| `0x00` | `4` | `tree_size` - The length of the serialized tree metadata. |
| `0x04` | `N` | **Serialized Tree** - Pre-order traversal dump (`'L'` for Leaf, `'I'` for Internal). |
| `0x04 + N` | `M` | **Compressed Bitstream** - The bit-packed encoded file data. |

## ⚙️ Algorithmic Complexity

* **Tree Construction:** O(N log N) where N is the number of unique symbols.
* **Code Generation:** O(N), a standard DFS traversal.
* **Memory Management:** Strictly managed with explicit destructors to prevent memory leaks.

## 💻 CLI Usage

1. Compress a file:
./huffman -c

2. Decompress a file:
./huffman -d

3. Inspect Huffman Codes:
./huffman -codes
