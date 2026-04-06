#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

#include "RenderWindow.hpp"
#include "Entity.hpp"

RenderWindow::RenderWindow(const char *p_title, int p_w, int p_h)
    : window(nullptr), renderer(nullptr), width(p_w), height(p_h)
{
    int winW, winH;
    int drawW, drawH;



    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    if (window == nullptr)
        std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_GetWindowSize(window, &winW, &winH);
    SDL_GetRendererOutputSize(renderer, &drawW, &drawH);

    dpiScaleX = (float)drawW / winW;
    dpiScaleY = (float)drawH / winH;

    SDL_RenderSetScale(renderer, dpiScaleX, dpiScaleY);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath) {
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == NULL)
        std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

    return texture;
}

TTF_Font *RenderWindow::loadFont(const char *p_filePath, int p_size) {
    TTF_Font *font = TTF_OpenFont(p_filePath, p_size);

    if (!font)
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;

    TTF_SetFontHinting(font, TTF_HINTING_LIGHT);
    return font;
}

int RenderWindow::getRefreshRate() {
    int displayIndex = SDL_GetWindowDisplayIndex(window);

    SDL_DisplayMode mode;

    SDL_GetDisplayMode(displayIndex, 0, &mode);

    return mode.refresh_rate;
}

void RenderWindow::cleanUp() {
    SDL_DestroyWindow(window);
}

void RenderWindow::clear() {
    SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity &p_entity, Vector2f p_size) {
    SDL_Rect src;
    src.x = p_entity.getCurrentFrame().x;
    src.y = p_entity.getCurrentFrame().y;
    src.w = p_entity.getCurrentFrame().w;
    src.h = p_entity.getCurrentFrame().h;

    //On dst.y we add 36 to fix the text position because of the ControlMenu
    SDL_Rect dst;
    dst.x = p_entity.getPos().x;
    dst.y = p_entity.getPos().y + 36;
    dst.w = p_size.x;
    dst.h = p_size.y;

    SDL_RenderCopy(renderer, p_entity.getTex(), &src, &dst);
}

void RenderWindow::display() {
    SDL_RenderPresent(renderer);
}

SDL_Window *RenderWindow::getWindow() const {
    return window;
}

SDL_Texture *RenderWindow::createTextTexture(TTF_Font *font, const std::string &text, SDL_Color color) {
    if (!font || !renderer)
        return nullptr;
    SDL_Surface *surf = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surf)
        return nullptr;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    return tex;
}

void RenderWindow::renderText(TTF_Font *font, const std::string &text, SDL_Color color, int x, int y) {
    SDL_Texture *tex = createTextTexture(font, text, color);
    if (!tex || tex == nullptr)
        return;

    //On dst.y we add 36 to fix the text position because of the ControlMenu
    SDL_Rect dst;
    SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
    dst.x = x;
    dst.y = y + 36;
    dst.w = dst.w;
    dst.h = dst.h;
    SDL_RenderCopy(renderer, tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
}

void RenderWindow::setMinimized() {
    SDL_MinimizeWindow(window);
}