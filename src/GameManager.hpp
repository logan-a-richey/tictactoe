// GameManager.h

#pragma once

#include <string>
#include <vector>

#include "Board.hpp"

class GameManager
{
public:
    void play_terminal_version(int mode);

    void display_board(const Board& board);

    std::string prompt_for_move(const Board& board);

    uint64_t parse_move_string(const std::string& move);

    Board get_start_position();

    Board make_move(const Board& board, uint64_t move);

    std::string get_game_over(const Board& board);

private:
    std::vector<Board> board_states = {};
    int current_board_state = 0; // index of current board state to view
};

