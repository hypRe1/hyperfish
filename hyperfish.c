#include "fenPositions.h"
#include "misc.h"
#include "perft.h"
#include "uci.h"

// init attack tables
void init_all() {
    init_leapers_attacks();
    init_slider_attacks(0);
    init_slider_attacks(1);
}

#define version "\x1B[36mv0.0.1\x1B[0m"
#define title "\x1B[32mhyperfish\x1B[0m"

int main() {
    printf("%s\n%s\n\n", title, version);
    init_all();

    int debug = 0;
    if (debug) {
        int score;
        struct Board board;
        parse_position("position fen rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", &board);
        score = evaluate(&board);
        printf("%d\n", score);
        
    } else {
        uci_loop();
    }

    return 0;
}