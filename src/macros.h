#pragma once

typedef unsigned long long U64;

#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

#if defined(__GNUC__) || defined(__GNUG__) || 1
#define count_bits(bitboard) (__builtin_popcountll(bitboard))
#else
// count bits within a bitboard (Brian Kernighan's way)
// removes lsb until empty
static inline int count_bits(U64 bitboard) {
    int count = 0;
    while (bitboard) {
        count++;
        bitboard &= bitboard - 1;  // remove lsb
    }
    return count;
}
#endif

// Least significant bit
// https://stackoverflow.com/questions/757059/position-of-least-significant-bit-that-is-set
#if defined(__GNUC__) || defined(__GNUG__) || 1
#define lsb_index(bitboard) (__builtin_ctzll(bitboard))
#else
#define lsb_index(bitboard) (count_bits((bitboard & -bitboard) - 1))
#endif
