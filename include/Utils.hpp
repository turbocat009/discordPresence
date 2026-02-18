#pragma once
#include <SDL2/SDL.h>
#include <string>

namespace utils
{
    float hireTimeInSeconds();
    void showFPS(SDL_Window *window);
}

//Delta Time
class DeltaTimer {
private:
    Uint32 lastTime;
    float deltaTime;
public:
    DeltaTimer();
    void update();
    float getDeltaTime() const;
};

static SDL_Scancode keyboardControls[] = {
    SDL_SCANCODE_W,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_A,
    SDL_SCANCODE_SPACE
};

static SDL_Scancode arrowControls[] = {
    SDL_SCANCODE_UP,
    SDL_SCANCODE_DOWN,
    SDL_SCANCODE_RIGHT,
    SDL_SCANCODE_LEFT,
    SDL_SCANCODE_RETURN
};