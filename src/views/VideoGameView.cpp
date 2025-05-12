//
// Created by coren on 24/03/2025.
//

#include "../../include/views/VideoGameView.h"
#include "../../include/models/VideoGame.h"
#include <SDL3/SDL.h>

VideoGameView::VideoGameView(VideoGame *videoGame) : videoGame(videoGame) {
}

void VideoGameView::render(SDL_Renderer *renderer) {
    float x = videoGame->getX();
    float y = videoGame->getY();
    float w = videoGame->getWidth();
    float h = videoGame->getHeight();
    int numKeys = videoGame->getKeys();

    float keyWidth = w / numKeys;
    float keyHeight = h * 0.8f;

    SDL_Color colors[] = {
            {255, 0,   0,   255},       // Rouge
            {255, 165, 0,   255},     // Orange
            {255, 255, 0,   255},     // Jaune
            {0,   255, 0,   255},       // Vert
            {0,   255, 255, 255},     // Cyan
            {0,   0,   255, 255},       // Bleu
            {128, 0,   128, 255},     // Violet
            {255, 0,   255, 255},     // Magenta
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