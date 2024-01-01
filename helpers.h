#pragma once
#include <stdio.h>

const char version[] = "v0.0.1";

const char title[] = " _                           __ _     _     \n\
| |__  _   _ _ __  _ __ ___ / _(_)___| |__  \n\
| '_ \\| | | | '_ \\| '__/ _ \\ |_| / __| '_ \\ \n\
| | | | |_| | |_) | | |  __/  _| \\__ \\ | | |\n\
|_| |_|\\__, | .__/|_|  \\___|_| |_|___/_| |_|\n\
       |___/|_|            \n\
      /`·.¸\n\
     /¸...¸`:·\n\
 ¸.·´  ¸   `·.¸.·´)\n\
: © ):´;      ¸  {\n\
 `·.¸ `·  ¸.·´\\`·¸)\n\
     `\\´´\\¸.·´\n\
\n";

typedef unsigned long long U64;

enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};

enum { white, black };

// File constants
const U64 not_a_file = 18374403900871474942ULL;
const U64 not_h_file = 9187201950435737471ULL;
const U64 not_hg_file = 4557430888798830399ULL;
const U64 not_ab_file = 18229723555195321596ULL;

// set/get/pop macros
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define pop_bit(bitboard, square) (bitboard &= ~(1ULL << square))

void print_bitboard(U64 bitboard) {
    printf("\n");
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            if (!file) printf("%d | ", 8-rank);
            printf("%d ", get_bit(bitboard, square) ? 1 : 0);
        }
        printf("\n");
    }
    printf("    - - - - - - - -\n");
    printf("    a b c d e f g h\n");
    printf("\nBitboard: %llud\n\n", bitboard);
}

// attacks

U64 mask_pawn_attacks(int side, int square) {
    U64 bitboard = 0ULL | (1ULL << square);
    if (!side) return 0ULL | ((bitboard >> 7) & not_a_file) | ((bitboard >> 9) & not_h_file);
    return 0ULL | ((bitboard << 7) & not_h_file) | ((bitboard << 9) & not_a_file);
}

U64 mask_knight_attacks(int square) {
    U64 bitboard = 0ULL | (1ULL << square);
    return 0ULL | ((bitboard >> 17) & not_h_file) 
                | ((bitboard >> 15) & not_a_file) 
                | ((bitboard >> 10) & not_hg_file) 
                | ((bitboard >> 6) & not_ab_file)

                | ((bitboard << 17) & not_a_file) 
                | ((bitboard << 15) & not_h_file) 
                | ((bitboard << 10) & not_ab_file) 
                | ((bitboard << 6) & not_hg_file);
}

U64 mask_king_attacks(int square) {
    U64 bitboard = 0ULL | (1ULL << square);
    return 0ULL | (bitboard >> 8)
                | ((bitboard >> 7) & not_a_file)
                | ((bitboard >> 9) & not_h_file)
                | ((bitboard >> 1) & not_h_file)
                | ((bitboard >> 7) & not_a_file)

                | (bitboard << 8)
                | ((bitboard << 7) & not_h_file)
                | ((bitboard << 9) & not_a_file)
                | ((bitboard << 1) & not_a_file)
                | ((bitboard << 7) & not_h_file);
}

U64 mask_bishop_attacks(int square) {
    U64 bitboard = 0ULL | (1ULL << square);
    U64 attacks = 0ULL;

    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 6 && f <= 6; r++, f++) attacks |= (1ULL << (r*8 + f));
    for (r = tr - 1, f = tf + 1; r >= 1 && f <= 6; r--, f++) attacks |= (1ULL << (r*8 + f));
    for (r = tr + 1, f = tf - 1; r <= 6 && f >= 1; r++, f--) attacks |= (1ULL << (r*8 + f));
    for (r = tr - 1, f = tf - 1; r >= 1 && f >= 1; r--, f--) attacks |= (1ULL << (r*8 + f));

    return attacks;
}

U64 mask_rook_attacks(int square) {
    U64 bitboard = 0ULL | (1ULL << square);
    U64 attacks = 0ULL;

    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1; r <= 6; r++) attacks |= (1ULL << (r*8 + tf));
    for (r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r*8 + tf));
    for (f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr*8 + f));
    for (f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr*8 + f));

    return attacks;
}
