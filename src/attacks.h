#pragma once
#include "magics/magicNumbers.h"
#include "macros.h"
#include "board.h"
#include <stdio.h>

#define not_a_file (18374403900871474942ULL)
#define not_h_file (9187201950435737471ULL)
#define not_hg_file (4557430888798830399ULL)
#define not_ab_file (18229723555195321596ULL)

#define rank2 (71776119061217280ULL)
#define rank3 (280375465082880ULL)
#define rank6 (16711680)
#define rank7 (65280ULL)

const int bishop_relevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6
};

const int rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12
};

U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask) {
    U64 occupancy = 0ULL;
    for (int count = 0; count < bits_in_mask; count++) {
        int square = lsb_index(attack_mask);
        pop_bit(attack_mask, square);
        if (index & (1 << count)) occupancy |= (1ULL << square);
    }
    return occupancy;
}

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

U64 bishop_attacks_on_the_fly(int square, U64 blockers) {
    U64 attacks = 0ULL;

    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1, f = tf + 1; r <= 7 && f <= 7; r++, f++) {
        attacks |= (1ULL << (r*8 + f));
        if ((1ULL << (r*8 + f)) & blockers) break;
    }
    for (r = tr - 1, f = tf + 1; r >= 0 && f <= 7; r--, f++) {
        attacks |= (1ULL << (r*8 + f));
        if ((1ULL << (r*8 + f)) & blockers) break;
    }
    for (r = tr + 1, f = tf - 1; r <= 7 && f >= 0; r++, f--) {
        attacks |= (1ULL << (r*8 + f));
        if ((1ULL << (r*8 + f)) & blockers) break;
    }
    for (r = tr - 1, f = tf - 1; r >= 0 && f >= 0; r--, f--) {
        attacks |= (1ULL << (r*8 + f));
        if ((1ULL << (r*8 + f)) & blockers) break;
    }

    return attacks;
}

U64 mask_rook_attacks(int square) {
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

U64 rook_attacks_on_the_fly(int square, U64 blockers) {
    U64 attacks = 0ULL;

    int r, f;
    int tr = square / 8;
    int tf = square % 8;

    for (r = tr + 1; r <= 7; r++) {
        attacks |= (1ULL << (r*8 + tf));
        if ((1ULL << (r*8 + tf)) & blockers) break;
    }
    for (r = tr - 1; r >= 0; r--) {
        attacks |= (1ULL << (r*8 + tf));
        if ((1ULL << (r*8 + tf)) & blockers) break;
    } 
    for (f = tf + 1; f <= 7; f++) {
        attacks |= (1ULL << (tr*8 + f));
        if ((1ULL << (tr*8 + f)) & blockers) break;
    }
    for (f = tf - 1; f >= 0; f--) {
        attacks |= (1ULL << (tr*8 + f));
        if ((1ULL << (tr*8 + f)) & blockers) break;
    }

    return attacks;
}

U64 pawn_attacks[2][64];
U64 knight_attacks[64];
U64 king_attacks[64];
U64 bishop_masks[64];
U64 rook_masks[64];
U64 bishop_attacks[64][512];
U64 rook_attacks[64][4096];

void init_leapers_attacks() {
    for (int square = 0; square < 64; square++) {
        pawn_attacks[white][square] = mask_pawn_attacks(white, square);
        pawn_attacks[black][square] = mask_pawn_attacks(black, square);
        knight_attacks[square] = mask_knight_attacks(square);
        king_attacks[square] = mask_king_attacks(square);
    }
}

void init_slider_attacks(int isBishop) {
    for (int square = 0; square < 64; square++) {
        bishop_masks[square] = mask_bishop_attacks(square);
        rook_masks[square] = mask_rook_attacks(square);

        U64 attack_mask = isBishop ? bishop_masks[square] : rook_masks[square];

        int relevant_bits_count = count_bits(attack_mask);
        int occupancy_indices = (1 << relevant_bits_count);

        for (int index = 0; index < occupancy_indices; index++) {
            U64 occupancy = set_occupancy(index, relevant_bits_count, attack_mask);
            if (isBishop) {
                int magic_index = (occupancy * bishop_magic_numbers[square]) >> (64-bishop_relevant_bits[square]);
                bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
            } else {
                int magic_index = (occupancy * rook_magic_numbers[square]) >> (64-rook_relevant_bits[square]);
                rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);
            }
        }
    }
}

static inline U64 get_bishop_attacks(int square, U64 occupancy) {
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= 64 - bishop_relevant_bits[square];
    return bishop_attacks[square][occupancy];
}

static inline U64 get_rook_attacks(int square, U64 occupancy) {
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= 64 - rook_relevant_bits[square];
    return rook_attacks[square][occupancy];
}

static inline U64 get_queen_attacks(int square, U64 occupancy) {
    return get_bishop_attacks(square, occupancy) | get_rook_attacks(square, occupancy);
}

static inline int is_square_attacked(int square, int side, struct Board* board) {
    if ((side == white) && (pawn_attacks[black][square] & board->bitboards[P])) return 1;
    if ((side == black) && (pawn_attacks[white][square] & board->bitboards[p])) return 1;
    if (knight_attacks[square] & ((side == white) ? board->bitboards[N] : board->bitboards[n])) return 1;
    if (get_bishop_attacks(square, board->occupancies[both]) & ((side == white) ? (board->bitboards[B] | board->bitboards[Q]) : (board->bitboards[b] | board->bitboards[q]))) return 1;
    if (get_rook_attacks(square, board->occupancies[both]) & ((side == white) ? (board->bitboards[R] | board->bitboards[Q]) : (board->bitboards[r] | board->bitboards[q]))) return 1;
    if (king_attacks[square] & ((side == white) ? board->bitboards[K] : board->bitboards[k])) return 1;
    return 0;
}
