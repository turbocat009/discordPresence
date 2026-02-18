
#include "Utils.hpp"
#include <sstream>

float utils::hireTimeInSeconds()
{
    return SDL_GetTicks() / 1000.0f;
}

void utils::showFPS(SDL_Window *window)
{
    static int frames = 0;
    static float lastTime = 0.0f;
    static float fps = 0.0f;

    float currentTime = SDL_GetTicks() / 1000.0f;
    frames++;

    if (currentTime - lastTime >= 1.0f)
    {
        fps = frames / (currentTime - lastTime);
        frames = 0;
        lastTime = currentTime;

        std::ostringstream title;
        title << "FPS: " << static_cast<int>(fps);
        SDL_SetWindowTitle(window, title.str().c_str());
    }
}

DeltaTimer::DeltaTimer() : lastTime(SDL_GetTicks()), deltaTime(0.0f) {}

void DeltaTimer::update() {
    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
}

float DeltaTimer::getDeltaTime() const {
    return deltaTime;
}
