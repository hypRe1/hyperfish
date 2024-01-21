#include "fenPositions.h"
#include "misc.h"
#include "perft.h"
// #include "magics/magicFinder.h"

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
    struct Board board;
    struct Moves move_list;
    parse_fen(start_position, &board);
    print_board(&board, 0);

    long long start = timeInMilliseconds();
    long nodes = 0;
    
    perft(5, &board);
    long long end = timeInMilliseconds();
    printf("\nTime taken to execute %llu ms", end-start);

    // print_magic_numbers();
    
    return 0;
}
