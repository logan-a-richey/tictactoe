// GameManager.cpp

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <cctype>
#include <regex>
// #include <thread>
// #include <chrono>

#include "GameManager.hpp"
#include "Board.hpp"
#include "Engine.hpp"

constexpr uint64_t BOARD_SIZE = 3;
// constexpr uint64_t PLAYER1 = 1;
// constexpr uint64_t PLAYER2 = 2;

enum Player : uint64_t
{
    PLAYER1 = 1,
    PLAYER2 = 2
};

// std::regex pattern("^\\s*([a-c])([0-2])\\s*$'");
// std::smatch();

uint64_t GameManager::parse_move_string(const std::string& move)
{
    static const std::regex pattern("^\\s*([a-cA-C])([1-3])\\s*$");
    std::smatch match;

    if (!std::regex_match(move, match, pattern)) {
        return 0;
    }

    char file = std::tolower(match.str(1)[0]);
    char rank = match.str(2)[0];

    int col = file - 'a';
    int row = rank - '1';
    int bit_index = row * 3 + col;

    return 1ULL << bit_index;
}

void GameManager::display_board(const Board& board)
{
    std::cout << "\n";

    
    for (int row = 2; row >= 0; --row)
    {
        std::cout << (row + 1) << " ";
        for (int col = 0; col < 3; ++col)
        {
            int bit_index = row * 3 + col;
            uint64_t mask = 1ULL << bit_index;

            if (board.player1 & mask) {
                std::cout << "x ";
            } else if (board.player2 & mask) {
                std::cout << "o ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "  a b c" << std::endl;
}

Board GameManager::get_start_position()
{
    return Board(0, 0, PLAYER1);
}

Board GameManager::make_move(const Board& board, uint64_t move)
{
    if (board.turn == PLAYER1) {
        return Board(board.player1 | move, board.player2, PLAYER2);
    } else {
        return Board(board.player1, board.player2 | move, PLAYER1);
    }
}

std::string GameManager::get_game_over(const Board& board)
{
    static const uint64_t WIN_MASKS[] =
    {
        0b000000111, 0b000111000, 0b111000000, // Rows
        0b001001001, 0b010010010, 0b100100100, // Cols
        0b100010001, 0b001010100 // Diags
    };

    for (const uint64_t mask : WIN_MASKS) {
        if ((board.player1 & mask) == mask) {
            return "p1";
        }
        if ((board.player2 & mask) == mask) {
            return "p2";
        }
    }

    if ((board.player1 | board.player2) == 0b111111111) {
        return "draw";
    }

    return "";
}

void GameManager::play_terminal_version(int mode)
{
    // Load Engine:
    Engine engine;

    // Game setup:
    Board board = get_start_position();
    std::string result = ""; // Store game over result string

    // Game mode:
    bool player1human = true;
    bool player2human = true;
    
    switch (mode)
    {
        case 1:
            player1human = false;
            break;
        case 2:
            player2human = false;
            break;
        case 3:
            player1human = false;
            player2human = false;
            break;
        default:
            break;
    }

    // Game loop:
    display_board(board);
    while ((result = get_game_over(board)) == "")
    {
        // Check for Computer Move:
        if ((!player1human && board.turn == PLAYER1) || (!player2human && board.turn == PLAYER2))
        {
            // Simulate thinking time
            // std::this_thread::sleep_for(std::chrono::seconds(1));

            // Call the engine best move algorithm:
            uint64_t best_move = engine.get_move(board);
            board = make_move(board, best_move);
            board_states.push_back(board);
            std::cout << "The engine made the move " << best_move << "." << std::endl;
            display_board(board);
            continue;
        }

        // Human Move:
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

    // End game message:
    if (result == "p1")
    {
        if (player1human) {
            std::cout << "Player X wins!\n";
        } else {
            std::cout << "Bot X won the game.\n";
        }
    }
    else if (result == "p2")
    {
        if (player2human) {
            std::cout << "Player O wins!\n";
        } else {
            std::cout << "Box O won the game.\n";
        }
    }
    else
    {
        std::cout << "It's a draw!\n";
    }
}

std::string GameManager::prompt_for_move([[maybe_unused]] const Board& board)
{
    if (board.turn == PLAYER1) {
        std::cout << "Player X to move: ";
    } else {
        std::cout << "Player O to move: ";
    }
    
    std::string move;
    std::cin >> move;
    return move;
}

