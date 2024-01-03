#pragma once
#include "helpers.h"
#include "attacks.h"

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
    U64 bPawnTargets = (bitboard << 8) & empty;
    U64 bPawnTargets2 = ((bPawnTargets & rank6) << 8) & empty;
    while (bPawnTargets) {
        int toSquare = get_ls1b_index(bPawnTargets);
        int fromSquare = toSquare - 8;
        if (toSquare > h2) {
            printf("%s%sq %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PROMOTION");
            printf("%s%sr %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PROMOTION");
            printf("%s%sb %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PROMOTION");
            printf("%s%sn %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PROMOTION");
        } else {
            printf("%s%s %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "SINGLE_PAWN_PUSH");
        }
        bPawnTargets &= bPawnTargets - 1;
    }
    while (bPawnTargets2) {
        int toSquare = get_ls1b_index(bPawnTargets2);
        int fromSquare = toSquare - 16;
        printf("%s%s %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "DOUBLE_PAWN_PUSH");
        bPawnTargets2 &= bPawnTargets2 - 1;
    }
}

void pawn_captures_white(U64 bitboard, U64 enemy, int enpassant) {
    U64 wPawnTargets;

    set_bit(enemy, enpassant);

    while (bitboard) {
        int fromSquare = get_ls1b_index(bitboard);
        bitboard &= bitboard - 1;

        wPawnTargets = pawn_attacks[white][fromSquare] & enemy;
        while (wPawnTargets) {
            int toSquare = get_ls1b_index(wPawnTargets);
            wPawnTargets &= wPawnTargets - 1;
            if (toSquare < a7) {
                printf("%s%sq %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PAWN_CAPTURE_PROMOTION");
                printf("%s%sr %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PAWN_CAPTURE_PROMOTION");
                printf("%s%sb %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PAWN_CAPTURE_PROMOTION");
                printf("%s%sn %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PAWN_CAPTURE_PROMOTION");
            } else {
                printf("%s%s %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PAWN_CAPTURE");
            }
        }
    }
}

void pawn_captures_black(U64 bitboard, U64 enemy, int enpassant) {
    U64 bPawnTargets;

    set_bit(enemy, enpassant);

    while (bitboard) {
        int fromSquare = get_ls1b_index(bitboard);
        bitboard &= bitboard - 1;

        bPawnTargets = pawn_attacks[black][fromSquare] & enemy;
        while (bPawnTargets) {
            int toSquare = get_ls1b_index(bPawnTargets);
            bPawnTargets &= bPawnTargets - 1;
            if (toSquare > h2) {
                printf("%s%sq %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PAWN_CAPTURE_PROMOTION");
                printf("%s%sr %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PAWN_CAPTURE_PROMOTION");
                printf("%s%sb %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PAWN_CAPTURE_PROMOTION");
                printf("%s%sn %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PAWN_CAPTURE_PROMOTION");
            } else {
                printf("%s%s %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "PAWN_CAPTURE");
            }
        }
    }
}

static inline void generate_moves(struct Board* board) {
    int source_square, target_square;
    if (board->side == white) {
        quiet_pawn_moves_white(board->bitboards[P], ~(board->occupancies[both]));
        pawn_captures_white(board->bitboards[P], board->occupancies[black], board->enpassant);
    } else if (board->side == black) {
        quiet_pawn_moves_black(board->bitboards[p], ~(board->occupancies[both]));
        pawn_captures_black(board->bitboards[p], board->occupancies[white], board->enpassant);
    }
}
