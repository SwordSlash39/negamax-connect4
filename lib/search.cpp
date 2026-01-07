#include "search.h"
#include "eval.h"
#include <utility>
#include <limits>
#include <algorithm>
#include <vector>

long long nodes_visited = 0;
const int column_order[] = {3, 2, 4, 1, 5, 0, 6};

size_t get_index(uint64_t key, size_t table_size) {
    return key & (table_size - 1);
}

std::pair<int, int> search(Board& board, int depth, int alpha, int beta, std::vector<HashEntry>& tt) {
    nodes_visited++; 
    int orig_alpha = alpha;

    // check key
    uint64_t key = get_key(board);
    size_t tt_idx = get_index(key, tt.size());
    HashEntry& entry = tt[tt_idx];

    // depth check
    if (entry.key == key && entry.depth >= depth) {
        // we have searched this position to a sufficient depth
        if (entry.flag == EXACT) return {entry.score, entry.best_move};
        if (entry.flag == ALPHA && entry.score <= alpha) return {alpha, entry.best_move};
        if (entry.flag == BETA && entry.score >= beta) return {beta, entry.best_move};
    }

    // check if game end
    int status = board.get_score();
    if (status == 1) return {-10000.0 - depth, -1}; // We lost (it's opponent's turn), higher the depth the better (we won earlier)
    if (status == 0) return {0.0, -1};              // Draw
    
    if (depth <= 0) {
        return {eval(board), -1};
    }

    int highest_eval = -std::numeric_limits<int>::max();
    int best_move = -1;
    int moves_tried = 0;
    int first_move = (entry.key == key) ? entry.best_move : 3;

    // use current best move first
    if (board.isLegal(first_move)) {
        moves_tried++;

        Board tmp = board;
        tmp.play(first_move);
        
        int score;

        // Negamax: we flip the score and the alpha/beta bounds
        score = -search(tmp, depth - 1, -beta, -alpha, tt).first;

        if (score > highest_eval) {
            highest_eval = score;
            best_move = first_move;
        }

        alpha = std::max(alpha, score);
        if (alpha >= beta) {
            return {highest_eval, best_move}; // Alpha-Beta Cutoff
        }
    }
    
    for (int col : column_order) {
        if (col == first_move) {
            continue;
        }
        if (board.isLegal(col)) {
            moves_tried++;

            Board tmp = board;
            tmp.play(col);
            
            int score;

            // Negamax: we flip the score and the alpha/beta bounds
            // we searched best move & column 3 possibly, now we should be safe
            if (moves_tried >= 2 && depth >= 3) {
                score = -search(tmp, depth - 2, -(alpha+1), -alpha, tt).first;

                // check to re search
                if (score > alpha) {
                    // we somehow found a order with higher eval than current
                    score = -search(tmp, depth - 1, -beta, -alpha, tt).first;
                }
            } else {
                score = -search(tmp, depth - 1, -beta, -alpha, tt).first;
            }

            if (score > highest_eval) {
                highest_eval = score;
                best_move = col;
            }

            alpha = std::max(alpha, score);
            if (alpha >= beta) {
                break; // Alpha-Beta Cutoff
            }
        }
    }
    
    // Update table
    HashFlag flag = EXACT;
    if (highest_eval <= orig_alpha) {
        flag = ALPHA;
    }
    else if (highest_eval >= beta) {
        flag = BETA;
    }

    if (entry.key != key || depth >= entry.depth) {
        entry.key = key;
        entry.score = highest_eval;
        entry.depth = depth;
        entry.best_move = best_move;
        entry.flag = flag;
    }

    return {highest_eval, best_move};
}

uint64_t get_key(Board &b) {
    return b.all_pieces + b.our_pieces;
}

std::pair<int, int> iter_search(Board board, int max_depth, int step, int hash_size_power_2) {
    const long long TABLE_SIZE = 1ULL << hash_size_power_2;
    std::vector<HashEntry> tt(TABLE_SIZE);

    std::pair<int, int> curr_eval_move = {0, 3};
    int alpha = -std::numeric_limits<int>::max();
    int beta = std::numeric_limits<int>::max();
    int window = 20;
    int curr_eval;

    // starts from 2 
    bool keepSearching = true;
    for (int depth = 2; depth <= max_depth; depth++) {
        while (keepSearching) {
            curr_eval_move = search(board, depth, alpha, beta, tt);
            
            if (depth <= 8) {
                // too unstable to search
                break;
            }

            curr_eval = curr_eval_move.first;

            if (curr_eval <= alpha) {
                alpha -= window;
                window *= 2;
            } 
            else if (curr_eval >= beta) {
                beta += window;
                window *= 2;
            } 
            else {
                // Update alpha
                alpha = curr_eval_move.first - window;
                beta = curr_eval_move.first + window;

                keepSearching = false;
            }
                
            if (curr_eval_move.first > 9000) {
                break; // safe stop if win is found
            }
        }
        keepSearching = true;
    }

    // get eval
    int eval = curr_eval_move.first;
    int bestMove = curr_eval_move.second;
    return {eval, bestMove};
}