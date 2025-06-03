// GameManager.cpp

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <cctype>

#include "GameManager.hpp"
#include "Board.hpp"

constexpr uint64_t BOARD_SIZE = 3;

enum Player : uint64_t
{
    PLAYER1 = 1,
    PLAYER2 = 2
};

uint64_t GameManager::parse_move_string(const std::string& move)
{
    if (move.length() != 2)
    {
        return 0;
    }

    char file = static_cast<char>(std::tolower(move[0]));
    char rank = move[1];

    if (file < 'a' || file > 'c' || rank < '1' || rank > '3')
    {
        return 0;
    }

    int col = file - 'a';
    int row = rank - '1';
    int bit_index = row * 3 + col;

    return 1ULL << bit_index;
}

void GameManager::display_board(const Board& board)
{
    std::cout << "  a b c\n";

    for (int row = 2; row >= 0; --row)
    {
        std::cout << (row + 1) << " ";

        for (int col = 0; col < 3; ++col)
        {
            int bit_index = row * 3 + col;
            uint64_t mask = 1ULL << bit_index;

            if (board.player1 & mask)
            {
                std::cout << "x ";
            }
            else if (board.player2 & mask)
            {
                std::cout << "o ";
            }
            else
            {
                std::cout << ". ";
            }
        }

        std::cout << "\n";
    }
}

Board GameManager::get_start_position()
{
    return Board(0, 0, PLAYER1);
}

Board GameManager::make_move(const Board& board, uint64_t move)
{
    if (board.turn == PLAYER1)
    {
        return Board(board.player1 | move, board.player2, PLAYER2);
    }
    else
    {
        return Board(board.player1, board.player2 | move, PLAYER1);
    }
}

std::string GameManager::get_game_over(const Board& board)
{
    static const uint64_t WIN_MASKS[] =
    {
        0b000000111, // Row 1
        0b000111000, // Row 2
        0b111000000, // Row 3
        0b001001001, // Col 1
        0b010010010, // Col 2
        0b100100100, // Col 3
        0b100010001, // Diag TL-BR
        0b001010100  // Diag TR-BL
    };

    for (const uint64_t mask : WIN_MASKS)
    {
        if ((board.player1 & mask) == mask)
        {
            return "p1";
        }

        if ((board.player2 & mask) == mask)
        {
            return "p2";
        }
    }

    if ((board.player1 | board.player2) == 0b111111111)
    {
        return "draw";
    }

    return "";
}

void GameManager::play_terminal_version()
{
    Board board = get_start_position();
    std::string result = "";

    display_board(board);

    while ((result = get_game_over(board)) == "")
    {
        const std::string move_str = prompt_for_move(board);
        const uint64_t move_mask = parse_move_string(move_str);

        if ((move_mask == 0) || ((board.player1 | board.player2) & move_mask))
        {
            std::cout << "Invalid move. Try again.\n";
            continue;
        }

        board = make_move(board, move_mask);
        board_states.push_back(board);
        display_board(board);
    }

    if (result == "p1")
    {
        std::cout << "Player X wins!\n";
    }
    else if (result == "p2")
    {
        std::cout << "Player O wins!\n";
    }
    else
    {
        std::cout << "It's a draw!\n";
    }
}

std::string GameManager::prompt_for_move(const Board& board)
{
    if (board.turn == PLAYER1)
    {
        std::cout << "It is Player X to move: ";
    }
    else if (board.turn == PLAYER2)
    {
        std::cout << "It is Player O to move: ";
    }
    else
    {
        std::cerr << "Unknown player's move?!\n";
    }

    std::string move;
    std::cin >> move;
    return move;
}

