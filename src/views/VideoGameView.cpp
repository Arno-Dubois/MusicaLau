//
// Created by coren on 24/03/2025.
//

#include "../../include/views/VideoGameView.h"

#include <SDL3/SDL.h>

void drawVideoGameInstrument(SDL_Renderer* renderer, float x, float y, float w, float h) {
    int numKeys = 16;
    float keyWidth = w / numKeys;
    float keyHeight = h * 0.8f;

    SDL_Color colors[] = {
            {255, 0, 0, 255},
            {255, 165, 0, 255},
            {255, 255, 0, 255},
            {0, 255, 0, 255},
            {0, 255, 255, 255},
            {0, 0, 255, 255},
            {128, 0, 128, 255},
            {255, 0, 255, 255},
    };

    for (int i = 0; i < numKeys; i++) {
        SDL_Color color = colors[i % 8];
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        float keyX = x + i * keyWidth;
        SDL_FRect key = {keyX, y + (h - keyHeight) / 2, keyWidth - 5, keyHeight};
        SDL_RenderFillRect(renderer, &key);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderRect(renderer, &key);
    }
}