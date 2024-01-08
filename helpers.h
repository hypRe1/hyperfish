#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"

// macros
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))
#define count_bits(bitboard) (__builtin_popcountll(bitboard))
// https://stackoverflow.com/questions/757059/position-of-least-significant-bit-that-is-set
// http://graphics.stanford.edu/%7Eseander/bithacks.html
#define get_ls1b_index(bitboard) (__builtin_ctzll(bitboard))

// https://stackoverflow.com/questions/41770887/cross-platform-definition-of-byteswap-uint64-and-byteswap-ulong
#if defined(_WIN32)
#define flipVertical(bitboard) (_byteswap_uint64(bitboard))
#elif defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define flipVertical(bitboard) (OSSwapInt64(bitboard))
#else
// https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating
U64 flipVertical(U64 bitboard) {
   const U64 k1 = 0x00FF00FF00FF00FFULL;
   const U64 k2 = 0x0000FFFF0000FFFFULL;
   bitboard = ((bitboard >>  8) & k1) | ((bitboard & k1) <<  8);
   bitboard = ((bitboard >> 16) & k2) | ((bitboard & k2) << 16);
   bitboard = ( bitboard >> 32)       | ( bitboard       << 32);
   return bitboard;
}
#endif

// 32 bit move encoding
// https://www.chessprogramming.org/Encoding_Moves Extended Move Structure
// source: 6 bits
// target: 6 bits
// sourcepiece: 3 bits
// targetpiece: 3 bits
// promotion: 1 bit
// capture: 1 bit
// special: 2 bits
// heuristc: 10 bits

// 0000 0000 0000 0000 0000 0000 0011 1111 source
// 0000 0000 0000 0000 0000 1111 1100 0000 target
// 0000 0000 0000 0000 0111 0000 0000 0000 sourcep
// 0000 0000 0000 0011 1000 0000 0000 0000 targetp
// 0000 0000 0000 1100 0000 0000 0000 0000 special
// 0000 0000 0001 0000 0000 0000 0000 0000 capture
// 0000 0000 0010 0000 0000 0000 0000 0000 promotion
// 0000 0000 0011 1100 0000 0000 0000 0000 code
// 1111 1111 1100 0000 0000 0000 0000 0000 heuristic

#define encode_move(source, target, sourcepiece, targetpiece, promotion, capture, special, heuristic) \
    ((source) | ((target) << 6) | ((sourcepiece) << 12) | ((targetpiece) << 15) | ((special) << 18) | ((capture) << 20) | ((promotion) << 21) |  ((heuristic << 22)))

#define get_move_source(move)       ((move) & 0x3f)
#define get_move_target(move)       (((move) & 0xfc0) >> 6)
#define get_move_sourcep(move)      (((move) & 0x7000) >> 12)
#define get_move_targetp(move)      (((move) & 0x38000) >> 15)
#define get_move_special(move)      (((move) & 0xc0000) >> 18)
#define get_move_capture(move)      ((move) & 0x100000)
#define get_move_promotion(move)    ((move) & 0x200000)
#define get_move_code(move)         (((move) & 0x3c0000) >> 18)
#define get_move_heuristic(move)    (((move) & 0xffc00000) >> 22)


// https://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
const char version[] = "\x1B[36mv0.0.1\x1B[0m";
const char title[] = "\x1B[32mhyperfish\x1B[0m";

typedef unsigned long long U64;

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

#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "

#define not_a_file (18374403900871474942ULL)
#define not_h_file (9187201950435737471ULL)
#define not_hg_file (4557430888798830399ULL)
#define not_ab_file (18229723555195321596ULL)

#define rank2 (71776119061217280ULL)
#define rank3 (280375465082880ULL)
#define rank6 (16711680)
#define rank7 (65280ULL)


// relevant occupancy bit count for every square on the board
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
        int square = get_ls1b_index(attack_mask);
        pop_bit(attack_mask, square);
        if (index & (1 << count)) occupancy |= (1ULL << square);
    }
    return occupancy;
}

void reset_board(struct Board* board) {
    memset(board->bitboards, 0ULL, sizeof(board->bitboards));
    memset(board->occupancies, 0ULL, sizeof(board->occupancies));
    board->side = 0;
    board->enpassant = no_sq;
    board->castle = 0;
}

void parse_fen(char *fen, struct Board* board) {
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

int get_piece(struct Board* board, int square) {
    for (int piece = P; piece <= k; piece++) {
        if (get_bit(board->bitboards[piece], square)) return piece;
    }
    return -1;
}

void print_board(struct Board* board) {
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
}

void print_board_flipped(struct Board* board) {
    printf("\n");
    for (int rank = 7; rank > -1; rank--) {
        printf("%d   ", 8-rank);
        for (int file = 7; file > -1; file--) {
            int square = rank * 8 + file;
            int piece = get_piece(board, square);
            printf("%c ", (piece == -1) ? '.' : ascii_pieces[piece]);
        }
        if      (rank == 5) printf("    Side:        %s", (board->side) ? "black" : "white");
        else if (rank == 4) printf("    Enpassant:   %s", (board->enpassant != no_sq) ? square_to_coordinates[board->enpassant] : "no");
        else if (rank == 3) printf("    Castling:    %c %c %c %c", (board->castle & wk) ? 'K' : '-',
                                                                  (board->castle & wq) ? 'Q' : '-',
                                                                  (board->castle & bk) ? 'k' : '-',
                                                                  (board->castle & bq) ? 'q' : '-');
        else if (rank == 1) {
            char fen[100];
            export_fen(board, fen);
            printf("    Fen: %s", fen);
        }
        printf("\n");
    }
    printf("\n    h g f e d c b a\n\n");
}

void print_move(unsigned int move) {
    int from = get_move_source(move);
    int to = get_move_target(move);
    int code = get_move_code(move);

    // printf("%u %u\n", move, code);
    printf("%s%s", square_to_coordinates[from], square_to_coordinates[to]);

    switch (code) {
        case (0): {  // Quiet move
            printf(" Quiet move %c\n", ascii_pieces[get_move_sourcep(move)]);
            break;
        }
        case (1): {   // Double pawn push
            printf(" Double pawn push\n");
            break;
        }
        case (2): {   // King castle
            printf(" Kingside castle\n");
            break;
        }
        case (3): {   // Queen castle
            printf(" Queenside castle\n");
            break;
        }
        case (4): {   // Captures
            printf(" %c captured %c\n", ascii_pieces[get_move_sourcep(move)], ascii_pieces[get_move_targetp(move)]);
            break;
        }
        case (5): {   // EP-capture
            printf(" Enpassant\n");
            break;
        }
        case (8): {   // Knight-promotion
            printf("k\n");
            break;
        }
        case (9): {   // Bishop-promotion
            printf("b\n");
            break;
        }
        case (10): {   // Rook-promotion
            printf("r\n");
            break;
        }
        case (11): {   // Queen-promotion
            printf("q\n");
            break;
        }
        case (12): {  // Knight-promotion capture
            printf("n Captured %c\n", ascii_pieces[get_move_targetp(move)]);
            break;
        }
        case (13): {  // Bishop-promotion capture
            printf("b Captured %c\n", ascii_pieces[get_move_targetp(move)]);
            break;
        }
        case (14): {  // Rook-promotion capture
            printf("r Captured %c\n", ascii_pieces[get_move_targetp(move)]);
            break;
        }
        case (15): {  // Queen-promotion capture
            printf("q Captured %c\n", ascii_pieces[get_move_targetp(move)]);
            break;
        }
    }
}
