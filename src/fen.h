#pragma once
#include "board.h"
#include "macros.h"
#include <stdio.h>

void parse_fen(struct Board* board, char *fen) {
    reset_board(board);
    int nSlashes = 0;

    // iterate over squares
    for (int square = 0; square < 64 && *fen && *fen != ' '; ) {
        // check for pieces
        if ((*fen >= 'b' && *fen <= 'r') || (*fen >= 'B' && *fen <= 'R')) {
            int piece = char_pieces[*fen];
            set_bit(board->bitboards[piece], square);
            square++;
            fen++;
        // check for spaces
        } else if (*fen >= '1' && *fen <= '8') {
            int offset = *fen - '0';
            square += offset;
            fen++;
        // check for slash indicating next rank
        } else if (*fen == '/'){
            fen++;
            nSlashes++;
        // if character is not one of the above then invalid
        } else {
            printf("\x1B[31mInvalid character while parsing FEN >>> %s\x1B[0m\n", fen);
            reset_board(board);
            return;
        }
    }
    fen++;
    // valid FEN should have 7 slashes
    if (nSlashes != 7) {
            printf("\x1B[31mInvalid FEN: Does not have 7 slashes\x1B[0m\n");
            reset_board(board);
            return;
    }

    // check side to play
    if      (*fen == 'w') board->side = white;
    else if (*fen == 'b') board->side = black;
    else {
        printf("\x1B[31mExpected w or b but got %c while parsing fen\x1B[0m\n", *fen);
        reset_board(board);
        return;
    }

    // check castling rights
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

    // check en passant target square
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

    // set board occupancies bitboards
    board->occupancies[white] = board->bitboards[P] | board->bitboards[N] | board->bitboards[B] | board->bitboards[R] | board->bitboards[K] | board->bitboards[Q];
    board->occupancies[black] = board->bitboards[p] | board->bitboards[n] | board->bitboards[b] | board->bitboards[r] | board->bitboards[k] | board->bitboards[q];
    board->occupancies[both] = board->occupancies[white] | board->occupancies[black];
}

void export_fen(struct Board* board, char* fen) {
    int index = 0;
    int nEmpty;

    // iterate over ranks
    for (int rank = 0; rank < 8; rank++) {
        int nEmpty = 0;
        // iterate over files
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            int piece = -1;

            // get piece occupying square -1 otherwise
            for (int bb_piece = P; bb_piece <= k; bb_piece++) {
                if (get_bit(board->bitboards[bb_piece], square)) piece = bb_piece;
            }

            if (piece == -1) {
                nEmpty += 1;
            } else {
                // add number of empty squares prior to piece if there are any
                if (nEmpty >= 1) {
                    fen[index] = nEmpty + '0';
                    nEmpty = 0;
                    index++;
                }
                // add ascii piece
                fen[index] = ascii_pieces[piece];
                index++;
            }
        }
        // add empty squares if there are any
        if (nEmpty >= 1) {
            fen[index] = nEmpty + '0';
            index++;
        }

        // add slash if not last rank
        if (rank != 7) {
            fen[index] = '/';
            index++;
        }
    }

    // add colour to move
    fen[index] = ' ';
    index++;
    fen[index] = (board->side) ? 'b' : 'w';
    index++;

    // add castling rights
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

    // add en passant target square
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

    // end of fen character array
    fen[index] = '\0';
}
