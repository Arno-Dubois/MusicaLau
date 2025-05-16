#include "../include/view/PianoView.h"

PianoView::PianoView(Piano *piano) : piano(piano) {
}

void PianoView::render(SDL_Renderer *renderer) {
    float x = piano->getX();
    float y = piano->getY();
    float w = piano->getWidth();
    float h = piano->getHeight();

    // Fond blanc pour le clavier complet
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect pianoBackground = {x, y, w, h};
    SDL_RenderFillRect(renderer, &pianoBackground);

    // Bordure du piano
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &pianoBackground);

    // Dessiner les touches blanches avec leurs bordures
    float whiteKeyWidth = w / 21.0f; // 21 touches blanches

    for (int i = 0; i < 21; i++) {
        // Bordures verticales entre les touches blanches
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_FRect keyBorder = {x + (i + 1) * whiteKeyWidth, y, 1.0f, h};
        SDL_RenderFillRect(renderer, &keyBorder);
    }

    // Dessiner les touches noires
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    float blackKeyWidth = whiteKeyWidth * 0.6f;
    float blackKeyHeight = h * 0.65f;

    for (int i = 0; i < 21; i++) {
        // Ne pas dessiner de touche noire après la dernière touche blanche
        if (i == 20) continue;

        // Motif pour les touches noires (après les touches 0, 1, 3, 4, 5...)
        // Pattern pour un clavier standard: pas de noir après E et B (positions 2 et 6 dans chaque octave)
        if ((i % 7 != 2) && (i % 7 != 6)) {
            SDL_FRect key = {x + i * whiteKeyWidth + whiteKeyWidth - blackKeyWidth / 2, y, blackKeyWidth,
                             blackKeyHeight};
            SDL_RenderFillRect(renderer, &key);
        }
    }
}