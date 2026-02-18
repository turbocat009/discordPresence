#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Scene {
    public:
        virtual ~Scene() = default;
        virtual void handleEvent(SDL_Event &ev, bool &running) = 0;
        virtual void update(float dt) = 0;
        virtual void render() = 0;
        void setFont(TTF_Font *f) { m_font = f; }
        TTF_Font *getFont() const { return m_font; }
    private:
        TTF_Font *m_font = nullptr;
};