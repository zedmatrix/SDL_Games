
# simple meson.build to include to other project games
project('SDL_Games', 'cpp', version: '0.2.0',
    default_options: ['cpp_std=c++23'],
    meson_version: '>=1.6.0')

# dependencies for the game engine
sdl2_dep = dependency('sdl2', version : '>=2.32.0', required: true)
glew_dep = dependency('glew', version : '>=2.2.0', required: true)
sdlmixer = dependency('SDL2_mixer', version : '>=2.8.1')
sdlttf = dependency('SDL2_ttf', version : '>=2.22.0')
box2d_dep = dependency('box2d', required: true)

# include directories and files
ge_include = include_directories('../GameEngine/')
ge_source = files(
    '../GameEngine/AudioEngine.cpp',
    '../GameEngine/Bullet.cpp',
    '../GameEngine/Camera.cpp',
    '../GameEngine/Error.cpp',
    '../GameEngine/GLSLProgram.cpp',
    '../GameEngine/Input.cpp',
    '../GameEngine/IOManager.cpp',
    '../GameEngine/MainWindow.cpp',
    '../GameEngine/ParticleEngine.cpp',
    '../GameEngine/picopng.cpp',
    '../GameEngine/ResourceManager.cpp',
    '../GameEngine/SpriteBatch.cpp',
    '../GameEngine/SpriteFont.cpp',
    '../GameEngine/TextureCache.cpp',
    '../GameEngine/Timing.cpp'
)

# Build Game Engine as static library
ge_lib = library('gameEngine', ge_source,
    include_directories: ge_include,
    dependencies: [sdl2_dep, sdlmixer, sdlttf, glew_dep])

# sdl game, source and include
incdir = include_directories('./')
srcdir = files(
    'main.cpp',
    'MainGame.cpp')

executable('sdlgame', srcdir,
    include_directories: incdir,
    dependencies: [sdl2_dep, glew_dep],
    link_with: ge_lib
)
