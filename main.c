#include "shared.h"
#include "encode.h"
#include "decode.h"

int main() {
    encode("./../input", "./../encoded");
    decode("./../encoded", "./../decoded");
}
