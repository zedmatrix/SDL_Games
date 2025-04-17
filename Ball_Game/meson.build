project('SDL2_GL_Ball_Game', 'cpp', version: '0.2.0',
    default_options: ['cpp_std=c++23'],
    meson_version: '>=1.6.0')

sdl2_dep = dependency('sdl2', version : '>=2.32.0', required: true)
glew_dep = dependency('glew', version : '>=2.2.0', required: true)
box2d_dep = dependency('box2d', version : '>=2.4.1', required: true)
sdlmixer = dependency('SDL2_mixer', version : '>=2.8.1')
sdlttf = dependency('SDL2_ttf', version : '>=2.22.0')

ge_include = include_directories('../GameEngine/')
ge_source = files(
    '../GameEngine/AudioEngine.cpp',
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

incdir = include_directories('./')
srcdir = files(
    'main.cpp',
    'MainGame.cpp',
    'BallController.cpp',
    'Grid.cpp')

# Build the executable and link it with Bengine
executable('ballgame', srcdir,
    include_directories: [incdir, ge_include],
    dependencies: [sdl2_dep, glew_dep],
    link_with: ge_lib
)

# install_data('Textures/block_grid.png', install_dir: meson.project_build_root() / 'Textures')
# install_data('Textures/CharacterRight_Standing.png', install_dir: meson.project_build_root() / 'Textures')
# install_data('Textures/myCharacter_standing.png', install_dir: meson.project_build_root() / 'Textures')
# install_data('Shaders/FragmentShader', install_dir: meson.project_build_root() / 'Shaders')
# install_data('Shaders/VertexShader', install_dir: meson.project_build_root() / 'Shaders')
#
# run_command(['mkdir', '-p', meson.project_build_root() / 'Textures'], check: true)
#
# run_command(['cp', 'Textures/Bullet.png',
#             meson.project_build_root() / 'Textures/Bullet.png'], check: true)
#
# run_command(['cp', 'Textures/CharacterRight_Standing.png',
#             meson.project_build_root() / 'Textures/CharacterRight_Standing.png'], check: true)
#
# run_command(['cp', 'Textures/myCharacter_standing.png',
#             meson.project_build_root() / 'Textures/myCharacter_standing.png'], check: true)
#
# run_command(['mkdir', '-p', meson.project_build_root() / 'Shaders'], check: true)
# run_command(['cp', 'Shaders/FragmentShader', meson.project_build_root() / 'Shaders/FragmentShader'], check: true)
# run_command(['cp', 'Shaders/VertexShader', meson.project_build_root() / 'Shaders/VertexShader'], check: true)
