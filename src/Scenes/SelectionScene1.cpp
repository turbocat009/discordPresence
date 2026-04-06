#include "Scenes/SelectionScene1.hpp"
#include "Entity.hpp"
#include "Button.hpp"
#include "DiscordFunc.hpp"
#include "ImageDownloader.hpp"
#include "ImgUtils.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <tinyfd/tinyfiledialogs.h>


SelectionScene1::SelectionScene1(RenderWindow *win, SDL_Texture *bgTex)
    : bg(bgTex),
      window(win),
      temp(std::filesystem::temp_directory_path()),
      Round(window->loadTexture("res/gfx/Round.png")),
      whiteBackground(window->loadTexture("res/gfx/white-background.png")),
      btnTex(window->loadTexture("res/gfx/Button_Green.png")),
      logOutBtnTex(window->loadTexture("res/gfx/logOutBtn.png")),
      smallTxt(window->loadTexture("res/gfx/smallTXT.png")),
      bigTxt(window->loadTexture("res/gfx/bigTXT.png")),
      imggg(window->loadTexture((temp.string() + "image.png").c_str())),
      imgHover(window->loadTexture("res/gfx/ImageHover.png")),
      applyBtn(Vector2f(window->getWidth() / 2 - 157, window->getHeight() / 2 + 125), Vector2f(314, 104), btnTex, "Apply Changes", window),
      logOutBtn(Vector2f(window->getWidth() / 2 - 250, window->getHeight() / 2 + 150), Vector2f(70, 70), logOutBtnTex, "", window),
      setButton(Vector2f(75, 100), Vector2f(140, 140), imgHover, "", window),
      name(Vector2f(window->getWidth() / 2 - 50, window->getHeight() / 2 - 175), Vector2f(297, 50), smallTxt, getActName(), 16, window),
      desc(Vector2f(window->getWidth() / 2 - 50, window->getHeight() / 2 - 100), Vector2f(297, 50), smallTxt, getActDesc(), 16, window),
      type(Vector2f(75, window->getHeight() / 2 - 25), Vector2f(459, 50), bigTxt, getActType(), 23, window),
      Image(Vector2f(75, 100), Vector2f(140, 140), imggg),
      Rounded(Vector2f(75, 100), Vector2f(140, 140), Round)
{
    fontX = (window->getWidth() - 250) / 2;
    fontY = window->getHeight() - 100;

    Name = name.getText();
    Desc = desc.getText();
    Type = type.getText();
    IMGG = getActIMG();

    
    randomVariableToMakeItWork = false;
    
    textFont = window->loadFont("res/fonts/SS3_Bold.ttf", 20);
    
    shit = true;
    
    DiscordFunc::setPresence(Name, Desc, Type, IMGG);
}

void SelectionScene1::handleEvent(SDL_Event &ev, bool &running) {
    if (ev.type == SDL_QUIT)
        running = false;
    if (ev.type == SDL_KEYDOWN) { 
        if (ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            running = false;
    }
    if (logOutBtn.isPressed()) {
        DiscordFunc::logOut();
    }
    if (applyBtn.isPressed()) {
        Name = LocalName;
        Desc = LocalDesc;
        Type = LocalType;
        modifyUser("https://discord-represence-backend.vercel.app/", DiscordFunc::getCurrentID(), Name, Desc, Type);
        DiscordFunc::setPresence(Name, Desc, Type, IMGG);
    }
    if(setButton.isPressed() && shit) {
        shit = false;
        const char *home = getenv("HOME");
        const char *filters[] = {"*.png", "*.jpg", "*.jpeg", "*.bmp", "*.gif"};
        const char *file = tinyfd_openFileDialog(
            "Select an image",
            home,             
            5,                
            filters,        
            "Image files",  
            0
        );
        if (file) {
            std::cout << "Selected file: " << file << std::endl;
            setIMG("https://discord-represence-backend.vercel.app/", DiscordFunc::getCurrentID(), modifyImg(file));
            DiscordFunc::setPresence(Name, Desc, Type, IMGG);
            shit = true;
        }
        else {
            std::cout << "No file selected." << std::endl;
            shit = true;
        }
        getIMG(getActIMG());
        IMGG = getActIMG();
        Image.setTexture(window->loadTexture((temp.string() + "image.png").c_str()));
    }


    applyBtn.handleEvent(ev);
    logOutBtn.handleEvent(ev);
    setButton.handleEvent(ev);
    name.handleEvent(ev);
    desc.handleEvent(ev);
    type.handleEvent(ev);

    if (DiscordFunc::getToken() == "nullptr") {
        logedOut_ = true;
    }

}

void SelectionScene1::render() {    
    applyBtn.Render(window->getRenderer());
    logOutBtn.Render(window->getRenderer());
    name.Render(window->getRenderer());
    desc.Render(window->getRenderer());
    type.Render(window->getRenderer());
    Image.render(window->getRenderer(), window->getWindow());
    setButton.Render(window->getRenderer());
    Rounded.render(window->getRenderer(), window->getWindow());

    window->renderText(textFont, "Name", {255, 255, 255, 255}, window->getWidth() / 2 - 50, window->getHeight() / 2 - 200);
    window->renderText(textFont, "Description", {255, 255, 255, 255}, window->getWidth() / 2 - 50, window->getHeight() / 2 - 125);
    window->renderText(textFont, "Type", {255, 255, 255, 255}, 75, window->getHeight() / 2 - 50);

    if (setButton.isHovered()) {
        window->renderText(textFont, "Click here", {255, 255, 255, 255}, 100, 130);
        window->renderText(textFont, "to set", {255, 255, 255, 255}, 125, 155);
        window->renderText(textFont, "an Image", {255, 255, 255, 255}, 105, 180);
    }
}

void SelectionScene1::update(float dt) {
    if (controllerType.empty()) {
        controllerType = "Controller Type";
    }

    applyBtn.Update(dt);
    logOutBtn.Update(dt);
    setButton.Update(dt);
    name.Update(dt);
    desc.Update(dt);
    type.Update(dt);
    Rounded.update(dt);
    Image.update(dt);

    LocalName = name.getText();
    LocalDesc = desc.getText();
    LocalType = type.getText();



}


