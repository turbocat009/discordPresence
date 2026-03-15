#pragma once
#include "Scenes/Scene.hpp"
#include "RenderWindow.hpp"
#include "Button.hpp"
#include "InputText.hpp"


#include <string>


class SelectionScene1 : public Scene
{
public:
    SelectionScene1(RenderWindow *win, SDL_Texture *bgTex);
    void handleEvent(SDL_Event &ev, bool &running) override;
    void render() override;
    void update(float dt) override;
    bool startRequestedContinue() const { return start_; }
    bool startRequestedBack() const { return back_; }
    SDL_Texture *bg;
private:
    RenderWindow *window;
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
    std::vector<Entity> entities;
    //bool continue_ = false;
    Button applyBtn;
    Button logOutBtn;
    Input name;
    Input desc;
    Input type;
    TTF_Font *textFont;
    std::string Name;
    std::string Desc;
    std::string Type;
    bool randomVariableToMakeItWork;
    std::string LocalName;
    std::string LocalDesc;
    std::string LocalType;
};