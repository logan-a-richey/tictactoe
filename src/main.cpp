// main.cpp

#include <iostream>
#include <cstring>

#include "GameManager.hpp"
#include "Board.hpp"

enum Mode : int {
    HUMAN_VS_HUMAN = 0,
    HUMAN_VS_ENGINE = 1,
    ENGINE_VS_HUMAN = 2,
    ENGINE_VS_ENGINE = 3
};

int main(int argc, char** argv)
{
    int mode = HUMAN_VS_HUMAN;
    if (argc > 1) {
        char arg = argv[1][0];  // Take the first character

        switch (arg) {
            case '1':
                mode = HUMAN_VS_ENGINE;
                break;
            case '2':
                mode = ENGINE_VS_HUMAN;
                break;
            case '3':
                mode = ENGINE_VS_ENGINE;
                break;
            default:
                mode = HUMAN_VS_HUMAN;
                break;
        }
    }

    // Run game in terminal
    GameManager game;
    game.play_terminal_version(mode);

    return 0;
}

