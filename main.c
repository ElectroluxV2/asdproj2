#include <stdlib.h>
#include "shared.h"
#include "heap.h"
#include "queue.h"
#include "occurrencesCount.h"
#include "huffman.h"

void addGroupTBD(group* array, const unsigned long long index, char* value, const unsigned long long count) {
    group created = {
            value,
            count
    };
    *(array + index) = created;
}

char* createDynamicStringTBD(const char* str) {
    char* dynamic = malloc(sizeof(str));
    strcpy(dynamic, str);
    return dynamic;
}

unsigned long long calculateSizeOfEncodedInput(char** dictionary, FILE* input) {
    unsigned long long size = 0;

    fseek(input, 0, SEEK_END);
    fseek(input, 0, SEEK_SET);

    int c;
    while ((c = fgetc(input)) != EOF) {
        unsigned int dictionaryIndex = 0;
        dictionaryIndex = c - 'a';
        size += strlen(*(dictionary + dictionaryIndex));
    }
    size += strlen(*(dictionary + 'z' - 'a' + 1));
    return size + (8 - size%8);
}

void tryToMatchStringWithDictionary(char** dictionary, const char* string, unsigned long long* index) {
    for (int i = 0; i < 'z' - 'a' + 1; i++) {
        if (*(dictionary + i) == string) {
            *index = i;
            return;
        }
    }
    index = NULL;
}

int main() {
    char** huffmanDictionary = getHuffmanDictionaryForFile("./../input");

    printHuffmanDirectory(huffmanDictionary);



    freeHuffmanDictionary(huffmanDictionary);
    free(huffmanDictionary);


//
//
//
//    FILE* file = fopen("./../input", "r");
//    unsigned long size = calculateSizeOfEncodedInput(dictionary, file);
//    //printf("Size: %lu\n", size);
//
//    char* binaryString = calloc(sizeof(char), size);
//
//    size_t n = 0;
//    fseek(file, 0, SEEK_END);
//    long f_size = ftell(file);
//    fseek(file, 0, SEEK_SET);
//    int c;
//    while ((c = fgetc(file)) != EOF) {
//        unsigned int dictionaryIndex = 0;
//        dictionaryIndex = c - 'a';
//        //printf("Character: %c byteValue: %s\n", c, *(dictionary + dictionaryIndex));
//        strcat(binaryString, *(dictionary + dictionaryIndex));
//    }
//    strcat(binaryString, *(dictionary + 'z' - 'a' + 1));
//
//
//
//    FILE* output = fopen("./../data.bin", "wb");
//    if (file == NULL) return 1;
//    for (int i = 0; i < strlen(binaryString); i+=8) {
//        char part[9];
//        strncpy(part, binaryString + i, 8);
//        part[8] = '\0';
//
//        unsigned char number = strtol(part, NULL, 2);
//
//        printf("%s: %d\n", part, number);
//        fputc(number, output);
//    }
//    fclose(output);
//
//    output = fopen("./../data.bin", "rb");
//    if (output == NULL) return 1;
//
//    // Trzeba zapisać w nagłówku ile będzie liczb po 8 bitów = rozmiar pliku
//    for (int i = 0; i < 30; i++) {
//        unsigned char number;
//        fread(&number, sizeof(unsigned char), 1, output);
//
//        for (int bit = 8; bit; --bit) {  // count from 8 to 1
//            putchar(number & (1 << (bit - 1)) ? '1' : '0');
//        }
//
//        printf(": %d\n", number);
//
//
//    }
//    fclose(output);
//
//    for (int i = 0; i < 'z' - 'a' + 1; i++) {
//        if (*(dictionary + i) != NULL) free(*(dictionary + i));
//    }
//
//    free(dictionary);
//    free(binaryString);
//    fclose(file);
}
