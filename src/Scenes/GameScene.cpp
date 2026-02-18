#include "Scenes/GameScene.hpp"
#include "RenderWindow.hpp"
#include "Debug.hpp"
#include "Utils.hpp"

GameScene::GameScene(RenderWindow *win, SDL_Texture *defTex, SDL_Texture *playerTex, SDL_Texture *gunTex)
    : window(win), player(Vector2f(100, 100), Vector2f(72, 64), playerTex, window), 
    player2(Vector2f(200, 200), Vector2f(72, 64), playerTex, window),
    object(Vector2f(rand() % (window->getWidth() - 32), rand() % (window->getHeight() - 32)), Vector2f(32, 32), gunTex)
{
    a = 0;

    for (Player* p : {&player, &player2}) {
        p->setGunTexture(gunTex);
    }

    for (int i = 0; i < window->getWidth() / 64; ++i)
        for (int j = 0; j < window->getHeight() / 64; ++j)
            tiles.emplace_back(Vector2f(i * 64, j * 64), Vector2f(64, 64), defTex);

    wind = window->getWindow();
}

void GameScene::handleEvent(SDL_Event &ev, bool &running) {
    if (ev.type == SDL_QUIT)
        running = false;
    if (ev.type == SDL_KEYDOWN) {
        if (ev.key.keysym.scancode == SDL_SCANCODE_F1)
            debug::drawCollisions = !debug::drawCollisions;
    }
}

void GameScene::update(float deltaTime) {
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);

    player.handleInput(keyState, deltaTime, keyboardControls);
    player2.handleInput(keyState, deltaTime, arrowControls);
    for (Player *p : {&player, &player2}) {
        p->Update(deltaTime);
        p->playerCollision(window->getWidth(), window->getHeight());
    }


    if (a == 0) {
        player.setCurrentFrame(2304, 512, true);
        player2.setCurrentFrame(2304, 512, true);
    }
    if ((int)deltaTime > 0) {        
        if (a == (1000 * (int)deltaTime))
            a = -1;
    }
    else {
        if (a == 30)
            a = -1;
    }
    
    a++;
    object.checkCollisions(player);
    object.Update(deltaTime);
}

void GameScene::render() {
    window->clear();

    for (Entity &e : tiles)
        window->render(e, Vector2f(64, 64));

    object.Render(window->getRenderer(), wind);
    player.Render(window->getRenderer(), wind);
    player2.Render(window->getRenderer(), wind);

    if (debug::drawCollisions) {
        player.drawCollisionBox(window->getRenderer());
        player2.drawCollisionBox(window->getRenderer());
        object.drawCollisionBox(window->getRenderer());
    }

}

