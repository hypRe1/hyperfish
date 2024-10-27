#pragma once

#include "board.h"
#include "movegenerator.h"
#include "misc.h"

static inline U64 perft_driver(struct Board* board, int depth) {
    // base case for recursion
    if (depth == 0)
        return 1;

    struct Moves move_list;
    struct Board copy;
    U64 nodes = 0;
    generate_moves(board, &move_list);

    // iterate through pseudo-legal moves
    for (int move_count = 0; move_count < move_list.count; move_count++) {
        copy = *board;
        int move = move_list.moves[move_count];
        // if move is legal search for moves from this position decrementing depth
        if (make_move(&copy, move)) {
            nodes += perft_driver(&copy, depth - 1);
        }
    }
    return nodes;
}

// perft function with divided output
static inline void perft(struct Board* board, int depth) {
    U64 total = 0;
    U64 nodes;
    struct Board copy;
    struct Moves move_list;
    long long start = timeInMilliseconds();

    // iterate through pseudo-legal moves
    generate_moves(board, &move_list);
    for (int move_count = 0; move_count < move_list.count; move_count++) {
        copy = *board;
        int move = move_list.moves[move_count];
        // skip illegal moves
        if (!make_move(&copy, move))
            continue;

        // get number of nodes for legal moves
        nodes = perft_driver(&copy, depth - 1);

        // print move and number of nodes
        print_move(move);
        printf(": %llu\n", nodes);
        total += nodes;
    }
    long long time_taken = timeInMilliseconds() - start;
    printf("\nTime taken to execute: %llu ms\n", time_taken);
    printf("Nodes searched: %llu\n", total);
    printf("Nodes per second: %llu\n\n", (U64)(total/time_taken*1000));
}