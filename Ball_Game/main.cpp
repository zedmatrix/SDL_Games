#include "MainGame.hpp"
#include <iostream>

int main(int argc, char** argv) {

    std::cout << "Game Engine - Ball Game - meson.build\n";

    MainGame game;
    game.run();

    std::cout << "Good Bye!\n";

    return 0;
}
