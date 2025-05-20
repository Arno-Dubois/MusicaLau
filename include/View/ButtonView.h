\
#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <vector>
#include <string>
#include <cmath> // For M_PI
#include "../../include/Controller/Button.h" // Assuming Button.h exists or will be created

class ButtonView {
public:
    ButtonView();
    ~ButtonView();

    bool initialize(TTF_Font* font); // Method to set the font

    void renderButtons(SDL_Renderer *renderer, const std::vector<Button> &buttons);

    // Utility functions for drawing, moved from Controller
    void renderTextCentered(SDL_Renderer *renderer, float centerX, float centerY, const std::string &text, SDL_Color color);
    void renderSmallText(SDL_Renderer *renderer, float centerX, float centerY, const std::string &text, SDL_Color color);
    void drawFileIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color);
    void drawPlayIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color);
    void drawUpArrow(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color);
    void drawStopIcon(SDL_Renderer *renderer, float centerX, float centerY, float size, SDL_Color color);

private:
    TTF_Font *font_; // Font for rendering text
};
