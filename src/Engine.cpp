// Engine.cpp

#include "Engine.hpp"

#include <limits>
#include <vector>

static const uint64_t WIN_MASKS[] = {
    0b000000111, 0b000111000, 0b111000000, // Rows
    0b001001001, 0b010010010, 0b100100100, // Cols
    0b100010001, 0b001010100 // Diags
};

/*****************************************************************************/
// Public:

uint64_t Engine::get_move(const Board& board) {
    uint64_t best_move = 0;
    int best_score
        = (board.turn == 1) ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();

    uint64_t occupied = board.player1 | board.player2;

    for (int i = 0; i < 9; ++i) {
        uint64_t move = 1ULL << i;
        if (occupied & move) continue;

        Board next = (board.turn == 1) ? Board(board.player1 | move, board.player2, 2)
                                       : Board(board.player1, board.player2 | move, 1);

        int score = minimax(next, board.turn != 1);
        if ((board.turn == 1 && score > best_score) || (board.turn == 2 && score < best_score)) {
            best_score = score;
            best_move = move;
        }
    }

    return best_move;
}

/*****************************************************************************/
// Private:

int Engine::evaluate_board(const Board& board) {
    for (auto mask : WIN_MASKS) {
        if ((board.player1 & mask) == mask) return 10; // player1 wins
        if ((board.player2 & mask) == mask) return -10; // player2 wins
    }

    if ((board.player1 | board.player2) == 0b111111111) return 0; // draw

    return std::numeric_limits<int>::min(); // not terminal
}

int Engine::minimax(const Board& board, bool maximizing) {
    int score = evaluate_board(board);
    if (score != std::numeric_limits<int>::min()) return score;

    int best = maximizing ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    uint64_t occupied = board.player1 | board.player2;

    for (int i = 0; i < 9; ++i) {
        uint64_t move = 1ULL << i;
        if (occupied & move) continue;

        Board next = (maximizing) ? Board(board.player1 | move, board.player2, 2)
                                  : Board(board.player1, board.player2 | move, 1);

        int eval = minimax(next, !maximizing);
        best = maximizing ? std::max(best, eval) : std::min(best, eval);
    }

    return best;
}
