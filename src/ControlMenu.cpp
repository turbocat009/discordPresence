#include "ControlMenu.hpp"
#include <SDL2/SDL.h>

Control::Control(Vector2f p_pos, Vector2f p_size, SDL_Texture *p_tex, RenderWindow *win, std::string text)
    : Entity(p_pos, p_size, p_tex),
      window(win),
      macOSCloseTex(window->loadTexture("res/gfx/mOSX.png")),
      macOSMinimTex(window->loadTexture("res/gfx/mOS-.png")),
      mOSClose(Vector2f(pos.x + 10, (pos.y - 36) + 10), Vector2f(16, 16), macOSCloseTex, " ", ' ', true, false, true, window),
      mOSMinim(Vector2f(pos.x + 32, (pos.y - 36) + 10), Vector2f(16, 16), macOSMinimTex, " ", ' ', true, false, true, window),
      p_text(text)
{
    font = window->loadFont("res/fonts/SS3_Regular.ttf", 20);
}

void Control::HandleEvents(SDL_Event &ev, bool &running) {
    mOSClose.handleEvent(ev);
    mOSMinim.handleEvent(ev);

    // Check button presses and consume them immediately so they don't stay true
    if (mOSClose.isPressed()) {
        running = false;
        mOSClose.consumePress();
    }
    if (mOSMinim.isPressed()) {
        window->setMinimized();
        mOSMinim.consumePress();
    }
}

void Control::Update(float deltaTime) {
    Entity::update(deltaTime);
    mOSClose.Update(deltaTime);
    mOSMinim.Update(deltaTime);
    SDL_GetMouseState(&mx, &my);
    mouse = {mx, my};
    moveWindow();
}

void Control::Render(SDL_Renderer *renderer, SDL_Window *wind) {
    pos.y = -38;
    Entity::render(renderer, wind);
    mOSClose.w = 46;
    mOSClose.render(renderer, wind);
    mOSMinim.w = 46;
    mOSMinim.render(renderer, wind);
    window->renderText(font, p_text, {255, 255, 255, 255}, pos.x + 75, pos.y + 5);
}

void Control::checkCollisions(Player &player) {
    if (this->collidesWith(player)) {
        std::cout << "Collision detected between Object and Player!" << std::endl;
        this->destroy();
        pos.x = rand() % 500;
        pos.y = rand() % 500;
        player.setMaxSpeed(400.0f);
    }
}

void Control::destroyPopUP(SDL_Event &ev) {
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    SDL_Point mouse = {mx, my};
    bool hovered = mOSClose.isHovered();
    if (isAnimationComplete()) {
        if (mOSClose.isPressed()) {
            currentFrame.x = 0;
            currentFrame.y = 0;
            stopAnimation();
        }
    }
}

void Control::moveWindow() {
    SDL_Rect controlRect = {static_cast<int>(pos.x), static_cast<int>(pos.y) + 36, 
                            static_cast<int>(size.x), static_cast<int>(size.y)};
    SDL_Point mousePoint = {mx, my};

    static bool isDragging = false;
    static int dragOffsetX = 0;
    static int dragOffsetY = 0;

    int mouseX, mouseY;
    Uint32 mouseButtons = SDL_GetGlobalMouseState(&mouseX, &mouseY);
    bool leftDown = mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT);

    // Start drag (hit-test still uses window-relative coords)
    if (!isDragging && leftDown && SDL_PointInRect(&mousePoint, &controlRect)) {
        dragOffsetX = mouseX;
        dragOffsetY = mouseY;

        int winX, winY;
        SDL_GetWindowPosition(window->getWindow(), &winX, &winY);

        dragOffsetX -= winX;
        dragOffsetY -= winY;

        isDragging = true;
    }

    // Drag
    if (isDragging && leftDown) {
        SDL_SetWindowPosition(
            window->getWindow(),
            mouseX - dragOffsetX,
            mouseY - dragOffsetY);
    }

    // Stop drag
    if (!leftDown) {
        isDragging = false;
    }
}
