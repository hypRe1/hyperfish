#pragma once
#include "board.h"
#include "macros.h"
#include "evaluation.h"
#include "movegenerator.h"
#include "attacks.h"

#define max_ply 64

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
int killer_moves[2][max_ply];

int pv_length[max_ply];
int pv_table[max_ply][max_ply];


int ply = 0;
long int nodes = 0;

static inline int score_move(int move) {
    // explore first if in principal variation
    if (move == pv_table[0][ply]) return 10000;

    // return MVV LVA score if move is a capture
    if (get_move_capture(move)) return mvv_lva[get_move_sourcep(move)][get_move_targetp(move)];
    else {
        // if first killer move return 9000
        if (killer_moves[0][ply] == move)
            return 9000;

        // if second killer move return 8000
        else if (killer_moves[1][ply] == move)
            return 8000;
        return 0;
    }
}

static inline void sort_moves(struct Moves *move_list) {
    // score each move and store in an array
    int move_scores[move_list->count];
    for (int count = 0; count < move_list->count; count++)
        move_scores[count] = score_move(move_list->moves[count]);

    int sorted = 0;
    int temp;

    // bubble sort algorithm to order move_list by scores
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
    pv_length[ply] = ply;

    // base case
    if (depth == 0) return quiescence(board, alpha, beta);

    // prevent overflow of arrays
    if (ply > max_ply - 1) return evaluate(board);

    nodes++;

    // check if king is in check by passing board, square king is on, and the opposite side into the is_square_attacked function
    int in_check = is_square_attacked(board, (board->side == white) ? lsb_index(board->bitboards[K]) : lsb_index(board->bitboards[k]), board->side ^ 1);

    // if in check increase depth by 1 so lines with forced checks are explored further
    if (in_check) depth++;
    int legal_moves = 0;

    struct Moves move_list;
    struct Board copy;
    generate_moves(board, &move_list);
    sort_moves(&move_list);

    // iterate over move_list
    for (int move_count = 0; move_count < move_list.count; move_count++) {
        copy = *board;

        int move = move_list.moves[move_count];
        if (make_move(&copy, move)) {
            legal_moves++;

            // recursion
            ply++;
            int score = -negamax(&copy, -beta, -alpha, depth-1);
            ply--;

            // fail-hard beta cutoff
            // node fails high
            if (score >= beta) {
                if (get_move_capture(move_list.moves[move_count]) == 0) {
                    // store killer moves
                    killer_moves[1][ply] = killer_moves[0][ply];
                    killer_moves[0][ply] = move_list.moves[move_count];
                }

                return beta;
            }

            // found better move (PV node)
            if (score > alpha) {
                alpha = score;

                // write PV move
                pv_table[ply][ply] = move_list.moves[move_count];

                // loop over the next ply
                for (int next_ply = ply + 1; next_ply < pv_length[ply + 1]; next_ply++)
                    pv_table[ply][next_ply] = pv_table[ply+1][next_ply];

                pv_length[ply] = pv_length[ply + 1];
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

    // node fails low
    return alpha;
}

void search_position(struct Board* board, int depth) {
    int score;

    // clear global variables
    nodes = 0;
    ply = 0;
    memset(killer_moves, 0, sizeof(killer_moves));
    memset(pv_table, 0, sizeof(pv_table));
    memset(pv_length, 0, sizeof(pv_length));

    for (int current_depth = 1; current_depth <= depth; current_depth++) {
        score = negamax(board, -0xF000, 0xF000, current_depth);

        printf("info score cp %d depth %d nodes %ld pv ", score, current_depth, nodes);
        for (int count = 0; count < pv_length[0]; count++) {
            print_move(pv_table[0][count]);
            printf(" ");
        }
        printf("\n");

    }
    printf("bestmove ");
    print_move(pv_table[0][0]);
    printf("\n");
}

void search_position_2(struct Board* board, int depth) {
    int score;

    // clear global variables
    nodes = 0;
    ply = 0;
    memset(killer_moves, 0, sizeof(killer_moves));
    memset(pv_table, 0, sizeof(pv_table));
    memset(pv_length, 0, sizeof(pv_length));

    score = negamax(board, -0xF000, 0xF000, depth);

    printf("info score cp %d depth %d nodes %ld pv ", score, depth, nodes);
    for (int count = 0; count < pv_length[0]; count++) {
        print_move(pv_table[0][count]);
        printf(" ");
    }
    printf("\n");

    printf("bestmove ");
    print_move(pv_table[0][0]);
    printf("\n");
}