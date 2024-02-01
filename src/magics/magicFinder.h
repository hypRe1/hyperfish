#pragma once

#include "../macros.h"
#include "../attacks.h"
#include "../prng.h"
#include <stdio.h>

U64 generate_magic_number() {
    return get_random_U64_number() & get_random_U64_number() & get_random_U64_number();
}

U64 find_magic_number(int square, int relevant_bits, int isBishop) {
    U64 occupancies[4096];
    U64 attacks[4096];
    U64 used_attacks[4096];
    U64 attack_mask = isBishop ? mask_bishop_attacks(square) : mask_rook_attacks(square);

    int occupancy_indices = 1 << relevant_bits;

    for (int index = 0; index < occupancy_indices; index++) {
        occupancies[index] = set_occupancy(index, relevant_bits, attack_mask);
        attacks[index] = isBishop ? bishop_attacks_on_the_fly(square, occupancies[index]) :
                                    rook_attacks_on_the_fly(square, occupancies[index]);
    }

    // test magic numbers
    for (int count; count < 100000000; count++) {
        U64 magic_number = generate_magic_number();

        // skip inappropriate magic numbers
        if (count_bits((attack_mask * magic_number) & 0xFF00000000000000) < 6) continue;

        memset(used_attacks, 0ULL, sizeof(used_attacks));

        int index, fail;

        // test magic index loop
        for (index = 0, fail = 0; (!fail) && (index < occupancy_indices); index++) {
            int magic_index = (int)((occupancies[index] * magic_number) >> (64 - relevant_bits));

            if (used_attacks[magic_index] == 0ULL)
                // init used attacks
                used_attacks[magic_index] = attacks[index];

            else if (used_attacks[magic_index] != attacks[index])
                // magic index does not work
                fail = 1;
        }

        if (!fail)
            return magic_number;
    }

    printf("Failed to find magic number\nImpossible (*_*)");
    return 0ULL;
}

void print_magic_numbers() {
    printf("Rook magic numbers:\n");
    for (int square = 0; square < 64; square++)
        printf("%llx\n", find_magic_number(square, rook_relevant_bits[square], 0));

    printf("\n\nBishop magic numbers:\n");
    for (int square = 0; square < 64; square++) 
        printf("%llx\n", find_magic_number(square, bishop_relevant_bits[square], 1));
}
