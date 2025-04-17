#include "MainWindow.hpp"

MainWindow::MainWindow() {

}
MainWindow::~MainWindow() {

}

int MainWindow::create(std::string windowTitle, int screenWidth, int screenHeight, unsigned int windowFlags) {
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    uint32_t m_flags = SDL_WINDOW_OPENGL;

    if (windowFlags & INVISIBLE) m_flags |= SDL_WINDOW_HIDDEN;
    if (windowFlags & FULLSCREEN) m_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    if (windowFlags & BORDERLESS) m_flags |= SDL_WINDOW_BORDERLESS;
    if (windowFlags & MAXIMIZED) m_flags |= SDL_WINDOW_MAXIMIZED;

    // Create your window using the predefine variables
    m_sdlWindow = SDL_CreateWindow(windowTitle.c_str(),
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               m_screenWidth,
                               m_screenHeight,
                               m_flags);
    if (m_sdlWindow == nullptr) {
        fatalError("SDL Window could not be created!");
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(m_sdlWindow);
    if (glContext == nullptr) {
        fatalError("SDL_GL Context could not be initialized");
    }
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        fatalError("Could not Initialize GLEW");
    }
    std::cout << "*** OpenGL Version: " << glGetString(GL_VERSION) << " ***\n";
    std::cout << "*** OpenGL Renderer: " << glGetString(GL_RENDERER) << " ***\n";


    // Enable VSYNC
    SDL_GL_SetSwapInterval(1);

    // Enable Alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 0;
}

void MainWindow::swapBuffer() {
    SDL_GL_SwapWindow(m_sdlWindow);
}
