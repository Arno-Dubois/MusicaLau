#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <vector>
#include <string>
#include <cmath>
#include "../../include/Controller/Button.h" 

class ButtonView {
public:
    ButtonView();
    ~ButtonView();

    bool initialize(TTF_Font *font);

    void renderButtons(SDL_Renderer *renderer, const std::vector<Button> &buttons, bool isSongPlayingActive,
                       bool isSongPaused);

    // Utility functions for drawing, moved from Controller
    void renderTextCentered(SDL_Renderer *renderer, float centerX, float centerY, const std::string &text,
                            SDL_Color color);

    void renderSmallText(SDL_Renderer *renderer, float centerX, float centerY, const std::string &text,
                         SDL_Color color);

    void drawFileIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color);
    void drawPlayIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color);
    void drawUpArrow(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color);
    void drawStopIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color);

    void drawPauseIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color);

    // Declaration ensured

private:
    TTF_Font *font_;
};