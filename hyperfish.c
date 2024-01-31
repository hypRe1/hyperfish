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
    uci_loop();
    
    return 0;
}