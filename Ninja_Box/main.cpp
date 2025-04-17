#include "MainGame.hpp"
#include <iostream>

int main(int argc, char** argv) {

    std::cout << "Blue Ninja Box Game - meson.build\n";

    MainGame mainGame;
    mainGame.run();

    std::cout << "\n \t *** Good Bye! *** \n" << std::endl;

    return 0;
}
