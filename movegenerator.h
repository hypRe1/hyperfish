#pragma once
#include "helpers.h"

// 8/pp1pp1pP/8/5p2/k4P2/2p1K1P1/PPPPP3/8 w - - 0 1
void quiet_pawn_moves_white(U64 bitboard, U64 empty) {
    U64 wPawnTargets = (bitboard >> 8) & empty;
    U64 wPawnTargets2 = ((wPawnTargets & rank3) >> 8) & empty;
    while (wPawnTargets) {
        int toSquare = get_ls1b_index(wPawnTargets);
        int fromSquare = toSquare + 8;
        if (toSquare < a7) {
            printf("%s%sq %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PROMOTION");
            printf("%s%sr %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PROMOTION");
            printf("%s%sb %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PROMOTION");
            printf("%s%sn %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PROMOTION");
        } else {
            printf("%s%s %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "SINGLE_PAWN_PUSH");
        }
        wPawnTargets &= wPawnTargets - 1;
    }
    while (wPawnTargets2) {
        int toSquare = get_ls1b_index(wPawnTargets2);
        int fromSquare = toSquare + 16;
        printf("%s%s %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "DOUBLE_PAWN_PUSH");
        wPawnTargets2 &= wPawnTargets2 - 1;
    }
}

void quiet_pawn_moves_black(U64 bitboard, U64 empty) {
    U64 wPawnTargets = (bitboard << 8) & empty;
    U64 wPawnTargets2 = ((wPawnTargets & rank6) << 8) & empty;
    while (wPawnTargets) {
        int toSquare = get_ls1b_index(wPawnTargets);
        int fromSquare = toSquare - 8;
        if (toSquare > h2) {
            printf("%s%sq %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PROMOTION");
            printf("%s%sr %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PROMOTION");
            printf("%s%sb %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PROMOTION");
            printf("%s%sn %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PROMOTION");
        } else {
            printf("%s%s %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "SINGLE_PAWN_PUSH");
        }
        wPawnTargets &= wPawnTargets - 1;
    }
    while (wPawnTargets2) {
        int toSquare = get_ls1b_index(wPawnTargets2);
        int fromSquare = toSquare - 16;
        printf("%s%s %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "DOUBLE_PAWN_PUSH");
        wPawnTargets2 &= wPawnTargets2 - 1;
    }
}

static inline void generate_moves(struct Board* board) {
    int source_square, target_square;
    U64 bitboard, attacks;

    for (int piece = P; piece <= k; piece++) {
        bitboard = board->bitboards[piece];
        switch (piece) {
            case (P): {
                printf("\n\nP quiet moves\n");
                quiet_pawn_moves_white(bitboard, ~(board->occupancies[both]));
                break;
            }
            case (p): {
                printf("\n\np quiet moves\n");
                quiet_pawn_moves_black(bitboard, ~(board->occupancies[both]));
            }
        }
    }

}
