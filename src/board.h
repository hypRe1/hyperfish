#pragma once
#include <string.h>
#include "macros.h"
struct Board {
    U64 bitboards[12];  // Bitboards for each piece type
    U64 occupancies[3];  // Bitboards for occupancies: white, black, both
    int side;  // Side to move white = 0, black = 1
    int enpassant;  // Enpassant target square
    int castle;  // Castling rights using bit flags
};

enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

const char *square_to_coordinates[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", "no_sq"
};

enum { white, black, both };

enum { P, N, B, R, Q, K, p, n, b, r, q, k };

enum { wk = 1, wq = 2, bk = 4, bq = 8 };

const char ascii_pieces[] = "PNBRQKpnbrqk";

int char_pieces[] = {
    ['P'] = P,
    ['N'] = N,
    ['B'] = B,
    ['R'] = R,
    ['Q'] = Q,
    ['K'] = K,
    ['p'] = p,
    ['n'] = n,
    ['b'] = b,
    ['r'] = r,
    ['q'] = q,
    ['k'] = k
};

// Reset board to empty state
void reset_board(struct Board* board) {
    memset(board->bitboards, 0ULL, sizeof(board->bitboards));
    memset(board->occupancies, 0ULL, sizeof(board->occupancies));
    board->side = 0;
    board->enpassant = no_sq;
    board->castle = 0;
}

// Get piece type from square
int get_piece(struct Board* board, int square) {
    for (int piece = P; piece <= k; piece++) {
        if (get_bit(board->bitboards[piece], square)) return piece;
    }
    return -1;
}

#include "fen.h"

// Print a bitboard visualisation
void print_bitboard(U64 bitboard) {
    printf("\n");
    for (int rank = 0; rank < 8; rank++) {
        printf("%d   ", 8-rank);
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            printf("%d ", get_bit(bitboard, square) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n    a b c d e f g h\n");
    printf("\nBitboard: %llud\n\n", bitboard);
}

// Print the current board state with optional bitboard visualisation
void print_board(struct Board* board, int includeBitboards) {
    printf("\n");
    for (int rank = 0; rank < 8; rank++) {
        printf("%d   ", 8-rank);
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            int piece = get_piece(board, square);
            printf("%c ", (piece == -1) ? '.' : ascii_pieces[piece]);
        }
        if      (rank == 2) printf("    Side:        %s", (board->side) ? "black" : "white");
        else if (rank == 3) printf("    Enpassant:   %s", (board->enpassant != no_sq) ? square_to_coordinates[board->enpassant] : "no");
        else if (rank == 4) printf("    Castling:    %c %c %c %c", (board->castle & wk) ? 'K' : '-',
                                                                  (board->castle & wq) ? 'Q' : '-',
                                                                  (board->castle & bk) ? 'k' : '-',
                                                                  (board->castle & bq) ? 'q' : '-');
        else if (rank == 6) {
            char fen[100];
            export_fen(board, fen);
            printf("    Fen: %s", fen);
        }
        printf("\n");
    }
    printf("\n    a b c d e f g h\n\n");

    if (includeBitboards) {
        for (int piece = P; piece <= k; piece++) {
            printf("    %c bitboard:", ascii_pieces[piece]);
            print_bitboard(board->bitboards[piece]);
        }
        for (int colour = white; colour <= both; colour++) {
            printf("    Occupancy bitboard %c:", colour);
            print_bitboard(board->occupancies[colour]);
        }
    }
}
