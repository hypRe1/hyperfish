#pragma once

// 32 bit move encoding
// https://www.chessprogramming.org/Encoding_Moves Extended Move Structure
// source: 6 bits
// target: 6 bits
// sourcepiece: 3 bits
// targetpiece: 3 bits
// promotion: 1 bit
// capture: 1 bit
// special: 2 bits
// heuristc: 10 bits  (not used)

// source piece and target piece are included to increase move ordering efficiency when using MVV/LVA
// heuristic has no purpose as of now but could in future iterations

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
    ((source) | ((target) << 6) | ((sourcepiece) << 12) | ((targetpiece) << 15) | ((special) << 18) | \
    ((capture) << 20) | ((promotion) << 21) |  ((heuristic << 22)))

#define get_move_source(move)       ((move) & 0x3f)
#define get_move_target(move)       (((move) & 0xfc0) >> 6)
#define get_move_sourcep(move)      (((move) & 0x7000) >> 12)
#define get_move_targetp(move)      (((move) & 0x38000) >> 15)
#define get_move_special(move)      (((move) & 0xc0000) >> 18)
#define get_move_capture(move)      ((move) & 0x100000)
#define get_move_promotion(move)    ((move) & 0x200000)
#define get_move_code(move)         (((move) & 0x3c0000) >> 18)
#define get_move_heuristic(move)    (((move) & 0xffc00000) >> 22)

// Print move with more detail about captures and special moves for debugging
void print_move_detailed(unsigned int move) {
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

// Print move in uci format
void print_move(unsigned int move) {
    // UCI null move
    if (move == 0) {
        printf("0000");
        return;
    }

    int from = get_move_source(move);
    int to = get_move_target(move);
    int code = get_move_code(move);

    // printf("%u %u\n", move, code);
    printf("%s%s", square_to_coordinates[from], square_to_coordinates[to]);

    switch (code) {
        case (8): {   // Knight-promotion
            printf("n");
            break;
        }
        case (9): {   // Bishop-promotion
            printf("b");
            break;
        }
        case (10): {   // Rook-promotion
            printf("r");
            break;
        }
        case (11): {   // Queen-promotion
            printf("q");
            break;
        }
        case (12): {  // Knight-promotion capture
            printf("n");
            break;
        }
        case (13): {  // Bishop-promotion capture
            printf("b");
            break;
        }
        case (14): {  // Rook-promotion capture
            printf("r");
            break;
        }
        case (15): {  // Queen-promotion capture
            printf("q");
            break;
        }
    }
}
