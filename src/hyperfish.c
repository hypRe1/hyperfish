#include "fenPositions.h"
#include "misc.h"
#include "perft.h"
#include "uci.h"
#include "bestmove.h"

// init attack tables
void init_all() {
    init_leapers_attacks();
    init_slider_attacks(0);
    init_slider_attacks(1);
    init_tables();
}

#define version "\x1B[36mv0.0.1\x1B[0m"
#define title "\x1B[32mhyperfish\x1B[0m"

int main() {
    printf("%s\n%s\n\n", title, version);
    init_all();

    int debug = 1;
    if (debug) {
        struct Board board;
        parse_position(&board, "fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
        print_board(&board, 0);
        search_position(&board, 5);

    } else {
        uci_loop();
    }

    return 0;
}
