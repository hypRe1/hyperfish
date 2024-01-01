#include "helpers.h"

U64 pawn_attacks[2][64];
U64 knight_attacks[64];
U64 king_attacks[64];


void init_leapers_attacks() {
    for (int square = 0; square < 64; square++) {
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);
        knight_attacks[square] = mask_knight_attacks(square);
        king_attacks[square] = mask_king_attacks(square);
    }
}

int main() {
    printf("%s\n%s\n\n", title, version);

    init_leapers_attacks();
    for (int square = 0; square < 64; square++)
        print_bitboard(mask_rook_attacks(square));

    // print_bitboard(mask_bishop_attacks(e4));

    return 1;
}
