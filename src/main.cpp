#define DISCORDPP_IMPLEMENTATION
#include "DiscordFunc.hpp"
#include "discordpp.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <thread>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "Utils.hpp"
#include "Player.hpp"
#include "Debug.hpp"
#include "ControlMenu.hpp"

#include "Scenes/Scene.hpp"
#include "Scenes/MenuScene.hpp"
#include "Scenes/LoadingScene.hpp"
#include "Scenes/SelectionScene1.hpp"

const uint64_t APPLICATION_ID = 1465308629581299856;

int main(int argc, char *argv[]) {
    if(SDL_Init(SDL_INIT_VIDEO) > 0)
        std::cout << "SDL_Init failed:" << SDL_GetError() << std::endl;

    if (!(IMG_Init(IMG_INIT_PNG)))
        std::cout << "IMG_Init failed:" << SDL_GetError() << std::endl;

    if (TTF_Init() == -1)
        std::cout << "TTF_Init failed:" << TTF_GetError() << std::endl;

    RenderWindow window("Discord RePresence", 582, 582);
    SDL_Surface *logoSurface = IMG_Load("res/gfx/logoWin.png");
    SDL_SetWindowIcon(window.getWindow(), logoSurface);

    SDL_SetRenderDrawColor(window.getRenderer(), 32, 32, 32, 255);

    TTF_Font* font = window.loadFont("res/fonts/SS3_Bold.ttf", 56);

    SDL_Texture *defTexture = window.loadTexture("res/gfx/default.png");

    Scene *scene = new MenuScene(&window, APPLICATION_ID);
    scene->setFont(font);

    DiscordFunc discord;
    discord.initDiscordSDK();

    Control controlMenu(Vector2f(0, 0), Vector2f(592, 36), window.loadTexture("res/gfx/cMenuBg.png"), &window, "Discord RePresence");

    bool running = true;

    SDL_Event event;
    DeltaTimer timer;

    /*const float timeStep = 0.01f;
    float accumulator = 0.0f;
    float currentTime = utils::hireTimeInSeconds();*/

    while (running) {
        int startTicks = SDL_GetTicks();

        window.clear();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            controlMenu.HandleEvents(event, running);
            scene->handleEvent(event, running);
        }
        
        
        discordpp::RunCallbacks();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        timer.update();
        float deltaTime = timer.getDeltaTime();
        controlMenu.Update(deltaTime);
        scene->update(deltaTime);
        
        if (MenuScene *m = dynamic_cast<MenuScene *>(scene)) {
            if (m->startRequested()) {
                delete scene;
                scene = new LoadingScene(&window, defTexture);
                scene->setFont(font);
                continue;
            }
        }
        if (LoadingScene *m = dynamic_cast<LoadingScene *>(scene)) {
            if (m->startRequested()) {
                delete scene;
                scene = new SelectionScene1(&window, defTexture);
                scene->setFont(font);
                continue;
            }
        }

        if (SelectionScene1 *m = dynamic_cast<SelectionScene1 *>(scene)) {
            if (m->startRequestedBack())  {
                delete scene;
                scene = new MenuScene(&window, APPLICATION_ID);
                scene->setFont(font);
                continue;
            }
        }

        scene->render();
        controlMenu.Render(window.getRenderer(), window.getWindow());
        
        int frameTicks = SDL_GetTicks() - startTicks;
        int refreshRate = window.getRefreshRate();
        if (refreshRate > 0 && frameTicks < 1000 / refreshRate) {
            SDL_Delay((1000 / refreshRate) - frameTicks);
        }

        window.display();
    }
    delete scene;

    window.cleanUp();
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}