// Engine.hpp

#pragma once

#include "Board.hpp"
#include <cstdint>

class Engine {
  public:
    // Returns bitmask of best move
    uint64_t get_move(const Board& board);

  private:
    int minimax(const Board& board, bool maximizing);
    int evaluate_board(const Board& board);
};
