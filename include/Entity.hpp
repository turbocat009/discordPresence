#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"

class Entity {
    public:
        Entity(Vector2f p_pos, Vector2f p_size, SDL_Texture *p_tex);
        virtual ~Entity() = default;
        Vector2f &getPos() {
            return pos;
        }
        SDL_Texture* getTex();
        SDL_Rect getCurrentFrame();
        void setCurrentFrame(int maxX, int maxY, bool repeat);
        // Animation control (public API)
        void playAnimation(int maxX, int maxY, float delay = 0.12f, bool repeat = false) {
            animMaxX = maxX;
            animMaxY = maxY;
            animDelay = delay;
            animRepeat = repeat;
            animPlaying = true;
            animTimer = 0.0f;
            currentFrame.x = 0;
            currentFrame.y = 0;
        }
        void stopAnimation() { animPlaying = false; }
        bool isAnimationComplete() const { return (currentFrame.x == (animMaxX - currentFrame.w) && currentFrame.y == (animMaxY - currentFrame.h)); }
        virtual void update(float deltaTime) {
            collisionBox.x = static_cast<int>(pos.x);
            collisionBox.y = static_cast<int>(pos.y);
        }
        bool collidesWith(const Entity &other);
        void drawCollisionBox(SDL_Renderer *renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &collisionBox);
        }
        void destroy();
        virtual void render(SDL_Renderer *renderer, SDL_Window *wind);
        int w;
        int h;
    private:
    protected:
        Vector2f pos;
        Vector2f size;
        SDL_Texture* tex;
        SDL_Rect currentFrame;
        SDL_Rect collisionBox;
        float initialX = pos.x;
        float initialY = pos.y;
        float sizeW = size.x;
        float sizeH = size.y;
        // Animation state
        bool animPlaying = false;
        int animMaxX = 0;
        int animMaxY = 0;
        float animTimer = 0.0f;
        float animDelay = 0.12f;
        bool animRepeat = false;
};