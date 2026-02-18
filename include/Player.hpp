#pragma once
#include "Entity.hpp"
#include "RenderWindow.hpp"
#include <SDL2/SDL.h>
#include <vector>

class Player : public Entity {
    public:
        Player(Vector2f p_pos, Vector2f p_size, SDL_Texture *p_tex/*, SDL_Scancode controls[]*/, RenderWindow *win);
        void setGunTexture(SDL_Texture *tex) { gunTexture = tex; }
        void handleInput(const Uint8 *keyState, float deltaTime, SDL_Scancode controls[]);
        void Move(float deltaTime);
        void Attack();
        void Update(float deltaTime); // New method
        void Render(SDL_Renderer *renderer, SDL_Window *wind);
        void playerCollision(int screenWidth, int screenHeight);
        void setSpeed(Vector2f newSpeed) { speedX = newSpeed.x; speedY = newSpeed.y; }
        void setMaxSpeed(float newMaxSpeed) { maxSpeed = newMaxSpeed; }
        void face();
        float playerID = rand() % 1000;
    private:
        RenderWindow *window;
        const float acceleration = 1000.0f;
        float speedX = 0.0f;
        float speedY = 0.0f;
        //const float friction = 0.9f;
        float maxSpeed = 250.0f;
        bool canAttack = true;
        Uint32 attackCooldown;
        Uint32 lastAttackTime;
        SDL_Texture *gunTexture;
        bool isAttacking;
        Uint32 attackDuration;
        Vector2f gunOffset;
        Vector2f gunSize;
        SDL_Texture *idleTex;
        SDL_Texture *movingTex;
        std::vector<Player*> otherPlayers;
};