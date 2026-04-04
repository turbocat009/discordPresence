#pragma once
#include "Scenes/Scene.hpp"
#include "RenderWindow.hpp"
#include "Player.hpp"
#include "Object.hpp"
#include <vector>

class LoadingScene : public Scene {
    public:
        LoadingScene(RenderWindow *win, SDL_Texture *defTex);
        void handleEvent(SDL_Event &ev, bool &running) override;
        void update(float deltaTime) override;
        void render() override;
        bool startRequested() const { return continue_; }
    private:
        int a;
        RenderWindow *window;
        SDL_Window *wind;
        std::vector<Entity> tiles;
        bool continue_ = false;
        bool fuck = true;
        TTF_Font *textFont;
};