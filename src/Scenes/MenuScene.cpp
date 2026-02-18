#include "Scenes/MenuScene.hpp"
#include "DiscordFunc.hpp"
#include "Button.hpp"
#include <SDL2/SDL.h>

MenuScene::MenuScene(RenderWindow *win, const uint64_t APPLICATION_ID )
    : window(win), ID(APPLICATION_ID),
      menuImg(window->loadTexture("res/gfx/MainMenu.png")),
      btnTex(window->loadTexture("res/gfx/Button_Orange.png")),
      Menu(Vector2f(window->getWidth() / 2 - 125, window->getHeight() / 2 + 25), Vector2f(512, 512), menuImg),
      logInBTN(Vector2f(window->getWidth() / 2 - 157, window->getHeight() / 2 + 50), Vector2f(314, 104), btnTex, "Log In With Discord", ' ', true, false, true, window)

{}

void MenuScene::handleEvent(SDL_Event &ev, bool &running) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    SDL_Point mouse = {mx, my};
    if (ev.type == SDL_QUIT)
        running = false;
    if (ev.type == SDL_KEYDOWN) {
        if (ev.key.keysym.scancode == SDL_SCANCODE_RETURN)
            start_ = true;
        if (ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = false;
    }
    if(logInBTN.isPressed()) {
        std::cout << "Button Pressed" << std::endl;
        DiscordFunc::logIn(ID, DiscordFunc::getClient().get());
    }
    if (DiscordFunc::getToken() != "nullptr") {
        start_ = true;
    }
    logInBTN.handleEvent(ev);
}

void MenuScene::update(float dt) {
    logInBTN.Update(dt);
}

void MenuScene::render() {
    color = {255, 255, 255, 255};


    logInBTN.Render(window->getRenderer());
    window->renderText(getFont(), "Discord RePresence", color, window->getWidth() / 2 - 240, window->getHeight() / 2 - 200);
}
