//
// Created by coren on 24/03/2025.
//

#include "../../include/views/XylophoneView.h"
#include "../../include/models/Xylophone.h"
#include <SDL3/SDL.h>

XylophoneView::XylophoneView(Xylophone *xylophone) : xylophone(xylophone) {
}

void XylophoneView::render(SDL_Renderer *renderer) {
    float x = xylophone->getX();
    float y = xylophone->getY();
    float w = xylophone->getWidth();
    float h = xylophone->getHeight();
    int numBars = xylophone->getBars();

    float barHeight = h / numBars; // Hauteur de chaque lame
    float barWidth;

    SDL_Color colors[] = {
            {255, 0,   0,   255},    // Rouge
            {255, 165, 0,   255},  // Orange
            {255, 255, 0,   255},  // Jaune
            {0,   128, 0,   255},    // Vert
            {0,   0,   255, 255},    // Bleu
            {75,  0,   130, 255},   // Indigo
            {148, 0,   211, 255},  // Violet
            {139, 69,  19,  255},   // Marron
            {255, 105, 180, 255}, // Rose
            {0,   191, 255, 255},   // Bleu ciel
            {154, 205, 50,  255},  // Vert jaune
            {255, 20,  147, 255}   // Rose foncé
    };

    for (int i = 0; i < numBars; i++) {
        barWidth = w * (1.0f - (i * 0.08f)); // Réduction progressive de la largeur
        float barX = x + (w - barWidth) / 2; // Centrer les lames
        float barY = y + i * barHeight;

        SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
        SDL_FRect bar = {barX, barY, barWidth, barHeight - 5}; // Lames avec un petit espace entre elles
        SDL_RenderFillRect(renderer, &bar);
    }
}