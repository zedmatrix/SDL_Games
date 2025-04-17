#pragma once

// Constants For Game Engine Includes
inline const float SCALE_SPEED = 0.1f;
inline const int SCREEN_WIDTH = 1024;
inline const int SCREEN_HEIGHT = 768;

// Main Shaders
namespace GE_Shader {
    inline const std::string VERTEX{"../../shaders/VertexShader"};
    inline const std::string FRAGMENT{"../../shaders/FragmentShader"};
}
// Light Shaders
namespace GE_Light {
    inline const std::string VERTEX{"../../shaders/VertexLight"};
    inline const std::string FRAGMENT{"../../shaders/FragmentLight"};
}
// Image Files
namespace GE_Images {
    inline const std::string BLOOD{"../../images/blood.png"};
    inline const std::string NINJA{"../../images/blue_ninja.png"};
    inline const std::string PLAYER{"../../images/ninja.png"};
    inline const std::string BRICKS{"../../images/bricks.png"};
    inline const std::string BULLET{"../../images/Bullet.png"};
    inline const std::string CIRCLE{"../../images/circle.png"};
    inline const std::string GLASS{"../../images/glass.png"};
    inline const std::string HUMAN{"../../images/jimmy.png"};
    inline const std::string STEEL{"../../images/steel.png"};
    inline const std::string WOOD{"../../images/wood.png"};
    inline const std::string ZOMBIE{"../../images/zombie.png"};
}

// Sound Files
namespace GE_Sound {
    inline const std::string INTRO{"../../sound/bodies.mp3"};
    inline const std::string PISTOL{"../../sound/long_pistol.wav"};
    inline const std::string MACHINE_GUN{"../../sound/pistol.wav"};
    inline const std::string RIFLE{"../../sound/rifle.wav"};
    inline const std::string SHOTGUN{"../../sound/shotgun.wav"};
}
