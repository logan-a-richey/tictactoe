// Board.hpp

#pragma once
#include <cstdint>

class Board 
{
public:
    uint64_t player1;
    uint64_t player2;
    uint64_t turn;

    Board(uint64_t p1 = 0, uint64_t p2 = 0, uint64_t t = 1)
        : player1(p1), player2(p2), turn(t) {}
};

