#include "Object.hpp"
#include <SDL2/SDL.h>

Object::Object(Vector2f p_pos, Vector2f p_size, SDL_Texture *p_tex)
    : Entity(p_pos, p_size, p_tex) {}

void Object::Update(float deltaTime) {
    Entity::update(deltaTime);
}

void Object::Render(SDL_Renderer *renderer, SDL_Window *wind) {
    Entity::render(renderer, wind);
}

void Object::checkCollisions(Player &player) {
    if (this->collidesWith(player)) {
        std::cout << "Collision detected between Object and Player!" << std::endl;
        this->destroy();
        pos.x = rand() % 500;
        pos.y = rand() % 500;
        player.setMaxSpeed(400.0f);
    }
}
