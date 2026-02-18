#pragma once
#include "Scenes/Scene.hpp"
#include "RenderWindow.hpp"
#include "Player.hpp"
#include "Object.hpp"
#include <vector>

class GameScene : public Scene {
    public:
        GameScene(RenderWindow *win, SDL_Texture *defTex, SDL_Texture *playerTex, SDL_Texture *gunTex);
        void handleEvent(SDL_Event &ev, bool &running) override;
        void update(float deltaTime) override;
        void render() override;
    private:
        int a;
        RenderWindow *window;
        SDL_Window *wind;
        Player player;
        Player player2;
        Object object;
        std::vector<Entity> tiles;
};