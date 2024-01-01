#include "helpers.h"

int main() {
    printf("%s\n%s\n\n", title, version);
    U64 bitboard = 0ULL;
    set_bit(bitboard, e2);
    print_bitboard(bitboard);
    pop_bit(bitboard, e2);
    print_bitboard(bitboard);
    pop_bit(bitboard, e2);
    print_bitboard(bitboard);
    return 1;
}