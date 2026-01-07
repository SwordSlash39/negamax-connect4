#include <iostream>
#include <cstdint>
#include "lib/board.h"
#include "lib/eval.h"
#include "lib/search.h"

using namespace std;

int main() {
    Board b;
    int col;

    bool ourTurn = true;
    while (b.get_score() == -1) {
        if (!ourTurn) {
            ourTurn = true;
            pair<double, int> eval_move = iter_search(b, 18, 2, 25);

            cout << "Eval: " << eval_move.first << endl;
            cout << "Best move: " << eval_move.second << endl;

            b.play(eval_move.second);
            continue;
        }
        b.printBoard();
        std::cout << "Player " << (b.moves_played % 2 + 1) << " (0-6): ";
        std::cin >> col;

        if (col >= 0 && col <= 6 && b.isLegal(col)) {
            b.play(col);
            ourTurn = false;
        } else {
            std::cout << "Invalid Move!\n";
        }
    }
    b.printBoard();
    int result = b.get_score();
    if (result == 1) std::cout << "Winner detected!\n";
    else if (result == 0) std::cout << "Draw!\n";

    cin.get();
    return 0;
}