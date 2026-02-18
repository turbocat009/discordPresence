#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Entity.hpp"

Entity::Entity(Vector2f p_pos, Vector2f p_size, SDL_Texture* p_tex) 
    :pos(p_pos), size(p_size), tex(p_tex) {
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = size.x;
    currentFrame.h = size.y;

    collisionBox.x = static_cast<int>(p_pos.x);
    collisionBox.y = static_cast<int>(p_pos.y);
    collisionBox.w = static_cast<int>(p_size.x);
    collisionBox.h = static_cast<int>(p_size.y);

    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
}

SDL_Texture* Entity::getTex() {
    return tex;
}

SDL_Rect Entity::getCurrentFrame() {
    return currentFrame;
}

void Entity::setCurrentFrame(int maxX, int maxY, bool repeat) {
    if((maxX - currentFrame.w) ==  currentFrame.x) {
        if ((maxY - currentFrame.h) == currentFrame.y) {
            if (repeat) {
                currentFrame.x = 0;
                currentFrame.y = 0;
            } else {
                return;
            }
        }
        else {
            currentFrame.x = 0;
            currentFrame.y += currentFrame.h;
        }
    }
    else {
        currentFrame.x += currentFrame.w;
    }
    
}

bool Entity::collidesWith(const Entity &other) {
    return SDL_HasIntersection(&collisionBox, &other.collisionBox);
}

void Entity::render(SDL_Renderer *renderer, SDL_Window *wind) {

    SDL_Rect srce;
    srce.x = currentFrame.x;
    srce.y = currentFrame.y;
    srce.w = w;
    srce.h = h;

    //On dest.y we add 48 to fix the text position because of the ControlMenu
    SDL_Rect dest;
    dest.x = pos.x;
    dest.y = pos.y + 36;
    dest.w = size.x;
    dest.h = size.y;
    SDL_RenderCopy(renderer, tex, &srce, &dest);
}

void Entity::destroy() {
    /*if (tex) {
        SDL_DestroyTexture(tex);
        tex = nullptr;
    }*/
    pos.x = -1000.0f;
    pos.y = -1000.0f;

}