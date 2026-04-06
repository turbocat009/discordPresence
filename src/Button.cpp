#include "Button.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Button::Button(Vector2f p_pos, Vector2f p_size, SDL_Texture *p_tex, std::string p_text, RenderWindow *win)
    : Entity(p_pos, p_size, p_tex), window(win), text(p_text)
{
    m_font = window->loadFont("res/fonts/SS3_Bold.ttf", 32);

    w = w/4;
    pressed = false;

    TTF_SizeText(m_font, text.c_str(), &textWidth, nullptr);
    txtX = pos.x + size.x / 2 - textWidth / 2;
    txtY = pos.y + size.y / 2 - 26;

    wind = window->getWindow();
}

void Button::Update(float deltaTime) {
    Entity::update(deltaTime);
    SDL_GetMouseState(&mx, &my);
    mouse = {mx, my};
    changeButtonUIState();
}

void Button::Render(SDL_Renderer *renderer) {

    Entity::render(renderer, wind);

    if (!m_font)
        std::cerr << "Font not loaded!" << std::endl;

    
    window->renderText(m_font, text, {255, 255, 255, 255}, txtX, txtY);
}

void Button::handleEvent(SDL_Event &ev) {
    //Detect click
    if (ev.type == SDL_MOUSEBUTTONDOWN &&
        ev.button.button == SDL_BUTTON_LEFT &&
        isHovered() && !pressed) {
            pressed = true;
        }
    else if (ev.type == SDL_MOUSEBUTTONUP &&
        ev.button.button == SDL_BUTTON_LEFT && pressed) {
            pressed = false;
        }
    
}

void Button::checkCollisions(Player &player) {
    if (this->collidesWith(player)) {
        std::cout << "Collision detected between Button and Player!" << std::endl;
        this->destroy();
        pos.x = rand() % 500;
        pos.y = rand() % 500;
        player.setMaxSpeed(400.0f);
    }
}

bool Button::isHovered() {
    //We add 48 to the y position because of the ControlMenu
    SDL_Rect buttonRect = { static_cast<int>(pos.x), static_cast<int>(pos.y + 36),
                            static_cast<int>(size.x), static_cast<int>(size.y) };
    return SDL_PointInRect(&mouse, &buttonRect);
}

bool Button::isPressed() {
    return pressed;
}

void Button::changeButtonUIState () {
    currentFrame.x = 0;
    txtY = pos.y + size.y / 2 - 26;

    if (isHovered()) {
        if (isPressed()){
            currentFrame.x = w * 3;
            txtY = pos.y + size.y / 2 - 20;
        }
        currentFrame.x = w;
    }
    if (isPressed()) {
        currentFrame.x = w * 2;
        txtY = pos.y + size.y / 2 - 20;
    }
}
