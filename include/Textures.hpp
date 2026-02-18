#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "RenderWindow.hpp"

class Textures {
    private:
        RenderWindow *window;
    public:
        SDL_Texture defTexture = window->loadTexture("res/gfx/default.png");
        SDL_Texture playerTexture = window->loadTexture("res/gfx/player.png");
        SDL_Texture gunTexture = window->loadTexture("res/gfx/gun.png");
};

