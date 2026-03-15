#pragma once
#include "Entity.hpp"
#include "Player.hpp"
#include "RenderWindow.hpp"

class Input : public Entity {
    public:
        Input(Vector2f p_pos, Vector2f p_size, SDL_Texture *p_tex, std::string p_text, int txtLenght, RenderWindow *win);
        void Update(float deltaTime);
        void Render(SDL_Renderer *renderer);
        void checkCollisions(Player &player);
        void setFont(TTF_Font *f) { m_font = f; }
        TTF_Font *getFont() const { return m_font; }
        bool isHovered();
        void setTexture(SDL_Texture *texture) { tex = texture; }
        bool isPressed();
        void consumePress() { pressed = false; }  
        void changeButtonUIState();
        void handleEvent(SDL_Event &ev);
        void write(SDL_Event &ev);
        std::string getText();
        void setText(std::string Text);
    private : SDL_Window *wind;
        RenderWindow *window;
        std::string text;
        TTF_Font *m_font;
        //SDL_Color b_color;
        int mx;
        int my;
        SDL_Point mouse;
        bool pressed;
        int txtX;
        int txtY;
        int textWidth;
        int length;
};