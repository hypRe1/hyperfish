#include "attacks.h"
#include "board.h"

void init_all() {
    init_leapers_attacks();
    init_slider_attacks(0);
    init_slider_attacks(1);
}

int main() {
    printf("%s\n%s\n\n", title, version);

    init_all();

    struct Board board;
    
    parse_fen(tricky_position, &board);
    // print_bitboard(board.occupancies[both]);
    print_board(&board);

    print_attacked_squares(white, &board);

    return 0;
}
