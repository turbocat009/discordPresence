#include "Scenes/MenuScene.hpp"
#include "DiscordFunc.hpp"
#include "Button.hpp"
#include "ErrorNotifier.hpp"
#include <SDL2/SDL.h>

MenuScene::MenuScene(RenderWindow *win, const uint64_t APPLICATION_ID)
    : window(win), ID(APPLICATION_ID),
      menuImg(window->loadTexture("res/gfx/MainMenu.png")),
      btnTex(window->loadTexture("res/gfx/Button_Orange.png")),
      popTex(window->loadTexture("res/gfx/PopUp(new).png")),
      Menu(Vector2f(window->getWidth() / 2 - 125, window->getHeight() / 2 + 25), Vector2f(512, 512), menuImg),
      logInBTN(Vector2f(window->getWidth() / 2 - 157, window->getHeight() / 2 + 50), Vector2f(314, 104), btnTex, "Log In With Discord", window)
{    
    if (DiscordFunc::WhichErrorNow == 1) {
        popups.emplace_back(Vector2f(29, 106), Vector2f(526, 330), popTex, "Failed to connect to Discord \nCheck your internet connection, \nor try again later", "Error", 5, window);
    }

    // Register global error notifier to show popups in this scene, avoiding duplicates
    ErrorNotifier::setCallback([this](const std::string &message, const std::string &title) {
        // if an identical popup is already active, skip
        for (auto &p : popups) {
            if (!p.getIsDestroyed() && p.getTitle() == title && p.getMessage() == message) {
                return;
            }
        }
        popups.emplace_back(Vector2f(29, 106), Vector2f(526, 330), popTex, message, title, 0, window);
    });
}

void MenuScene::handleEvent(SDL_Event &ev, bool &running) {
    if (ev.type == SDL_QUIT)
        running = false;
    if (ev.type == SDL_KEYDOWN) {
        if (ev.key.keysym.scancode == SDL_SCANCODE_RETURN)
            start_ = true;
        if (ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = false;
    }

    bool eventConsumedByPopup = false;
    for (auto it = popups.rbegin(); it != popups.rend(); ++it) {
        it->handleEvent(ev);
        if (it->consumesEvent()) {
            eventConsumedByPopup = true;
            break;
        }
    }

    if (!eventConsumedByPopup) {
        logInBTN.handleEvent(ev);
        if(logInBTN.isPressed()) {
            std::cout << "Button Pressed" << std::endl;
            DiscordFunc::logIn(ID, DiscordFunc::getClient().get());
        }
        if (DiscordFunc::getToken() != "nullptr") {
            start_ = true;
        }
    }
}

void MenuScene::update(float dt) {
    logInBTN.Update(dt);

    if (DiscordFunc::WhichErrorNow == 2) {
        {
            std::string msg = "Failed to Log In to Discord \nUser has cancelled \nTry Again";
            bool exists = false;
            for (auto &p : popups) if (!p.getIsDestroyed() && p.getTitle() == "Error" && p.getMessage() == msg) { exists = true; break; }
            if (!exists) {
                popups.emplace_back(Vector2f(29, 106), Vector2f(526, 330), popTex, msg, "Error", 0, window);
                popups.back().setShowErrorCode(true);
                popups.back().setErrorCode("2");
            }
        }
        DiscordFunc::WhichErrorNow = 0;
    }
    if (DiscordFunc::WhichErrorNow == 3) {
        {
            std::string msg = "Failed to Log In to Discord \nTry Again";
            bool exists = false;
            for (auto &p : popups) if (!p.getIsDestroyed() && p.getTitle() == "Error" && p.getMessage() == msg) { exists = true; break; }
            if (!exists) {
                popups.emplace_back(Vector2f(29, 106), Vector2f(526, 330), popTex, msg, "Error", 0, window);
                popups.back().setShowErrorCode(true);
                popups.back().setErrorCode("3");
            }
        }
        DiscordFunc::WhichErrorNow = 0;
    }

    for (auto &popup : popups) {
        popup.Update(dt);
    }
}

void MenuScene::render() {
    color = {255, 255, 255, 255};

    logInBTN.Render(window->getRenderer());
    window->renderText(getFont(), "Discord RePresence", color, window->getWidth() / 2 - 240, window->getHeight() / 2 - 200);

    for (auto &popup : popups) {
        popup.Render(window->getRenderer());
    }
}
