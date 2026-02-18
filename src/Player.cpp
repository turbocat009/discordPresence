#include "Player.hpp"
#include <iostream>
#include <SDL2/SDL.h>

Player::Player(Vector2f p_pos, Vector2f p_size, SDL_Texture *p_tex /*, SDL_Scancode controls[]*/, RenderWindow *win)
    : Entity(p_pos, p_size, p_tex),
      window(win),
      attackCooldown(1000),
      lastAttackTime(0),
      gunTexture(nullptr),
      isAttacking(false),
      attackDuration(200),
      gunOffset({30.0f, 10.0f}),
      gunSize({32.0f, 32.0f}),
      idleTex(window->loadTexture("res/gfx/Players/Red/IdleAnimDown.png")),
      movingTex(window->loadTexture("res/gfx/Players/Red/WalkAnimDown.png"))
{
    currentFrame.x = 0;
    currentFrame.y = 0;
    currentFrame.w = 576;
    currentFrame.h = 512;
}

void Player::handleInput(const Uint8 *keyState, float deltaTime, SDL_Scancode controls[]) {
    bool movingVertical = false;
    bool movingHorizontal = false;

    Uint32 currentTime = SDL_GetTicks();

    if (keyState[controls[0]]) {
        speedY -= acceleration * deltaTime;
        speedY = (speedY < -maxSpeed) ? -maxSpeed : speedY;
        movingVertical = true;
    }

    if (keyState[controls[1]]) {
        speedY += acceleration * deltaTime;
        speedY = (speedY > maxSpeed) ? maxSpeed : speedY;
        movingVertical = true;
    }

    if (keyState[controls[2]]) {
        speedX += acceleration * deltaTime;
        speedX = (speedX > maxSpeed) ? maxSpeed : speedX;
        movingHorizontal = true;
    }

    if (keyState[controls[3]]) {
        speedX -= acceleration * deltaTime;
        speedX = (speedX < -maxSpeed) ? -maxSpeed : speedX;
        movingHorizontal = true;
    }

    if (!movingHorizontal) {
        speedX = speedX > 0 ? std::max(0.0f, speedX - acceleration * deltaTime) 
                           : std::min(0.0f, speedX + acceleration * deltaTime);
    }

    if (!movingVertical) {
        speedY = speedY > 0 ? std::max(0.0f, speedY - acceleration * deltaTime)
                           : std::min(0.0f, speedY + acceleration * deltaTime);
    }

    if (keyState[controls[4]] && canAttack && (currentTime - lastAttackTime >= attackCooldown)) {
        Attack();
        lastAttackTime = currentTime;
    }

    face();
    Move(deltaTime);
}

void Player::Move(float deltaTime) {
    pos.x += speedX * deltaTime;
    pos.y += speedY * deltaTime;
}

void Player::Attack() {
    std::cout << "Attack!" << std::endl;
    isAttacking = true;
}

void Player::Update(float deltaTime) {
    Entity::update(deltaTime);

    Uint32 currentTime = SDL_GetTicks();
    if (isAttacking && (currentTime - lastAttackTime >= attackDuration)) {
        isAttacking = false;
    }
}

void Player::Render(SDL_Renderer *renderer, SDL_Window *wind) {
    // Render the player
    //Entity::render(renderer, wind);
    int winW, winH;
    SDL_GetWindowSize(wind, &winW, &winH);

    const int baseW = 1024;
    const int baseH = 576;

    float scaleX = (float)winW / baseW;
    float scaleY = (float)winH / baseH;
    float scale = std::min(scaleX, scaleY);

    float offsetX = (winW - (baseW * scale)) / 2;
    float offsetY = (winH - (baseH * scale)) / 2;



    SDL_Texture *playerStat;
    //IdleAnim
    if (speedX == 0 && speedY == 0) {
        playerStat = idleTex;
    } else {
        playerStat = movingTex;
    }

    SDL_Rect src;
    src.x = currentFrame.x;
    src.y = currentFrame.y;
    src.w = currentFrame.w;
    src.h = currentFrame.h;

    SDL_Rect dest;
    dest.x = static_cast<int>(offsetX + pos.x * scale);
    dest.y = static_cast<int>(offsetY + pos.y * scale);
    dest.w = static_cast<int>(size.x * scale);
    dest.h = static_cast<int>(size.y * scale);
    SDL_RenderCopy(renderer, playerStat, &src, &dest);

    // Render the gun if attacking
    if (isAttacking && gunTexture != nullptr) {
        SDL_Rect gunDest;
        gunDest.x = static_cast<int>(pos.x + gunOffset.x);
        gunDest.y = static_cast<int>(pos.y + gunOffset.y);
        gunDest.w = static_cast<int>(gunSize.x);
        gunDest.h = static_cast<int>(gunSize.y);

        SDL_RenderCopy(renderer, gunTexture, nullptr, &gunDest);
    }
}

void Player::playerCollision(int screenWidth, int screenHeight) {
    // Screen Collision
    if (pos.x < 0) {
        pos.x = 0;
    }
    if (pos.y < 0) {
        pos.y = 0;
    }
    if (pos.x + size.x > screenWidth) {
        pos.x = screenWidth - size.x;
    }
    if (pos.y + size.y > screenHeight) {
        pos.y = screenHeight - size.y;
    }

    // Collision with other players
    for (Player *player : otherPlayers)
    {
        if (!player)
            continue; // skip null pointers (defensive)
        if (player == this)
            continue; // don't collide with self

        if (this->collidesWith(*player))
        { // dereference to pass a Player&
            std::cout << "Collision" << std::endl;
        }
    }
}

void Player::face() {
    if (speedX == 0){
        if (speedY > 0) {
            std::cout << "facing amunt" << std::endl;
            idleTex = window->loadTexture("res/gfx/Players/Red/IdleAnimDown.png");
            movingTex = window->loadTexture("res/gfx/Players/Red/WalkAnimDown.png");
        }
        if (speedY < 0) {
            std::cout << "facing avall" << std::endl;
            idleTex = window->loadTexture("res/gfx/Players/Red/IdleAnimUp.png");
            movingTex = window->loadTexture("res/gfx/Players/Red/WalkAnimUp.png");
        }
    }
    if (speedY == 0) {
        if (speedX > 0) {
            std::cout << "facing dreta" << std::endl;
            idleTex = window->loadTexture("res/gfx/Players/Red/IdleAnimRight.png");
            movingTex = window->loadTexture("res/gfx/Players/Red/WalkAnimRight.png");
        }
        if (speedX < 0) {
            std::cout << "facing esquerra" << std::endl;
            idleTex = window->loadTexture("res/gfx/Players/Red/IdleAnimLeft.png");
            movingTex = window->loadTexture("res/gfx/Players/Red/WalkAnimLeft.png");
        }
    }

    if (speedX > 0) {
        if (speedY > 0) {
            std::cout << "facing nord-est" << std::endl;
            idleTex = window->loadTexture("res/gfx/Players/Red/IdleAnimUpRight.png");
            movingTex = window->loadTexture("res/gfx/Players/Red/WalkAnimUpRight.png");
        }
        if (speedY < 0) {
            std::cout << "facing sud-est" << std::endl;
            idleTex = window->loadTexture("res/gfx/Players/Red/IdleAnimDownLeft.png");
            movingTex = window->loadTexture("res/gfx/Players/Red/WalkAnimDownLeft.png");
        }
    }

    if (speedX < 0) {
        if (speedY > 0) {
            std::cout << "facing nord-oest" << std::endl;
        }
        if (speedY < 0) {
            std::cout << "facing sud-oest" << std::endl;
        }
    }
}
