#include "attacks.h"
#include "board.h"
#include "movegenerator.h"
#include "helpers.h"

void init_all() {
    init_leapers_attacks();
    init_slider_attacks(0);
    init_slider_attacks(1);
}

int main() {
    printf("%s\n%s\n\n", title, version);

    init_all();

    struct Board board;
    struct Moves move_list;
    parse_fen(start_position, &board);
    print_board(&board);
    generate_moves(&board, &move_list);
    print_move_list(&move_list);

    // struct Board new;
    // int index;
    // int count = move_list.count;
    // for (index = 0; index < count; index++) {
    //     getchar();
    //     new = board;
    //     make_move(&new, move_list.moves[index]);
    //     print_board(&new);
    // }
    
    return 0;
}

