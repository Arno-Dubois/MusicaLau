#include "Utils/TextHelper.h"
#include <array>
#include <filesystem>
#include <iostream>
#include <vector>

namespace TextHelper {
TTF_Font *LoadFont(const std::string &fontName, int ptsize) {
  // Look next to the executable first, so the app works from any working
  // directory
  if (const char *base = SDL_GetBasePath()) {
    if (TTF_Font *font = TTF_OpenFont(
            (std::string(base) + "assets/fonts/" + fontName).c_str(), ptsize))
      return font;
  }

  // Then relative to the current working directory
  if (TTF_Font *font =
          TTF_OpenFont(("assets/fonts/" + fontName).c_str(), ptsize))
    return font;

  // Last resort: system Arial (Windows only)
  return TTF_OpenFont("C:/Windows/Fonts/arial.ttf", ptsize);
}

SDL_Surface *RenderTextSolid(TTF_Font *font, const std::string &text,
                             SDL_Color fg) {
  return TTF_RenderText_Solid(font, text.c_str(), text.length(), fg);
}
} // namespace TextHelper