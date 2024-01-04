#pragma once
#include "magics.h"
#include "helpers.h"
#include "random.h"

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

U64 bishop_attacks_on_the_fly(int square, U64 blockers) {
    U64 bitboard = 0ULL | (1ULL << square);
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

U64 rook_attacks_on_the_fly(int square, U64 blockers) {
    U64 bitboard = 0ULL | (1ULL << square);
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

// generate candidate for magic number
U64 generate_magic_number() {
    return get_random_U64_number() & get_random_U64_number() & get_random_U64_number();
}

// find magic numbers through brute force
U64 find_magic_number(int square, int relevant_bits, int isBishop) {
    U64 occupancies[4096];

    // init attack tables
    U64 attacks[4096];

    U64 used_attacks[4096];

    U64 attack_mask = isBishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

    int occupancy_indices = 1 << relevant_bits;

    for (int index = 0; index < occupancy_indices; index++) {
        occupancies[index] = set_occupancy(index, relevant_bits, attack_mask);
        attacks[index] = isBishop ? bishop_attacks_on_the_fly(square, occupancies[index]) :
                                    rook_attacks_on_the_fly(square, occupancies[index]);
    }

    // test magic numbers
    for (int count; count < 100000000; count++) {
        U64 magic_number = generate_magic_number();

        // skip inappropriate magic numbers
        if (count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;

        memset(used_attacks, 0ULL, sizeof(used_attacks));

        int index, fail;

        // test magic index loop
        for (index = 0, fail = 0; (!fail) && (index < occupancy_indices); index++) {
            int magic_index = (int)((occupancies[index] * magic_number) >> (64 - relevant_bits));

            if (used_attacks[magic_index] == 0ULL)
                // init used attacks
                used_attacks[magic_index] = attacks[index];

            else if (used_attacks[magic_index] != attacks[index])
                // magic index does not work
                fail = 1;
        }

        if (!fail)
            return magic_number;
    }

    printf("Failed to find magic number\nImpossible (*_*)");
    return 0ULL;
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

// print attacked squares
void print_attacked_squares(int side, struct Board* board) {
    printf("\n");
    for (int rank = 0; rank < 8; rank++) {
        printf("%d │ ", 8-rank);
        for (int file = 0; file < 8; file++){
            int square = rank * 8 + file;
            printf("%d ", is_square_attacked(square, side, board) ? 1 : 0);
        }
        printf("\n");
    }
    
    printf("  ╰────────────────\n");
    printf("    a b c d e f g h\n\n");
}
