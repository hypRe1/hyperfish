#include "attacks.h"
#include "board.h"
#include "movegenerator.h"
#include "helpers.h"

void init_all() {
    init_leapers_attacks();
    init_slider_attacks(0);
    init_slider_attacks(1);
}

long nodes;

static inline long perft_driver(int depth, struct Board* board) {
    long nodes;

    if (depth == 0) {
        return nodes+1;
    }
    
    struct Board old = *board;
    struct Moves move_list;
    generate_moves(board, &move_list);
    
    for (int move_count = 0; move_count < move_list.count; move_count++) {   
        
        
        if (!make_move(&old, move_list.moves[move_count]))
            continue;
        
        nodes += perft_driver(depth - 1, board);

        board = &old;
    }

    return nodes;
}

int main() {
    printf("%s\n%s\n\n", title, version);

    init_all();

    struct Board board;
    struct Moves move_list;
    parse_fen(tricky_position, &board);
    print_board(&board);

    long long start = timeInMilliseconds();
    generate_moves(&board, &move_list);
    
    print_move_list(&move_list);

    // struct Board new;
    // int index, valid;
    // int count = move_list.count;
    // for (index = 0; index < count; index++) {
    //     getchar();
    //     new = board;
    //     valid = make_move(&new, move_list.moves[index]);
    //     if (valid) print_board(&new);
    // }

    long long end = timeInMilliseconds();

    printf("\nTime taken to execute %llu ms", end-start);
    
    return 0;
}

