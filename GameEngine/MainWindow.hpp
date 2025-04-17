#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <format>
#include "Error.hpp"

enum MainWindowFlags { INVISIBLE = 0x1, FULLSCREEN = 0x2, BORDERLESS = 0x4, MAXIMIZED = 0x8 };

class MainWindow {

public:
    MainWindow();
    ~MainWindow();

    int create(std::string windowTitle, int screenWidth, int screenHeight, unsigned int windowFlags);

    void swapBuffer();
    int getScreenWidth() { return m_screenWidth; }
    int getScreenHeight() { return m_screenHeight; }

private:
    SDL_Window* m_sdlWindow;
    int m_screenWidth, m_screenHeight;

};

#endif //MAINWINDOW_HPP
