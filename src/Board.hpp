// Board.hpp

#pragma once
#include <cstdint>

// board state

class Board 
{
public:
    uint64_t player1; // store player1 pieces
    uint64_t player2; // store player2 pieces
    uint64_t turn; // 1 = player1, 2 = player2

    // constructor
    Board(uint64_t p1 = 0, uint64_t p2 = 0, uint64_t t = 1)
        : player1(p1), player2(p2), turn(t) {}
};

