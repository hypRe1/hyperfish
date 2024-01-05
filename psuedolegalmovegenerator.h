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

void castling_moves_white(struct Board* board) {
    if (board->castle & wk) {
        if ((board->occupancies[both] & 6917529027641081856ULL) == 0) {
            if (!(is_square_attacked(e1, black, board) || is_square_attacked(f1, black, board)))
                printf("e1g1 CASTLING_MOVE\n");
        }
    }
    
    if (board->castle & wq) {
        if ((board->occupancies[both] & 1008806316530991104ULL) == 0) {
            if (!(is_square_attacked(e1, black, board) || is_square_attacked(d1, black, board)))
                printf("e1c1 CASTLING_MOVE\n");
        }
    }
}

void castling_moves_black(struct Board* board) {
    if (board->castle & wk) {
        if ((board->occupancies[both] & 96ULL) == 0) {
            if (!(is_square_attacked(e8, white, board) || is_square_attacked(d8, white, board)))
                printf("e8g8 CASTLING_MOVE\n");
        }
    }
    
    if (board->castle & wq) {
        if ((board->occupancies[both] & 14ULL) == 0) {
            if (!(is_square_attacked(e8, white, board) || is_square_attacked(d8, black, board)))
                printf("e8c8 CASTLING_MOVE\n");
        }
    }
}

void generate_knight_moves(U64 bitboard, U64 enemyOrEmpty) {
    U64 nTargets;

    while (bitboard) {
        int fromSquare = get_ls1b_index(bitboard);
        bitboard &= bitboard - 1;

        nTargets = knight_attacks[fromSquare] & enemyOrEmpty;

        while (nTargets) {
            int toSquare = get_ls1b_index(nTargets);
            nTargets &= nTargets - 1;

            printf("%s%s %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "KNIGHT_MOVE");
        }
    }
}

void generate_king_moves(U64 bitboard, U64 enemyOrEmpty) {
    U64 kTargets;

    while (bitboard) {
        int fromSquare = get_ls1b_index(bitboard);
        bitboard &= bitboard - 1;

        kTargets = king_attacks[fromSquare] & enemyOrEmpty;

        while (kTargets) {
            int toSquare = get_ls1b_index(kTargets);
            kTargets &= kTargets - 1;

            printf("%s%s %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "KING_MOVE");
        }
    }
}


void generate_bishop_moves(U64 bitboard, U64 enemyOrEmpty, U64 occupancy) {
    U64 bTargets;

    while (bitboard) {
        int fromSquare = get_ls1b_index(bitboard);
        bitboard &= bitboard - 1;

        bTargets = get_bishop_attacks(fromSquare, occupancy) & enemyOrEmpty;

        while (bTargets) {
            int toSquare = get_ls1b_index(bTargets);
            bTargets &= bTargets - 1;

            printf("%s%s %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "BISHOP_MOVE");
        }
    }
}

void generate_rook_moves(U64 bitboard, U64 enemyOrEmpty, U64 occupancy) {
    U64 rTargets;

    while (bitboard) {
        int fromSquare = get_ls1b_index(bitboard);
        bitboard &= bitboard - 1;

        rTargets = get_rook_attacks(fromSquare, occupancy) & enemyOrEmpty;

        while (rTargets) {
            int toSquare = get_ls1b_index(rTargets);
            rTargets &= rTargets - 1;

            printf("%s%s %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "ROOK_MOVE");
        }
    }
}

void generate_queen_moves(U64 bitboard, U64 enemyOrEmpty, U64 occupancy) {
    // generate_bishop_moves(bitboard, enemyOrEmpty, occupancy);
    // generate_rook_moves(bitboard, enemyOrEmpty, occupancy);

    U64 qTargets;

    while (bitboard) {
        int fromSquare = get_ls1b_index(bitboard);
        bitboard &= bitboard - 1;

        qTargets = get_queen_attacks(fromSquare, occupancy) & enemyOrEmpty;

        while (qTargets) {
            int toSquare = get_ls1b_index(qTargets);
            qTargets &= qTargets - 1;

            printf("%s%s %s\n", square_to_coordinates[fromSquare], square_to_coordinates[toSquare], "QUEEN_MOVE");
        }
    }

}
 
static inline void generate_moves(struct Board* board) {
    int source_square, target_square;
    if (board->side == white) {
        quiet_pawn_moves_white(board->bitboards[P], ~(board->occupancies[both]));
        pawn_captures_white(board->bitboards[P], board->occupancies[black], board->enpassant);
        castling_moves_white(board);
        generate_knight_moves(board->bitboards[N], ~(board->occupancies[white]));
        generate_king_moves(board->bitboards[K], ~(board->occupancies[white]));
        generate_bishop_moves(board->bitboards[B], ~(board->occupancies[white]), board->occupancies[both]);
        generate_rook_moves(board->bitboards[R], ~(board->occupancies[white]), board->occupancies[both]);
        generate_queen_moves(board->bitboards[R], ~(board->occupancies[white]), board->occupancies[both]);
    } else if (board->side == black) {
        quiet_pawn_moves_black(board->bitboards[p], ~(board->occupancies[both]));
        pawn_captures_black(board->bitboards[p], board->occupancies[white], board->enpassant);
        castling_moves_black(board);
        generate_knight_moves(board->bitboards[n], ~(board->occupancies[black]));
        generate_king_moves(board->bitboards[k], ~(board->occupancies[black]));
        generate_rook_moves(board->bitboards[r], ~(board->occupancies[black]), board->occupancies[both]);
        generate_queen_moves(board->bitboards[r], ~(board->occupancies[black]), board->occupancies[both]);
    }
}
