#include "PopUp.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <thread>

static float easeInOut(float t) {
    return t * t * (3.0f - 2.0f * t);
}

PopUp::PopUp(Vector2f p_pos, Vector2f p_size, SDL_Texture *p_tex, std::string p_text, std::string p_title, int p_type, RenderWindow *win)
    : Entity(p_pos, p_size, p_tex), window(win), text(p_text), title(p_title), type(p_type),
      bgIMG(window->loadTexture("res/gfx/bgIMG.png")),
      btnIMGGreen(window->loadTexture("res/gfx/Button_Green.png")),
      btnIMGOrange(window->loadTexture("res/gfx/Button_Orange.png")),
      bg(Vector2f(0, 0), Vector2f(window->getWidth(), window->getHeight()), bgIMG)
{
    m_font = window->loadFont("res/fonts/SS3_Bold.ttf", 32);

    pressed = false;
    isDestroyed = false;
    consumed = false;
    // start with show animation
    animState = ANIM_SHOWING;
    animTimer = 0.0f;
    animDuration = 0.18f;
    anim = 0.0f;
    offsetY = 32.0f;

    TTF_SizeText(m_font, title.c_str(), &textWidth, nullptr);
    titleX = pos.x + (size.x / 2)  - (textWidth / 2);
    titleY = pos.y + 5;
    txtX = pos.x + 25;
    txtY = pos.y + 60;

    // Split text into lines by '\n' for flexible rendering
    lines.clear();
    size_t start = 0;
    while (start <= text.length()) {
        size_t e = text.find('\n', start);
        if (e == std::string::npos) {
            lines.push_back(text.substr(start));
            break;
        }
        lines.push_back(text.substr(start, e - start));
        start = e + 1;
    }

    // Wrap lines to fit inside popup width
    wrappedLines.clear();
    int maxTextW = static_cast<int>(p_size.x) - 56; // same inset used when rendering
    for (auto &ln : lines) {
        std::string cur;
        std::istringstream iss(ln);
        std::string word;
        while (iss >> word) {
            std::string test = cur.empty() ? word : (cur + " " + word);
            int w = 0, h = 0;
            if (m_font)
                TTF_SizeText(m_font, test.c_str(), &w, &h);
            if (w > maxTextW && !cur.empty()) {
                wrappedLines.push_back(cur);
                cur = word;
            } else {
                cur = test;
            }
        }
        if (!cur.empty()) wrappedLines.push_back(cur);
    }

    switch (type) {
        case 0:
            buttons.emplace_back(Vector2f(p_pos.x + ((p_size.x / 2) - 100), p_pos.y + p_size.y - 80), Vector2f(200, 60), btnIMGGreen, "Accept", win);
            break;
        case 1:
            buttons.emplace_back(Vector2f(p_pos.x + 40, p_pos.y + p_size.y - 80), Vector2f(200, 60), btnIMGGreen, "Cancel", win);
            buttons.emplace_back(Vector2f(p_pos.x + p_size.x - 240, p_pos.y + p_size.y - 80), Vector2f(200, 60), btnIMGGreen, "Accept", win);
            break;
        case 2:
            buttons.emplace_back(Vector2f(p_pos.x + p_size.x - 240, p_pos.y + p_size.y - 80), Vector2f(200, 60), btnIMGGreen, "Accept", win);
            buttons.emplace_back(Vector2f(p_pos.x + 40, p_pos.y + p_size.y - 80), Vector2f(200, 60), btnIMGOrange, "Copy Error", win);
            break;
        case 3:
            buttons.emplace_back(Vector2f(p_pos.x + 20, p_pos.y + p_size.y - 90), Vector2f(120, 75), btnIMGGreen, "Cancel", win);
            buttons.emplace_back(Vector2f(p_pos.x + ((p_size.x / 2) - 60), p_pos.y + p_size.y - 90), Vector2f(120, 75), btnIMGOrange, "Copy Code", win);
            buttons.emplace_back(Vector2f(p_pos.x + p_size.x - 140, p_pos.y + p_size.y - 90), Vector2f(120, 75), btnIMGGreen, "Retry", win);
            break;
        case 4:
            buttons.emplace_back(Vector2f(p_pos.x + 40, p_pos.y + p_size.y - 80), Vector2f(200, 60), btnIMGGreen, "Cancel", win);
            buttons.emplace_back(Vector2f(p_pos.x + p_size.x - 240, p_pos.y + p_size.y - 80), Vector2f(200, 60), btnIMGGreen, "Update", win);
            break;
        case 5:
            buttons.emplace_back(Vector2f(p_pos.x - 100, p_size.y - 100), Vector2f(200, 60), btnIMGGreen, "Cancel", win);
            break;
        default:
            buttons.emplace_back(Vector2f(p_pos.x + ((p_size.x / 2) - 100), p_pos.y + p_size.y - 90), Vector2f(200, 75), btnIMGGreen, "Accept", win);
            break;
    }

    wind = window->getWindow();

    // capture base Y positions for buttons so they move with popup during animation
    btnBaseY.clear();
    for (auto &b : buttons) {
        btnBaseY.push_back(b.getPos().y);
    }
}

void PopUp::Update(float deltaTime) {
    if (!isDestroyed) {
        Entity::update(deltaTime);
        SDL_GetMouseState(&mx, &my);
        mouse = {mx, my};
        bg.update(deltaTime);
        // Update animation
        if (animState == ANIM_SHOWING) {
            animTimer = std::min(animTimer + deltaTime, animDuration);
            float progress = animTimer / animDuration;
            anim = easeInOut(progress);
            offsetY = (1.0f - anim) * 32.0f;
            if (animTimer >= animDuration) {
                animState = ANIM_VISIBLE;
                anim = 1.0f;
                offsetY = 0.0f;
            }
        } else if (animState == ANIM_HIDING) {
            animTimer = std::min(animTimer + deltaTime, animDuration);
            float progress = animTimer / animDuration;
            anim = easeInOut(1.0f - progress);
            offsetY = (1.0f - anim) * 32.0f;
            if (animTimer >= animDuration) {
                // finish hiding
                animState = ANIM_HIDDEN;
                isDestroyed = true;
            }
        }

        // Update buttons positions (move with popup) and update them
        for (size_t i = 0; i < buttons.size(); ++i) {
            auto &btn = buttons[i];
            if (i < btnBaseY.size()) {
                btn.getPos().y = btnBaseY[i] + offsetY;
            }
            btn.Update(deltaTime);
        }
    }
}

void PopUp::Render(SDL_Renderer *renderer) {
    if (!isDestroyed) {
        // render overlay with fade based on anim
        float overlayAlpha = 0.55f * anim;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, static_cast<Uint8>(overlayAlpha * 255));
        SDL_Rect full = {0, 0, window->getWidth(), window->getHeight()};
        SDL_RenderFillRect(renderer, &full);

        // render popup texture with vertical offset and alpha
        if (tex) {
            SDL_Rect srce = getCurrentFrame();
            SDL_Rect dest;
            dest.x = static_cast<int>(pos.x);
            dest.y = static_cast<int>(pos.y + 36 + offsetY);
            dest.w = static_cast<int>(size.x);
            dest.h = static_cast<int>(size.y);

            SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(tex, static_cast<Uint8>(anim * 255));
            SDL_RenderCopy(renderer, tex, &srce, &dest);
            SDL_SetTextureAlphaMod(tex, 255);
        }

        if (!m_font)
            std::cerr << "Font not loaded!" << std::endl;

        SDL_Color textCol = {255, 255, 255, static_cast<Uint8>(255 * anim)};
        window->renderText(m_font, title, textCol, titleX, titleY + static_cast<int>(offsetY));
        int lineY = txtX; 
        lineY = txtY + static_cast<int>(offsetY);
        for (size_t i = 0; i < wrappedLines.size(); ++i) {
            window->renderText(m_font, wrappedLines[i], textCol, txtX, lineY);
            lineY += 30;
        }
        if (showErrorCode)
            window->renderText(m_font, "Error Code: " + errorMSG, textCol, txtX, lineY + 30);

        for (auto& btn : buttons) {
            btn.setParentAlpha(anim);
            btn.Render(renderer);
        }
    }
}

void PopUp::handleEvent(SDL_Event &ev) {
    consumed = false;

    if (isDestroyed) return;

    bool isMouseEvent = false;
    int ex = 0, ey = 0;
    if (ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_MOUSEBUTTONUP) {
        isMouseEvent = true;
        ex = ev.button.x;
        ey = ev.button.y;
    } else if (ev.type == SDL_MOUSEMOTION) {
        isMouseEvent = true;
        ex = ev.motion.x;
        ey = ev.motion.y;
    }

    SDL_Rect popupRect = { static_cast<int>(pos.x), static_cast<int>(pos.y),
                            static_cast<int>(size.x), static_cast<int>(size.y) };

    if (isMouseEvent) {
        SDL_Point pt = { ex, ey };
        if (SDL_PointInRect(&pt, &popupRect)) {
            consumed = true;
            for (auto &btn : buttons) {
                btn.handleEvent(ev);
            }

            if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT) {
                for (auto &btn : buttons) {
                    if (btn.pollClicked()) {
                        startHide();
                        break;
                    }
                }
            }
            return;
        }
        consumed = true;
        return;
    }

    if (ev.type == SDL_KEYDOWN) {
        consumed = true;
        if (ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            startHide();
        }
    }
}

void PopUp::checkCollisions(Player &player) {
    if (this->collidesWith(player)) {
        std::cout << "Collision detected between Button and Player!" << std::endl;
        this->destroy();
        pos.x = rand() % 500;
        pos.y = rand() % 500;
        player.setMaxSpeed(400.0f);
    }
}

bool PopUp::isHovered() {
    //We add 48 to the y position because of the ControlMenu
    SDL_Rect buttonRect = { static_cast<int>(pos.x), static_cast<int>(pos.y + 36),
                            static_cast<int>(size.x), static_cast<int>(size.y) };
    return SDL_PointInRect(&mouse, &buttonRect);
}

bool PopUp::isPressed() {
    return pressed;
}

void PopUp::setTitle(std::string new_title) {
    title = new_title;
}

void PopUp::setErrorCode(std::string errorCode) {
    errorMSG = errorCode;
}

void PopUp::setShowErrorCode(bool show) {
    showErrorCode = show;
}

void PopUp::destroy() {
    isDestroyed = true;
}

void PopUp::startHide() {
    if (animState == ANIM_HIDING || animState == ANIM_HIDDEN) return;
    animState = ANIM_HIDING;
    animTimer = 0.0f;
}

void PopUp::show() {
    if (animState == ANIM_SHOWING || animState == ANIM_VISIBLE) return;
    isDestroyed = false;
    animState = ANIM_SHOWING;
    animTimer = 0.0f;
}
