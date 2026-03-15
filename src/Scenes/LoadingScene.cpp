#include "Scenes/LoadingScene.hpp"
#include "RenderWindow.hpp"
#include "Debug.hpp"
#include "Utils.hpp"
#include "ImageDownloader.hpp"
#include "DiscordFunc.hpp"

LoadingScene::LoadingScene(RenderWindow *win, SDL_Texture *defTex)
    : window(win)
{
    a = 0;

    for (int i = 0; i < window->getWidth() / 64; ++i)
        for (int j = 0; j < window->getHeight() / 64; ++j)
            tiles.emplace_back(Vector2f(i * 64, j * 64), Vector2f(64, 64), defTex);

    wind = window->getWindow();
}

void LoadingScene::handleEvent(SDL_Event &ev, bool &running) {
    if (ev.type == SDL_QUIT)
        running = false;
    if (ev.type == SDL_KEYDOWN) {
        if (ev.key.keysym.scancode == SDL_SCANCODE_F1)
            debug::drawCollisions = !debug::drawCollisions;
    }
}

void LoadingScene::update(float deltaTime) {
    if (DiscordFunc::getCurrentID() != 0 && fuck) {
        fuck = false;
        std::cout << getJson("http://127.0.0.1:3000/", DiscordFunc::getCurrentID()) << std::endl;
        std::cout << "Ended Getting JSON" << std::endl;
        continue_ = true;
    }
}

void LoadingScene::render() {
    window->clear();

    for (Entity &e : tiles)
        window->render(e, Vector2f(64, 64));
}
