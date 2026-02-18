#pragma once
#include "Entity.hpp"
#include "Player.hpp"
#include "RenderWindow.hpp"

class Input : public Entity {
    public:
        Input(Vector2f p_pos, Vector2f p_size, SDL_Texture *p_tex, std::string p_text, int txtLenght, char p_button, bool bW, bool leftIMG, bool isCentered, RenderWindow *win);
        void Update(float deltaTime);
        void Render(SDL_Renderer *renderer);
        void checkCollisions(Player &player);
        void setFont(TTF_Font *f) { m_font = f; }
        TTF_Font *getFont() const { return m_font; }
        bool isHovered();
        void setTexture(SDL_Texture *texture) { tex = texture; }
        bool isPressed();
        void consumePress() { pressed = false; }  // Reset pressed state after consuming it
        void changeButtonUIState();
        void handleEvent(SDL_Event &ev);
        void write(SDL_Event &ev);
        std::string getText();
    private : SDL_Window *wind;
        RenderWindow *window;
        std::string text;
        TTF_Font *m_font;
        char button;
        SDL_Color b_color;
        bool black;
        bool left;
        bool centered;
        int mx;
        int my;
        SDL_Point mouse;
        bool pressed;
        int txtX;
        int txtY;
        int textWidth;
        int length;
};