#include "board.h"
#include "movegenerator.h"
#include "misc.h"

static inline U64 perft_driver(int depth, struct Board* board) {
    if (depth == 0) 
        return 1;

    struct Moves move_list;
    struct Board copy;
    U64 nodes = 0;
    generate_moves(board, &move_list);

    for (int move_count = 0; move_count < move_list.count; move_count++) {
        copy = *board;
        int move = move_list.moves[move_count];
        if (make_move(&copy, move)) {
            nodes += perft_driver(depth - 1, &copy);
        }

    }
    return nodes;
}

static inline void perft(int depth, struct Board* board) {
    U64 total = 0;
    U64 nodes;
    struct Board copy;
    struct Moves move_list;

    generate_moves(board, &move_list);
    for (int move_count = 0; move_count < move_list.count; move_count++) {
        copy = *board;
        int move = move_list.moves[move_count];
        if (!make_move(&copy, move))
            continue;

        nodes = perft_driver(depth - 1, &copy);
        print_move(move);
        printf(": %llu\n", nodes);
        total += nodes;
    }
    printf("\nNodes searched: %llu\n", total);
}