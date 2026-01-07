#pragma once

#include <utility>
#include <vector>
#include "board.h"

extern long long nodes_visited;

enum HashFlag {EXACT, ALPHA, BETA};
struct HashEntry {
    uint64_t key;
    int score;
    int depth;
    int best_move;
    HashFlag flag;
};

uint64_t get_key(Board &b);

std::pair<int, int> search(Board& board, int depth, int alpha, int beta, std::vector<HashEntry>& tt); // simple negamax search, returns int column (best col to pick)

std::pair<int, int> iter_search(Board board, int max_depth, int step, int hash_size_power_2); // iterative search with hashtable