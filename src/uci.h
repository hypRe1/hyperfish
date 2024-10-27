#pragma once
#include "board.h"
#include "movegenerator.h"
#include "moveEncoding.h"
#include "evaluation.h"
#include "perft.h"
#include "bestmove.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// find encoded move value by comparing it to generated moves
int parse_move(struct Board* board, char *move_str) {
    struct Moves move_list;
    generate_moves(board, &move_list);

    // parse algebraic notation coordinate
    #define toSquare(file, rank) ((file - 'a') + (8 - (rank - '0')) * 8)

    int source = toSquare(move_str[0], move_str[1]);
    int target = toSquare(move_str[2], move_str[3]);

    // iterate over pseudo-legal moves
    for (int move_count = 0; move_count < move_list.count; move_count++) {
        int move = move_list.moves[move_count];

        // check if source and target squares match
        if (source == get_move_source(move) && target == get_move_target(move)) {
            // check if promotion matches else continue
            if (get_move_promotion(move)) {
                int piece = 1 + get_move_special(move);
                if ((piece == Q || piece == q) && move_str[4] == 'q')
                    return move;

                else if ((piece == R || piece == r) && move_str[4] == 'r')
                    return move;

                else if ((piece == B || piece == b) && move_str[4] == 'b')
                    return move;

                else if ((piece == N || piece == n) && move_str[4] == 'n')
                    return move;

                continue;
            }

            return move;
        }
    }

    // return illegal move
    return 0;
}

void parse_position(struct Board* board, char *command) {
    char *current_char = command;

    if (strncmp(command, "startpos", 8) == 0)
        parse_fen(board, start_position);

    else {
        // parse FEN position
        current_char = strstr(command, "fen");

        if (current_char == NULL)
            parse_fen(board, start_position);
        else {
            current_char += 4;
            parse_fen(board, current_char);
        }
    }

    // parse position from UCI moves
    current_char = strstr(command, "moves");

    if (current_char != NULL) {
        current_char += 6;

        while(*current_char) {
            int move = parse_move(board, current_char);

            if (move == 0)
                break;

            make_move(board, move);

            while (*current_char && *current_char != ' ') current_char++;

            current_char++;
        }
    }
}

void parse_go(struct Board* board, char *command) {
    int depth = 6;  // default depth
    char *current_depth = NULL;

    if ((current_depth = strstr(command, "depth")))
        depth = atoi(current_depth + 6);

    if (strstr(command, "perft") != NULL) {
        perft(board, depth);
    } else {
        search_position(board, depth);
    }
}

void uci_loop() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    char input[2000];

    struct Board board;
    parse_position(&board, "position startpos");

    while (1) {
        memset(input, 0, sizeof(input));
        fflush(stdout);

        if (!fgets(input, 2000, stdin)) continue;  // continue if no input
        if (input[0] == '\n') continue;  // continue if input is line break

        // kill the chess engine
        if (strncmp(input, "quit", 4) == 0) break;

        // tell GUI engine is alive
        if (strncmp(input, "isready", 7) == 0) {
            printf("readyok\n");
            continue;
        }

        // parse position command
        if (strncmp(input, "position", 8) == 0) {
            char *command = input;
            command += 9;
            parse_position(&board, command);
            continue;
        }

        // alias for position command
        if (strncmp(input, "pos", 3) == 0) {
            char *command = input;
            command += 4;
            parse_position(&board, command);
            continue;
        }

        // start new game
        if (strncmp(input, "ucinewgame", 10) == 0) {
            parse_position(&board, "position startpos");
            continue;
        }

        // parse go command
        if (strncmp(input, "go", 2) == 0) {
            char *command = input;
            command += 3;
            parse_go(&board, command);
            continue;
        }

        // return UCI info
        if (strncmp(input, "uci", 3) == 0) {
            printf("id name hyperfish\n");
            printf("id author hypre\n");
            printf("uciok\n");
            continue;
        }

        // show board and all its bitboards
        if (strncmp(input, "show all", 8) == 0) {
            print_board(&board, 1);
            continue;
        }

        // show board
        if (strncmp(input, "show", 4) == 0) {
            print_board(&board, 0);
            continue;
        }

        // print all legal moves
        if (strncmp(input, "moves", 5) == 0) {
            struct Board copy;
            struct Moves move_list;
            generate_moves(&board, &move_list);
            for (int move_count = 0; move_count < move_list.count; move_count++) {
                copy = board;
                int move = move_list.moves[move_count];
                if (!make_move(&copy, move))
                    continue;

                print_move(move);
                printf("\n");

                continue;
            }
        }

        // print all legal captures
        if (strncmp(input, "captures", 8) == 0) {
            struct Board copy;
            struct Moves move_list;
            generate_captures(&board, &move_list);
            for (int move_count = 0; move_count < move_list.count; move_count++) {
                copy = board;
                int move = move_list.moves[move_count];
                if (!make_move(&copy, move))
                    continue;

                print_move(move);
                printf("\n");

                continue;
            }
        }
    }
}
