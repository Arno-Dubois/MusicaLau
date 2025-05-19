#include "../../include/utils/TextHelper.h"
#include <iostream>
#include <array>
#include <filesystem>
#include <vector>

namespace TextHelper {
    TTF_Font *LoadFont(const std::string &fontName, int ptsize) {
        // Essayer d'abord le chemin principal
        TTF_Font *font = TTF_OpenFont(("assets/fonts/" + fontName).c_str(), ptsize);
        if (font) return font;

        // Ensuite essayer Arial comme secours
        return TTF_OpenFont("C:/Windows/Fonts/arial.ttf", ptsize);
    }

    SDL_Surface *RenderTextSolid(TTF_Font *font, const std::string &text, SDL_Color fg) {
        return TTF_RenderText_Solid(font, text.c_str(), text.length(), fg);
    }
}