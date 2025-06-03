// main.cpp

#include <iostream>
#include "GameManager.hpp"
#include "Board.hpp"

int main()
{
    std::cout << "Welcome to Tic Tac Toe" << std::endl;

    GameManager game;
    game.play_terminal_version();

    std::cout << "Bye." << std::endl;
    return 0;
}

