#include "shared.h"
#include "huffman.h"
#include "encode.h"
#include "decode.h"

int main() {
    char** huffmanDictionary = getHuffmanDictionaryForFile("./../input");
//    printHuffmanDirectory(huffmanDictionary);

//    encode("./../input", "./../output", huffmanDictionary);
    decode("./../output", "./../decoded");

    freeHuffmanDictionary(huffmanDictionary);
    free(huffmanDictionary);

}
