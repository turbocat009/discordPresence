#include "Scenes/SelectionScene1.hpp"
#include "Entity.hpp"
#include "Button.hpp"
#include "DiscordFunc.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

SelectionScene1::SelectionScene1(RenderWindow *win, SDL_Texture *bgTex)
    : bg(bgTex),
      window(win),
      whiteBackground(window->loadTexture("res/gfx/white-background.png")),
      btnTex(window->loadTexture("res/gfx/Button_Green.png")),
      logOutBtnTex(window->loadTexture("res/gfx/logOutBtn.png")),
      smallTxt(window->loadTexture("res/gfx/smallTXT.png")),
      bigTxt(window->loadTexture("res/gfx/bigTXT.png")),
      applyBtn(Vector2f(window->getWidth() / 2 - 157, window->getHeight() / 2 + 125), Vector2f(314, 104), btnTex, "Apply Changes", ' ', true, false, true, window),
      logOutBtn(Vector2f(window->getWidth() / 2 - 250, window->getHeight() / 2 + 150), Vector2f(70, 70), logOutBtnTex, "", ' ', true, false, true, window),
      name(Vector2f(window->getWidth() / 2 - 50, window->getHeight() / 2 - 175), Vector2f(297, 50), smallTxt, "A", 16, ' ', true, false, true, window),
      desc(Vector2f(window->getWidth() / 2 - 50, window->getHeight() / 2 - 100), Vector2f(297, 50), smallTxt, "B", 16, ' ', true, false, true, window),
      type(Vector2f(75, window->getHeight() / 2 - 25), Vector2f(459, 50), bigTxt, "C", 23, ' ', true, false, true, window)

{

    fontX = (window->getWidth() - 250) / 2;
    fontY = window->getHeight() - 100;

    Name = "name.getText()";
    Desc = "desc.getText()";
    Type = "type.getText()";

    textFont = window->loadFont("res/fonts/SS3_Bold.ttf", 20);
}

void SelectionScene1::handleEvent(SDL_Event &ev, bool &running) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    SDL_Point mouse = {mx, my};
    if (ev.type == SDL_QUIT)
        running = false;
    if (ev.type == SDL_KEYDOWN) { 
        if (ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = false;
    }
    if (logOutBtn.isPressed()) {
        DiscordFunc::logOut();
    }
    if (applyBtn.isPressed()) {
        Name = name.getText();
        Desc = desc.getText();
        Type = type.getText();
    }

    applyBtn.handleEvent(ev);
    logOutBtn.handleEvent(ev);
    name.handleEvent(ev);
    desc.handleEvent(ev);
    type.handleEvent(ev);

    if (DiscordFunc::getToken() == "nullptr") {
        running = false;
    }
}

void SelectionScene1::render() {    
    applyBtn.Render(window->getRenderer());
    logOutBtn.Render(window->getRenderer());
    name.Render(window->getRenderer());
    desc.Render(window->getRenderer());
    type.Render(window->getRenderer());

    window->renderText(textFont, "Name", {255, 255, 255, 255}, window->getWidth() / 2 - 50, window->getHeight() / 2 - 200);
    window->renderText(textFont, "Description", {255, 255, 255, 255}, window->getWidth() / 2 - 50, window->getHeight() / 2 - 125);
    window->renderText(textFont, "Type", {255, 255, 255, 255}, 75, window->getHeight() / 2 - 50);
}

void SelectionScene1::update(float dt)
{
    if (controllerType.empty()) {
        controllerType = "Controller Type";
    }
    fontY = (window->getHeight() - 100) + 10 * SDL_sin(SDL_GetTicks() * (3.14 / 1500));

    applyBtn.Update(dt);
    logOutBtn.Update(dt);
    name.Update(dt);
    desc.Update(dt);
    type.Update(dt);

    DiscordFunc::setPresence(Name, Desc, Type);
}


