#include <SDL2/SDL.h>
#include <iostream>
#include "Error.hpp"

void fatalError(std::string errorString) {
    std::cout << "Error: " << errorString << std::endl;
    SDL_Quit();
    exit(1);
}
