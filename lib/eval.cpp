#include "eval.h"

int eval(Board &board) {    
    // fix next time
    int eval = 0;
    uint64_t our_pieces = board.our_pieces;

    uint64_t col2 = our_pieces & (our_pieces >> 1);
    uint64_t row2 = our_pieces & (our_pieces >> 7);
    uint64_t diagr2 = our_pieces & (our_pieces >> 8); // note a down left diag is the same as a diag right
    uint64_t diagl2 = our_pieces & (our_pieces >> 6);

    uint64_t col3 = col2 & (our_pieces >> 2);
    uint64_t row3 = row2 & (our_pieces >> 14);
    uint64_t diagr3 = diagr2 & (our_pieces >> 16);
    uint64_t diagl3 = diagl2 & (our_pieces >> 12);

    // add eval
    eval += __builtin_popcountll(col2 | row2 | diagr2 | diagl2);
    eval += 20 * __builtin_popcountll(col3 | row3 | diagr3 | diagl3);

    // now for the opps
    uint64_t opp_pieces = our_pieces ^ board.all_pieces;

    col2 = opp_pieces & (opp_pieces >> 1);
    row2 = opp_pieces & (opp_pieces >> 7);
    diagr2 = opp_pieces & (opp_pieces >> 8); // note a down left diag is the same as a diag right
    diagl2 = opp_pieces & (opp_pieces >> 6);

    col3 = col2 & (opp_pieces >> 2);
    row3 = row2 & (opp_pieces >> 14);
    diagr3 = diagr2 & (opp_pieces >> 16);
    diagl3 = diagl2 & (opp_pieces >> 12);

    // add eval
    eval -= __builtin_popcountll(col2 | row2 | diagr2 | diagl2);
    eval -= 20 * __builtin_popcountll(col3 | row3 | diagr3 | diagl3);
    
    return eval;
}