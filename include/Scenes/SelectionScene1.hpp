#pragma once
#include "Scenes/Scene.hpp"
#include "RenderWindow.hpp"
#include "Button.hpp"
#include "InputText.hpp"
#include "Entity.hpp"


#include <string>
#include <iostream>
#include <filesystem>

class SelectionScene1 : public Scene
{
public:
    SelectionScene1(RenderWindow *win, SDL_Texture *bgTex);
    void handleEvent(SDL_Event &ev, bool &running) override;
    void render() override;
    void update(float dt) override;
    bool startRequestedContinue() const { return start_; }
    bool startRequestedBack() const { return logedOut_; }
    SDL_Texture *bg;
private:
    RenderWindow *window;
    std::filesystem::path temp;
    SDL_Texture *Round;
    SDL_Texture *whiteBackground;
    SDL_Texture *btnTex;
    bool start_ = false;
    bool back_ = false;
    int fontX;
    int fontY;
    std::string controllerType;
    SDL_Texture *logOutBtnTex;
    SDL_Texture *smallTxt;
    SDL_Texture *bigTxt;
    SDL_Texture *imggg;
    SDL_Texture *imgHover;
    std::vector<Entity> entities;
    bool logedOut_ = false;
    Button applyBtn;
    Button logOutBtn;
    Button setButton;
    Input name;
    Input desc;
    Input type;
    TTF_Font *textFont;
    std::string Name;
    std::string Desc;
    std::string Type;
    std::string IMGG;
    bool randomVariableToMakeItWork;
    std::string LocalName;
    std::string LocalDesc;
    std::string LocalType;
    Entity Image;
    Entity Rounded;
    bool shit;
};