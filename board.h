#pragma once
#include "helpers.h"

struct Board {
    U64 bitboards[12];
    U64 occupancies[3];
    int side;
    int enpassant;
    int castle;
};