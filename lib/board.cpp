#include "board.h"
#include <iostream>

Board::Board() {
    all_pieces = 0;
    our_pieces = 0;
    moves_played = 0;
}

void Board::play(int col) {
    uint64_t move_bit = all_pieces + (1ULL << (col * 7)); // since its 1s and 0s adding 1 will push it to another 1 (00111 + 00001 = 01000) can OR to update
    our_pieces ^= all_pieces; // swap turn with XOR
    all_pieces |= move_bit; // OR operation (we only update after swapping as its more efficient)

    // Update move counter
    moves_played++;
}

bool Board::isLegal(int col) {
    // check if top slot is full
    // top slot: col * 7 + 5
    return (all_pieces & (1ULL << (col * 7 + 5))) == 0;
}

int Board::get_score() {
    // first we flip the board since play flips it
    uint64_t flipped_board = our_pieces ^ all_pieces;
    
    // check for win
    // these check for pairs first
    uint64_t column_win = flipped_board & (flipped_board >> 1);
    uint64_t row_win = flipped_board & (flipped_board >> 7);
    uint64_t diag_right_win = flipped_board & (flipped_board >> 8); // note a down left diag is the same as a diag right
    uint64_t diag_left_win = flipped_board & (flipped_board >> 6);

    // now we check if pairs match with other pairs
    if (
        (column_win & (column_win >> 2)) | 
        (row_win & (row_win >> 14)) | 
        (diag_right_win & (diag_right_win >> 16)) | 
        (diag_left_win & (diag_left_win >> 12))
    ) {
        return 1; // win
    }

    // Check for draw
    if (moves_played >= 42) {
        return 0; // draw
    }

    return -1; // game not over
}

void Board::printBoard() {
    // Determine which bits belong to "X" and which to "O"
    // If moves_played is even, it's Player 1's turn to move, 
    // so Player 2 just moved.
    uint64_t p1_bits, p2_bits;
    if (moves_played % 2 == 0) {
        p1_bits = our_pieces;              // Player 1
        p2_bits = all_pieces ^ our_pieces; // Player 2
    } else {
        p2_bits = our_pieces;              // Player 2
        p1_bits = all_pieces ^ our_pieces; // Player 1
    }

    std::cout << "\n  0 1 2 3 4 5 6\n"; // Header
    std::cout << "-----------------\n";

    // Loop rows from top (5) to bottom (0)
    for (int row = 5; row >= 0; --row) {
        std::cout << row << "|"; // Side header
        for (int col = 0; col < 7; ++col) {
            uint64_t mask = 1ULL << (col * 7 + row);
            
            if (p1_bits & mask) {
                std::cout << "X ";
            } else if (p2_bits & mask) {
                std::cout << "O ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "-----------------\n";
}