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
    parse_fen(start_position, &board);
    print_board(&board);
    generate_moves(&board);

    return 0;
}
