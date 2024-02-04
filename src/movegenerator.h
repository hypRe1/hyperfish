#pragma once
#include "attacks.h"
#include "moveEncoding.h"

struct Moves {
    int moves[256];
    int count;
};

static inline void add_move(struct Moves* move_list, unsigned int move) {
    move_list->moves[move_list->count] = move;
    move_list->count++;
}

void print_move_list(struct Moves* move_list) {
    for (int move_count = 0; move_count < move_list->count; move_count++)
        print_move(move_list->moves[move_count]);
    printf("Total number of moves: %d\n", move_list->count);
}

// 8/pp1pp1pP/8/5p2/k4P2/2p1K1P1/PPPPP3/8 w - - 0 1
void quiet_pawn_moves_white(struct Moves* move_list, U64 bitboard, U64 empty) {
    U64 wPawnTargets = (bitboard >> 8) & empty;
    U64 wPawnTargets2 = ((wPawnTargets & rank3) >> 8) & empty;
    while (wPawnTargets) {
        int toSquare = lsb_index(wPawnTargets);
        int fromSquare = toSquare + 8;
        if (toSquare < a7) {
            add_move(move_list, encode_move(fromSquare, toSquare, P, 0, 1, 0, 0, 0));
            add_move(move_list, encode_move(fromSquare, toSquare, P, 0, 1, 0, 1, 0));
            add_move(move_list, encode_move(fromSquare, toSquare, P, 0, 1, 0, 2, 0));
            add_move(move_list, encode_move(fromSquare, toSquare, P, 0, 1, 0, 3, 0));
        } else {
            add_move(move_list, encode_move(fromSquare, toSquare, P, 0, 0, 0, 0, 0));
        }
        wPawnTargets &= wPawnTargets - 1;
    }
    while (wPawnTargets2) {
        int toSquare = lsb_index(wPawnTargets2);
        int fromSquare = toSquare + 16;
        add_move(move_list, encode_move(fromSquare, toSquare, P, 0, 0, 0, 1, 0));
        wPawnTargets2 &= wPawnTargets2 - 1;
    }
}

void quiet_pawn_moves_black(struct Moves* move_list, U64 bitboard, U64 empty) {
    U64 bPawnTargets = (bitboard << 8) & empty;
    U64 bPawnTargets2 = ((bPawnTargets & rank6) << 8) & empty;
    while (bPawnTargets) {
        int toSquare = lsb_index(bPawnTargets);
        int fromSquare = toSquare - 8;
        if (toSquare > h2) {
            add_move(move_list, encode_move(fromSquare, toSquare, P, 0, 1, 0, 0, 0));
            add_move(move_list, encode_move(fromSquare, toSquare, P, 0, 1, 0, 1, 0));
            add_move(move_list, encode_move(fromSquare, toSquare, P, 0, 1, 0, 2, 0));
            add_move(move_list, encode_move(fromSquare, toSquare, P, 0, 1, 0, 3, 0));
        } else {
            add_move(move_list, encode_move(fromSquare, toSquare, P, 0, 0, 0, 0, 0));
        }
        bPawnTargets &= bPawnTargets - 1;
    }
    while (bPawnTargets2) {
        int toSquare = lsb_index(bPawnTargets2);
        int fromSquare = toSquare - 16;
        add_move(move_list, encode_move(fromSquare, toSquare, P, 0, 0, 0, 1, 0));
        bPawnTargets2 &= bPawnTargets2 - 1;
    }
}

void pawn_captures_white(struct Moves* move_list, struct Board* board, U64 bitboard, U64 enemy, int enpassant) {
    U64 wPawnTargets;

    while (bitboard) {
        int fromSquare = lsb_index(bitboard);
        bitboard &= bitboard - 1;

        wPawnTargets = pawn_attacks[white][fromSquare] & enemy;

        if (enpassant != no_sq) {
            U64 wPawnEnpassant = pawn_attacks[white][fromSquare] & (1ULL << enpassant);
            if (wPawnEnpassant) {
                int toSquare = lsb_index(wPawnEnpassant);
                add_move(move_list, encode_move(fromSquare, toSquare, P, P, 0, 1, 1, 0));
            }
        }

        while (wPawnTargets) {
            int toSquare = lsb_index(wPawnTargets);
            int targetP = get_piece(board, toSquare)%6;
            wPawnTargets &= wPawnTargets - 1;
            if (toSquare < a7) {
                add_move(move_list, encode_move(fromSquare, toSquare, P, targetP, 1, 1, 0, 0));
                add_move(move_list, encode_move(fromSquare, toSquare, P, targetP, 1, 1, 1, 0));
                add_move(move_list, encode_move(fromSquare, toSquare, P, targetP, 1, 1, 2, 0));
                add_move(move_list, encode_move(fromSquare, toSquare, P, targetP, 1, 1, 3, 0));
            } else {
                add_move(move_list, encode_move(fromSquare, toSquare, P, targetP, 0, 1, 0, 0));
            }
        }
    }
}

void pawn_captures_black(struct Moves* move_list, struct Board* board, U64 bitboard, U64 enemy, int enpassant) {
    U64 bPawnTargets;

    while (bitboard) {
        int fromSquare = lsb_index(bitboard);
        bitboard &= bitboard - 1;

        bPawnTargets = pawn_attacks[black][fromSquare] & enemy;

        if (enpassant != no_sq) {
            U64 bPawnEnpassant = pawn_attacks[black][fromSquare] & (1ULL << enpassant);
            if (bPawnEnpassant) {
                int toSquare = lsb_index(bPawnEnpassant);
                add_move(move_list, encode_move(fromSquare, toSquare, P, P, 0, 1, 1, 0));
            }
        }

        while (bPawnTargets) {
            int toSquare = lsb_index(bPawnTargets);
            int targetP = get_piece(board, toSquare);
            bPawnTargets &= bPawnTargets - 1;
            if (toSquare > h2) {
                add_move(move_list, encode_move(fromSquare, toSquare, P, targetP, 1, 1, 0, 0));
                add_move(move_list, encode_move(fromSquare, toSquare, P, targetP, 1, 1, 1, 0));
                add_move(move_list, encode_move(fromSquare, toSquare, P, targetP, 1, 1, 2, 0));
                add_move(move_list, encode_move(fromSquare, toSquare, P, targetP, 1, 1, 3, 0));
            } else {
                add_move(move_list, encode_move(fromSquare, toSquare, P, targetP, 0, 1, 0, 0));
            }
        }
    }
}

void castling_moves_white(struct Moves* move_list, struct Board* board) {
    if (board->castle & wk) {
        if ((board->occupancies[both] & 6917529027641081856ULL) == 0) {
            if (!(is_square_attacked(board, e1, black) || is_square_attacked(board, f1, black)))
                add_move(move_list, encode_move(e1, g1, K, 0, 0, 0, 2, 0));
        }
    }

    if (board->castle & wq) {
        if ((board->occupancies[both] & 1008806316530991104ULL) == 0) {
            if (!(is_square_attacked(board, e1, black) || is_square_attacked(board, d1, black)))
                add_move(move_list, encode_move(e1, c1, K, 0, 0, 0, 3, 0));
        }
    }
}

void castling_moves_black(struct Moves* move_list, struct Board* board) {
    if (board->castle & bk) {
        if ((board->occupancies[both] & 96ULL) == 0) {
            if (!(is_square_attacked(board, e8, white) || is_square_attacked(board, f8, white)))
                add_move(move_list, encode_move(e8, g8, K, 0, 0, 0, 2, 0));
        }
    }

    if (board->castle & bq) {
        if ((board->occupancies[both] & 14ULL) == 0) {
            if (!(is_square_attacked(board, e8, white) || is_square_attacked(board, d8, white)))
                add_move(move_list, encode_move(e8, c8, K, 0, 0, 0, 3, 0));
        }
    }
}

void knight_quiet_moves(struct Moves* move_list, U64 bitboard, U64 empty) {
    U64 nTargets;

    while (bitboard) {
        int fromSquare = lsb_index(bitboard);
        bitboard &= bitboard - 1;

        nTargets = knight_attacks[fromSquare] & empty;

        while (nTargets) {
            int toSquare = lsb_index(nTargets);
            nTargets &= nTargets - 1;

            add_move(move_list, encode_move(fromSquare, toSquare, N, 0, 0, 0, 0, 0));
        }
    }
}

void knight_captures(struct Moves* move_list, struct Board* board, U64 bitboard, U64 enemy) {
    U64 nTargets;

    while (bitboard) {
        int fromSquare = lsb_index(bitboard);
        bitboard &= bitboard - 1;

        nTargets = knight_attacks[fromSquare] & enemy;

        while (nTargets) {
            int toSquare = lsb_index(nTargets);
            nTargets &= nTargets - 1;

            add_move(move_list, encode_move(fromSquare, toSquare, N, get_piece(board, toSquare)%6, 0, 1, 0, 0));
        }
    }
}

void king_quiet_moves(struct Moves* move_list, U64 bitboard, U64 empty) {
    U64 kTargets;

    while (bitboard) {
        int fromSquare = lsb_index(bitboard);
        bitboard &= bitboard - 1;

        kTargets = king_attacks[fromSquare] & empty;

        while (kTargets) {
            int toSquare = lsb_index(kTargets);
            kTargets &= kTargets - 1;

            add_move(move_list, encode_move(fromSquare, toSquare, K, 0, 0, 0, 0, 0));
        }
    }
}

void king_captures(struct Moves* move_list, struct Board* board, U64 bitboard, U64 enemy) {
    U64 kTargets;

    while (bitboard) {
        int fromSquare = lsb_index(bitboard);
        bitboard &= bitboard - 1;

        kTargets = king_attacks[fromSquare] & enemy;

        while (kTargets) {
            int toSquare = lsb_index(kTargets);
            kTargets &= kTargets - 1;

            add_move(move_list, encode_move(fromSquare, toSquare, K, get_piece(board, toSquare)%6, 0, 1, 0, 0));
        }
    }
}


void bishop_quiet_moves(struct Moves* move_list, U64 bitboard, U64 empty, U64 occupancy) {
    U64 bTargets;

    while (bitboard) {
        int fromSquare = lsb_index(bitboard);
        bitboard &= bitboard - 1;

        bTargets = get_bishop_attacks(fromSquare, occupancy) & empty;

        while (bTargets) {
            int toSquare = lsb_index(bTargets);
            bTargets &= bTargets - 1;

            add_move(move_list, encode_move(fromSquare, toSquare, B, 0, 0, 0, 0, 0));
        }
    }
}

void bishop_captures(struct Moves* move_list, struct Board* board, U64 bitboard, U64 enemy, U64 occupancy) {
    U64 bTargets;

    while (bitboard) {
        int fromSquare = lsb_index(bitboard);
        bitboard &= bitboard - 1;

        bTargets = get_bishop_attacks(fromSquare, occupancy) & enemy;

        while (bTargets) {
            int toSquare = lsb_index(bTargets);
            bTargets &= bTargets - 1;

            add_move(move_list, encode_move(fromSquare, toSquare, B, get_piece(board, toSquare)%6, 0, 1, 0, 0));
        }
    }
}

void rook_quiet_moves(struct Moves* move_list, U64 bitboard, U64 empty, U64 occupancy) {
    U64 rTargets;

    while (bitboard) {
        int fromSquare = lsb_index(bitboard);
        bitboard &= bitboard - 1;

        rTargets = get_rook_attacks(fromSquare, occupancy) & empty;

        while (rTargets) {
            int toSquare = lsb_index(rTargets);
            rTargets &= rTargets - 1;

            add_move(move_list, encode_move(fromSquare, toSquare, R, 0, 0, 0, 0, 0));
        }
    }
}

void rook_captures(struct Moves* move_list, struct Board* board, U64 bitboard, U64 enemy, U64 occupancy) {
    U64 rTargets;

    while (bitboard) {
        int fromSquare = lsb_index(bitboard);
        bitboard &= bitboard - 1;

        rTargets = get_rook_attacks(fromSquare, occupancy) & enemy;

        while (rTargets) {
            int toSquare = lsb_index(rTargets);
            rTargets &= rTargets - 1;

            add_move(move_list, encode_move(fromSquare, toSquare, R, get_piece(board, toSquare)%6, 0, 1, 0, 0));
        }
    }
}

void queen_quiet_moves(struct Moves* move_list, U64 bitboard, U64 empty, U64 occupancy) {
    U64 qTargets;

    while (bitboard) {
        int fromSquare = lsb_index(bitboard);
        bitboard &= bitboard - 1;

        qTargets = get_queen_attacks(fromSquare, occupancy) & empty;

        while (qTargets) {
            int toSquare = lsb_index(qTargets);
            qTargets &= qTargets - 1;

            add_move(move_list, encode_move(fromSquare, toSquare, Q, 0, 0, 0, 0, 0));
        }
    }

}

void queen_captures(struct Moves* move_list, struct Board* board, U64 bitboard, U64 enemy, U64 occupancy) {
    U64 qTargets;

    while (bitboard) {
        int fromSquare = lsb_index(bitboard);
        bitboard &= bitboard - 1;

        qTargets = get_queen_attacks(fromSquare, occupancy) & enemy;

        while (qTargets) {
            int toSquare = lsb_index(qTargets);
            qTargets &= qTargets - 1;

            add_move(move_list, encode_move(fromSquare, toSquare, Q, get_piece(board, toSquare)%6, 0, 1, 0, 0));
        }
    }

}

const unsigned short castling_rights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

int make_move(struct Board* board, int move) {
    int source = get_move_source(move);
    int target = get_move_target(move);
    int sourceP = get_move_sourcep(move);
    int isCapture = get_move_capture(move);
    int isPromotion = get_move_promotion(move);
    int moveSpecial = get_move_special(move);

    board->enpassant = no_sq;

    if (board->side == black) {
        sourceP += 6;
    }

    pop_bit(board->bitboards[sourceP], source);

    if (isPromotion) {
        sourceP = moveSpecial + 1;
        if (board->side == black) sourceP += 6;
    }

    set_bit(board->bitboards[sourceP], target);

    if (isCapture) {
        int captured = get_move_targetp(move);
        if (board->side == white) {
            captured += 6;
        }
        // print_move(move);
        if ((moveSpecial == 1) & (!isPromotion)) {
            (board->side == white) ? pop_bit(board->bitboards[captured], target+8) : pop_bit(board->bitboards[captured], target-8);
        } else {
            pop_bit(board->bitboards[captured], target);
        }

    } else if (!isPromotion) {
        switch (moveSpecial)
        {
        case 1:  // double pawn push (need to set enpassant square)
            if (board->side == white) {
                board->enpassant = source - 8;
            } else {
                board->enpassant = source + 8;
            }

            break;

        case 2: {  // king-side castling
            if (board->side == white) {
                pop_bit(board->bitboards[R], h1);
                set_bit(board->bitboards[R], f1);
            } else {
                pop_bit(board->bitboards[r], h8);
                set_bit(board->bitboards[r], f8);
            }
            break;
        }

        case 3: {  // queen-side castling
            if (board->side == white) {
                pop_bit(board->bitboards[R], a1);
                set_bit(board->bitboards[R], d1);
            } else {
                pop_bit(board->bitboards[r], a8);
                set_bit(board->bitboards[r], d8);
            }
            break;
        }


        default:
            break;
        }
    }

    board->castle &= castling_rights[source];
    board->castle &= castling_rights[target];

    memset(board->occupancies, 0ULL, sizeof(board->occupancies));
    board->occupancies[white] = board->bitboards[P] | board->bitboards[N] | board->bitboards[B] | board->bitboards[R] | board->bitboards[K] | board->bitboards[Q];
    board->occupancies[black] = board->bitboards[p] | board->bitboards[n] | board->bitboards[b] | board->bitboards[r] | board->bitboards[k] | board->bitboards[q];
    board->occupancies[both] = board->occupancies[white] | board->occupancies[black];

    int king_square = (board->side) ? lsb_index(board->bitboards[k]) : lsb_index(board->bitboards[K]);
    board->side = (board->side) ^ 1;

    if (is_square_attacked(board, king_square, board->side)) {
        // illegal move - take back
        return 0;
    }


    return 1;
}


static inline void generate_moves(struct Board* board, struct Moves* move_list) {
    int source_square, target_square;
    memset(move_list->moves, 0U, sizeof(move_list->moves));
    move_list->count = 0;
    if (board->side == white) {
        quiet_pawn_moves_white(move_list, board->bitboards[P], ~(board->occupancies[both]));
        pawn_captures_white(move_list, board, board->bitboards[P], board->occupancies[black], board->enpassant);
        castling_moves_white(move_list, board);
        knight_quiet_moves(move_list, board->bitboards[N], ~(board->occupancies[both]));
        knight_captures(move_list, board, board->bitboards[N], board->occupancies[black]);
        king_quiet_moves(move_list, board->bitboards[K], ~(board->occupancies[both]));
        king_captures(move_list, board, board->bitboards[K], board->occupancies[black]);
        bishop_quiet_moves(move_list, board->bitboards[B], ~(board->occupancies[both]), board->occupancies[both]);
        bishop_captures(move_list, board, board->bitboards[B], board->occupancies[black], board->occupancies[both]);
        rook_quiet_moves(move_list, board->bitboards[R], ~(board->occupancies[both]), board->occupancies[both]);
        rook_captures(move_list, board, board->bitboards[R], board->occupancies[black], board->occupancies[both]);
        queen_quiet_moves(move_list, board->bitboards[Q], ~(board->occupancies[both]), board->occupancies[both]);
        queen_captures(move_list, board, board->bitboards[Q], board->occupancies[black], board->occupancies[both]);
    } else if (board->side == black) {
        quiet_pawn_moves_black(move_list, board->bitboards[p], ~(board->occupancies[both]));
        pawn_captures_black(move_list, board, board->bitboards[p], board->occupancies[white], board->enpassant);
        castling_moves_black(move_list, board);
        knight_quiet_moves(move_list, board->bitboards[n], ~(board->occupancies[both]));
        knight_captures(move_list, board, board->bitboards[n], board->occupancies[white]);
        king_quiet_moves(move_list, board->bitboards[k], ~(board->occupancies[both]));
        king_captures(move_list, board, board->bitboards[k], board->occupancies[white]);
        bishop_quiet_moves(move_list, board->bitboards[b], ~(board->occupancies[both]), board->occupancies[both]);
        bishop_captures(move_list, board, board->bitboards[b], board->occupancies[white], board->occupancies[both]);
        rook_quiet_moves(move_list, board->bitboards[r], ~(board->occupancies[both]), board->occupancies[both]);
        rook_captures(move_list, board, board->bitboards[r], board->occupancies[white], board->occupancies[both]);
        queen_quiet_moves(move_list, board->bitboards[q], ~(board->occupancies[both]), board->occupancies[both]);
        queen_captures(move_list, board, board->bitboards[q], board->occupancies[white], board->occupancies[both]);
    }
}

static inline void generate_captures(struct Board* board, struct Moves* move_list) {
    int source_square, target_square;
    memset(move_list->moves, 0U, sizeof(move_list->moves));
    move_list->count = 0;
    if (board->side == white) {
        pawn_captures_white(move_list, board, board->bitboards[P], board->occupancies[black], board->enpassant);
        knight_captures(move_list, board, board->bitboards[N], board->occupancies[black]);
        king_captures(move_list, board, board->bitboards[K], board->occupancies[black]);
        bishop_captures(move_list, board, board->bitboards[B], board->occupancies[black], board->occupancies[both]);
        rook_captures(move_list, board, board->bitboards[R], board->occupancies[black], board->occupancies[both]);
        queen_captures(move_list, board, board->bitboards[Q], board->occupancies[black], board->occupancies[both]);
    } else if (board->side == black) {
        pawn_captures_black(move_list, board, board->bitboards[p], board->occupancies[white], board->enpassant);
        knight_captures(move_list, board, board->bitboards[n], board->occupancies[white]);
        king_captures(move_list, board, board->bitboards[k], board->occupancies[white]);
        bishop_captures(move_list, board, board->bitboards[b], board->occupancies[white], board->occupancies[both]);
        rook_captures(move_list, board, board->bitboards[r], board->occupancies[white], board->occupancies[both]);
        queen_captures(move_list, board, board->bitboards[q], board->occupancies[white], board->occupancies[both]);
    }
}
