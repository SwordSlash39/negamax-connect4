#include <iostream>
#include <cstdint>
#include "lib/board.h"
#include "lib/eval.h"
#include "lib/search.h"

#include <limits>

using namespace std;

#include <chrono>

int main() {
    Board b;
    int search_depth = 20;
    
    // Reset counter
    nodes_visited = 0;

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    // Call search
    auto result = iter_search(b, 20, 2, 25);

    // End timer
    auto end = std::chrono::high_resolution_clock::now();
    
    // Calculate duration in seconds
    std::chrono::duration<double> elapsed = end - start;
    double seconds = elapsed.count();

    // Calculate NPS
    long long nps = (seconds > 0) ? (long long)(nodes_visited / seconds) : 0;

    std::cout << "Depth: " << search_depth << std::endl;
    std::cout << "Nodes: " << nodes_visited << std::endl;
    std::cout << "Time:  " << seconds << "s" << std::endl;
    std::cout << "NPS:   " << nps << std::endl;
    std::cout << "Best Move: " << result.second << std::endl;

    cin.get();
    return 0;
}