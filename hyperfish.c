#include "attacks.h"
#include "board.h"
#include "movegenerator.h"
#include "helpers.h"

void init_all() {
    init_leapers_attacks();
    init_slider_attacks(0);
    init_slider_attacks(1);
}

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

int main() {
    printf("%s\n%s\n\n", title, version);

    init_all();

    struct Board board;
    struct Moves move_list;
    parse_fen("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", &board);
    print_board(&board);

    long long start = timeInMilliseconds();
    long nodes = 0;
    perft(2, &board);
    // generate_moves(&board, &move_list);
    // print_move_list(&move_list);
    long long end = timeInMilliseconds();
    printf("\nTime taken to execute %llu ms", end-start);
    
    return 0;
}

