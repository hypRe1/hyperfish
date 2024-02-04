#pragma once
#include "board.h"
#include "macros.h"
#include "evaluation.h"
#include "movegenerator.h"
#include "attacks.h"

int ply;
int best_move = 0;
long int nodes;

static inline int quiescence(struct Board* board, int alpha, int beta) {
    int evaluation = evaluate(board);

    // fail-hard beta cutoff
    // node fails high
    if (evaluation >= beta) return beta;

    // found better move (PV node)
    if (evaluation > alpha) alpha = evaluation;

    struct Moves move_list;
    struct Board copy;
    generate_captures(board, &move_list);

    for (int move_count = 0; move_count < move_list.count; move_count++) {
        copy = *board;

        int move = move_list.moves[move_count];
        if (make_move(&copy, move)) {
            ply++;
            int score = -quiescence(&copy, -beta, -alpha);
            ply--;

            // fail-hard beta cutoff
            // node fails high
            if (score >= beta) return beta;

            // found better move (PV node)
            if (score > alpha) alpha = score;
        }
    }

    // node fails low
    return alpha;
}

static inline int negamax(struct Board* board, int alpha, int beta, int depth) {
    // base case
    if (depth == 0) return quiescence(board, alpha, beta);

    nodes++;

    int in_check = is_square_attacked(board, (board->side == white) ? lsb_index(board->bitboards[K]) : lsb_index(board->bitboards[k]), board->side ^ 1);
    int legal_moves = 0;

    int best_sofar = 0;
    int old_alpha = alpha;

    struct Moves move_list;
    struct Board copy;
    generate_moves(board, &move_list);

    for (int move_count = 0; move_count < move_list.count; move_count++) {
        copy = *board;

        int move = move_list.moves[move_count];
        if (make_move(&copy, move)) {
            legal_moves++;

            ply++;
            int score = -negamax(&copy, -beta, -alpha, depth-1);
            ply--;

            // fail-hard beta cutoff
            // node fails high
            if (score >= beta) return beta;

            // found better move (PV node)
            if (score > alpha) {
                alpha = score;
                if (ply == 0) best_sofar = move_list.moves[move_count];
            }
        }
    }

    // no legal moves in current position
    if (legal_moves == 0) {
        // king is in check (checkmate)
        if (in_check) return -99000 + ply;

        // king is not in check (stalemate)
        else return 0;
    }

    if (old_alpha != alpha) best_move = best_sofar;

    // node fails low
    return alpha;
}

void search_position(struct Board* board, int depth) {
    int score = negamax(board, -90000, 90000, depth);
    if (best_move) {
        printf("info score cp %d depth %d nodes %ld\n", score, depth, nodes);
        printf("bestmove ");
        print_move(best_move);
        printf("\n");
    }
}
