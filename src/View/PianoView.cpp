#include "../include/View/PianoView.h"

PianoView::PianoView(Piano *piano) : piano(piano) {
}

void PianoView::render(SDL_Renderer *renderer, int windowWidth, int windowHeight) {
    if (!piano) return;

    const auto &keys = piano->getPianoKeys();
    if (keys.empty()) { // Draw a simple placeholder if keys aren't calculated yet
        float x = piano->getX();
        float y = piano->getY();
        float w = piano->getWidth();
        float h = piano->getHeight();
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Light grey placeholder
        SDL_FRect pianoBackground = {x, y, w, h};
        SDL_RenderFillRect(renderer, &pianoBackground);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderRect(renderer, &pianoBackground);
        // Optionally, render text like "Piano keys not loaded"
        return;
    }

    // Render white keys first
    for (const auto &key: keys) {
        if (!key.isBlack) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
            // Si la touche est survolée, utiliser une couleur de surbrillance
            if (key.isHovered) {
                SDL_SetRenderDrawColor(renderer, 210, 230, 255, 255); // Bleu clair pour le hover
            }
            SDL_RenderFillRect(renderer, &key.rect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black border
            SDL_RenderRect(renderer, &key.rect);
        }
    }

    // Render black keys on top
    for (const auto &key: keys) {
        if (key.isBlack) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
            // Si la touche est survolée, utiliser une couleur de surbrillance
            if (key.isHovered) {
                SDL_SetRenderDrawColor(renderer, 40, 40, 80, 255); // Bleu foncé pour le hover
            }
            SDL_RenderFillRect(renderer, &key.rect);
            // Optionally, add a slight highlight or border to black keys if desired
        }
    }
}