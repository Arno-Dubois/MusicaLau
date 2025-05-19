#include "../../include/utils/TextHelper.h"

namespace TextHelper {
    SDL_Surface *RenderTextSolid(TTF_Font *font, const std::string &text, SDL_Color fg) {
        return TTF_RenderText_Solid(font, text.c_str(), text.length(), fg);
    }
}