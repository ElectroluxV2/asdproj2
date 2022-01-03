#include "encode.h"
#include "decode.h"

int main(int argc, const char** argv) {
    if (argc < 4) {
        printf("Usage [dehumanise/humanise] [input] [output]\n");
        return 0;
    }

    if (strcmp(*(argv + 1), "dehumanise") == 0) {
        if (encode(*(argv + 2), *(argv + 3))) {
            printf("Dehumanised\n");
        } else {
            printf("Failed to dehumanise\n");
        }
    } else {
        if (decode(*(argv + 2), *(argv + 3))) {
            printf("Humanised\n");
        } else {
            printf("Failed to humanise\n");
        }
    }
}
