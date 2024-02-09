#pragma once
#include "board.h"
#include "macros.h"
#include "evaluation.h"
#include "movegenerator.h"
#include "attacks.h"

// MVV LVA [aggressor][victim]
static int mvv_lva[6][6] = {
 	105, 205, 305, 405, 505, 605,
	104, 204, 304, 404, 504, 604,
	103, 203, 303, 403, 503, 603,
	102, 202, 302, 402, 502, 602,
	101, 201, 301, 401, 501, 601,
	100, 200, 300, 400, 500, 600,
};

// killer moves [id][ply]
int killer_moves[2][246];

int ply = 0;
int best_move = 0;
long int nodes = 0;

static inline int score_move(int move) {
    if (get_move_capture(move)) return mvv_lva[get_move_sourcep(move)][get_move_targetp(move)];
    else {
        if (killer_moves[0][ply] == move)
            return 9000;
        else if (killer_moves[1][ply] == move)
            return 8000;
        return 0;
    }
}

static inline void sort_moves(struct Moves *move_list) {
    int move_scores[move_list->count];
    for (int count = 0; count < move_list->count; count++)
        move_scores[count] = score_move(move_list->moves[count]);

    int sorted = 0;
    int temp;

    while (!sorted) {
        sorted = 1;
        for (int i = 0, j = 1; j < move_list->count; i++, j++) {
            if (move_scores[i] < move_scores[j]) {
                sorted = 0;

                temp = move_scores[i];
                move_scores[i] = move_scores[j];
                move_scores[j] = temp;

                temp = move_list->moves[i];
                move_list->moves[i] = move_list->moves[j];
                move_list->moves[j] = temp;
            }
        }
    }
}

static inline int quiescence(struct Board* board, int alpha, int beta) {
    nodes++;

    int evaluation = evaluate(board);

    // fail-hard beta cutoff
    // node fails high
    if (evaluation >= beta) return beta;

    // found better move (PV node)
    if (evaluation > alpha) alpha = evaluation;

    struct Moves move_list;
    struct Board copy;
    generate_captures(board, &move_list);
    sort_moves(&move_list);

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

    if (in_check) depth++;
    int legal_moves = 0;

    int best_sofar = 0;
    int old_alpha = alpha;

    struct Moves move_list;
    struct Board copy;
    generate_moves(board, &move_list);
    sort_moves(&move_list);

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
            if (score >= beta) {
                if (get_move_capture(move_list.moves[count]) == 0) {
                    // store killer moves
                    killer_moves[1][ply] = killer_moves[0][ply];
                    killer_moves[0][ply] = move_list.moves[move_count];
                }

                return beta;
            }

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
        if (in_check) return -0xE000 + ply;

        // king is not in check (stalemate)
        else return 0;
    }

    if (old_alpha != alpha) best_move = best_sofar;

    // node fails low
    return alpha;
}

void search_position(struct Board* board, int depth) {
    nodes = 0;
    ply = 0;
    int score = negamax(board, -0xF000, 0xF000, depth);

    if (best_move) {
        printf("info score cp %d depth %d nodes %ld\n", score, depth, nodes);
        printf("bestmove ");
        print_move(best_move);
        printf("\n");
    }
}
