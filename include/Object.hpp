#pragma once
#include "Entity.hpp"
#include <SDL2/SDL.h>
#include "Player.hpp"

class Object : public Entity {
    public:
        Object(Vector2f p_pos, Vector2f p_size, SDL_Texture *p_tex);
        void Update(float deltaTime);
        void Render(SDL_Renderer *renderer, SDL_Window *wind);
        void checkCollisions(Player &player);
};