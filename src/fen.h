#pragma once
#include "board.h"
#include "macros.h"
#include <stdio.h>

void parse_fen(struct Board* board, char *fen) {
    reset_board(board);
    int nSlashes = 0;
    for (int square = 0; square < 64 && *fen && *fen != ' '; ) {
        if ((*fen >= 'b' && *fen <= 'r') || (*fen >= 'B' && *fen <= 'R')) {
            int piece = char_pieces[*fen];
            set_bit(board->bitboards[piece], square);
            square++;
            fen++;
        } else if (*fen >= '1' && *fen <= '8') {
            int offset = *fen - '0';
            square += offset;
            fen++;
        } else if (*fen == '/'){
            fen++;
            nSlashes++;
        } else {
            printf("\x1B[31mInvalid character while parsing FEN >>> %s\x1B[0m\n", fen);
            reset_board(board);
            return;
        }
    }
    fen++;
    if (nSlashes != 7) {
            printf("\x1B[31mInvalid FEN: Does not have 7 slashes\x1B[0m\n");
            reset_board(board);
            return;
    }

    if      (*fen == 'w') board->side = white;
    else if (*fen == 'b') board->side = black;
    else {
        printf("\x1B[31mExpected w or b but got %c while parsing fen\x1B[0m\n", *fen);
        reset_board(board);
        return;
    }

    fen += 2;
    while (*fen != ' ') {
        switch (*fen) {
            case 'K': board->castle |= wk; break;
            case 'Q': board->castle |= wq; break;
            case 'k': board->castle |= bk; break;
            case 'q': board->castle |= bq; break;
            case '-': break;
        }
        fen++;
    }
    fen++;
    if (*fen != '-') {
        if (('a' <= fen[0]) && (fen[0] <= 'h') && ('0' <= fen[1]) && (fen[1] <= '8')) {
            int file = fen[0] - 'a';
            int rank = 8 - (fen[1] - '0');
            board->enpassant = rank*8+file;
        } else {
            printf("\x1B[31mFailed to parse enpassant of FEN\x1B[0m\n");
            reset_board(board);
            return;
        }
    }
    board->occupancies[white] = board->bitboards[P] | board->bitboards[N] | board->bitboards[B] | board->bitboards[R] | board->bitboards[K] | board->bitboards[Q];
    board->occupancies[black] = board->bitboards[p] | board->bitboards[n] | board->bitboards[b] | board->bitboards[r] | board->bitboards[k] | board->bitboards[q];
    board->occupancies[both] = board->occupancies[white] | board->occupancies[black];
}

void export_fen(struct Board* board, char* fen) {
    int index = 0;
    int nEmpty;

    for (int rank = 0; rank < 8; rank++) {
        int nEmpty = 0;
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            int piece = -1;

            for (int bb_piece = P; bb_piece <= k; bb_piece++) {
                if (get_bit(board->bitboards[bb_piece], square)) piece = bb_piece;
            }

            if (piece == -1) {
                nEmpty += 1;
            } else {
                if (nEmpty >= 1) {
                    fen[index] = nEmpty + '0';
                    nEmpty = 0;
                    index++;
                }
                fen[index] = ascii_pieces[piece];
                index++;
            }
        }
        if (nEmpty >= 1) {
            fen[index] = nEmpty + '0';
            index++;
        }

        if (rank != 7) {
            fen[index] = '/';
            index++;
        }
    }
    fen[index] = ' ';
    index++;
    fen[index] = (board->side) ? 'b' : 'w';
    index++;
    fen[index] = ' ';
    index++;
    int noRights = 1;

    if (board->castle & wk) {
        fen[index] = 'K';
        noRights = 0;
        index++;
    } if (board->castle & wq) {
        fen[index] = 'Q';
        noRights = 0;
        index++;
    } if (board->castle & bk) {
        fen[index] = 'k';
        noRights = 0;
        index++;
    } if (board->castle & bq) {
        fen[index] = 'q';
        noRights = 0;
        index++;
    }

    if (noRights) {
        fen[index] = '-';
        index++;
    }

    fen[index] = ' ';
    index++;

    if (board->enpassant == no_sq) {
        fen[index] = '-';
        index++;
    } else {
        fen[index] = square_to_coordinates[board->enpassant][0];
        index++;
        fen[index] = square_to_coordinates[board->enpassant][1];
        index++;
    }

    fen[index] = ' ';
    index++;

    fen[index] = '\0';
}
