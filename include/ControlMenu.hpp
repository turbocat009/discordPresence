#pragma once
#include "Entity.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Player.hpp"
#include "Button.hpp"
#include "RenderWindow.hpp"
extern "C"
{
    #include <libtray/tray.h>
}

class Control : public Entity {
    public:
        Control(Vector2f p_pos, Vector2f p_size, SDL_Texture *p_tex, RenderWindow *win, std::string text);
        void Update(float deltaTime);
        void Render(SDL_Renderer *renderer, SDL_Window *wind);
        void checkCollisions(Player &player);
        void destroyPopUP(SDL_Event &ev);
        void HandleEvents(SDL_Event &ev, bool &running);
        void moveWindow();
        void closeAPP(struct tray_menu_item *item);
        void openAPP(struct tray_menu_item *item);

    private:
        RenderWindow *window;
        SDL_Texture *macOSCloseTex;
        //SDL_Texture *macOSMinimTex;
        Button mOSClose;
        //Button mOSMinim;
        TTF_Font *font;
        std::string p_text;
        SDL_Point mouse;
        int mx;
        int my;
};