#pragma once
#include "Entity.hpp"
#include "Player.hpp"
#include "RenderWindow.hpp"
#include "Button.hpp"
#include <vector>
#include <string>

class PopUp : public Entity {
    public:
        PopUp(Vector2f p_pos, Vector2f p_size, SDL_Texture *p_tex, std::string p_text, std::string p_title, int p_type, RenderWindow *win);
        void Update(float deltaTime);
        void Render(SDL_Renderer *renderer);
        void checkCollisions(Player &player);
        void setFont(TTF_Font *f) { m_font = f; }
        TTF_Font *getFont() const { return m_font; }
        bool isHovered();
        bool isPressed();
        void consumePress() { pressed = false; } 
        void handleEvent(SDL_Event &ev);
        bool consumesEvent() const { return consumed; }
        void clearConsumed() { consumed = false; }
        void setErrorCode(std::string errorCode);
        void setTitle(std::string new_title);
        void setShowErrorCode(bool set);
        void destroy(); 
        std::vector<std::string> wrappedLines;
        void startHide();
        void show();
        const std::string &getMessage() const { return text; }
        const std::string &getTitle() const { return title; }
        bool getIsDestroyed() const { return isDestroyed; }
    private : 
        SDL_Window *wind;
        RenderWindow *window;
        std::string text;
        std::string title;
        int type;
        std::string buttonText;
        TTF_Font *m_font;
        SDL_Texture *bgIMG;
        SDL_Texture *btnIMGGreen;
        SDL_Texture *btnIMGOrange;
        SDL_Point mouse;
        Entity bg;
        std::vector<Button> buttons;
        std::vector<std::string> lines;
        std::string errorMSG;
        int mx;
        int my;
        bool pressed;
        bool isDestroyed;
        bool consumed;
        int titleX;
        int titleY;
        int txtX;
        int txtY;
        int textWidth;
        bool showErrorCode;
        // Animation state
        enum AnimState { ANIM_HIDDEN = 0, ANIM_SHOWING = 1, ANIM_VISIBLE = 2, ANIM_HIDING = 3 };
        AnimState animState = ANIM_VISIBLE;
        float animTimer = 0.0f;
        float animDuration = 0.18f;
        float anim = 1.0f;
        float offsetY = 0.0f;
        std::vector<float> btnBaseY;
};