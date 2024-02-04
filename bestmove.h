#pragma once
#include "board.h"
#include "macros.h"
#include "evaluation.h"
#include "movegenerator.h"

int ply;
int best_move;
int nodes;

static inline int negamax(struct Board* board, int alpha, int beta, int depth) {
    // base case
    if (depth == 0) return evaluate(board);

    nodes++;

    struct Moves move_list;

}
