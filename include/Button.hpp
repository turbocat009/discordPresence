#pragma once
#include "Entity.hpp"
#include "Player.hpp"
#include "RenderWindow.hpp"
#include <functional>

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
        void consumePress() { pressed = false; } 
        bool pollClicked(); 
        void setOnClick(std::function<void()> fn) { onClick = fn; }
        bool hasOnClick() const { return (bool)onClick; }
        void changeButtonUIState();
        void handleEvent(SDL_Event &ev);
        void setParentAlpha(float a) { parentAlpha = a; }
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
        bool clicked;
        std::function<void()> onClick;
        float parentAlpha = 1.0f;
};