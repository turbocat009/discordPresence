#pragma once
#include "Scenes/Scene.hpp"
#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Button.hpp"
#include "PopUp.hpp"
#include <vector>

class MenuScene : public Scene {
    public:
        MenuScene(RenderWindow *win, const uint64_t APPLICATION_ID);
        void handleEvent(SDL_Event &ev, bool &running) override;
        void update(float dt) override;
        void render() override;
        bool startRequested() const { return start_; }

    private:
        RenderWindow *window;
        const uint64_t ID;
        SDL_Texture *menuImg;
        SDL_Texture *btnTex;
        SDL_Texture *popTex;
        SDL_Texture *btnTexHovered;
        SDL_Texture *popUpTex;
        Entity Menu;
        Button logInBTN;
        SDL_Color color;
        bool start_ = false;
        void buttonHovered();
        std::vector<PopUp> popups;
};