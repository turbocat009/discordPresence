#pragma once
#include "Entity.hpp"
#include "Player.hpp"
#include "RenderWindow.hpp"

class Button : public Entity {
    public:
        Button(Vector2f p_pos, Vector2f p_size, SDL_Texture *p_tex, std::string p_text, RenderWindow *win);
        void Update(float deltaTime);
        void Render(SDL_Renderer *renderer);
        void checkCollisions(Player &player);
        void setFont(TTF_Font *f) { m_font = f; }
        TTF_Font *getFont() const { return m_font; }
        bool isHovered();
        bool isPressed();
        void consumePress() { pressed = false; }  // Reset pressed state after consuming it
        void changeButtonUIState();
        void handleEvent(SDL_Event &ev);
    private : 
        SDL_Window *wind;
        RenderWindow *window;
        std::string text;
        TTF_Font *m_font;
        /*SDL_Color b_color;*/
        SDL_Point mouse;
        int mx;
        int my;
        bool pressed;
        int txtX;
        int txtY;
        int textWidth;
};