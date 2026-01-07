#pragma once

#include <cstdint>

struct Board {
    uint64_t all_pieces; // positions of all filled tiles (us or opp)
    uint64_t our_pieces; // pos of our filled tiles
    int moves_played;

    Board();
    void play(int col);
    void unplay(int col);
    int get_score(); // -1 for no game over, 0 for draw, 1 for win9
    bool isLegal(int col); // returns if playing in that column is legal
    void printBoard();
};