#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Entity.hpp"

class RenderWindow {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width;
    int height;
    float dpiScaleX;
    float dpiScaleY;

public:
    RenderWindow(const char *p_title, int p_w, int p_h);
    SDL_Texture *loadTexture(const char *p_filePath);
    TTF_Font *loadFont(const char *p_filePath, int p_size);
    int getRefreshRate();
    void cleanUp();
    void clear();
    void render(Entity &p_entity, Vector2f p_size);
    void display();
    SDL_Window *getWindow() const;
    SDL_Renderer *getRenderer() { return renderer; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    SDL_Texture *createTextTexture(TTF_Font *font, const std::string &text, SDL_Color color);
    void renderText(TTF_Font *font, const std::string &text, SDL_Color color, int x, int y);
    void setMinimized();
};