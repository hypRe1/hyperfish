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

    // struct Board board;
    // parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPpP/R3K2R b KQkq - 0 1", &board);
    // // print_bitboard(board.occupancies[both]);
    // print_board(&board);

    // generate_moves(&board);

    print_bitboard(flipVertical(rank2));

    return 0;
}
