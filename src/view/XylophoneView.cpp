//
// Created by coren on 24/03/2025.
//

#include "../../include/view/XylophoneView.h"
#include "../../include/model/Xylophone.h"
#include <SDL3/SDL.h>

XylophoneView::XylophoneView(Xylophone *xylophone) : xylophone(xylophone) {
}

void XylophoneView::render(SDL_Renderer *renderer) {
    float x = xylophone->getX();
    float y = xylophone->getY();
    float w = xylophone->getWidth();
    float h = xylophone->getHeight();
    int numBars = xylophone->getBars();

    // Dimensions générales
    float xylophoneWidth = w * 0.9f; // Largeur totale du xylophone
    float xylophoneHeight = h * 0.8f; // Hauteur totale du xylophone

    // Position du xylophone (centré horizontalement, mais plus bas verticalement)
    float xylophoneX = x + (w - xylophoneWidth) / 2;
    float xylophoneY = y + h * 0.2f; // Déplacer plus bas d'environ 20% de la hauteur totale

    // Ajuster la hauteur si besoin pour maintenir l'instrument dans la zone visible
    if (xylophoneY + xylophoneHeight > y + h) {
        xylophoneHeight = (y + h) - xylophoneY - 10; // Garder une marge de 10 pixels
    }

    // Dessiner le cadre/support du xylophone
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Brun pour le support en bois

    // Support gauche
    SDL_FRect leftSupport = {
            xylophoneX,
            xylophoneY + xylophoneHeight * 0.1f,
            xylophoneWidth * 0.05f,
            xylophoneHeight * 0.8f
    };
    SDL_RenderFillRect(renderer, &leftSupport);

    // Support droit
    SDL_FRect rightSupport = {
            xylophoneX + xylophoneWidth - xylophoneWidth * 0.05f,
            xylophoneY + xylophoneHeight * 0.1f,
            xylophoneWidth * 0.05f,
            xylophoneHeight * 0.8f
    };
    SDL_RenderFillRect(renderer, &rightSupport);

    // Dessiner la base
    SDL_FRect base = {
            xylophoneX,
            xylophoneY + xylophoneHeight * 0.9f,
            xylophoneWidth,
            xylophoneHeight * 0.1f
    };
    SDL_RenderFillRect(renderer, &base);

    // Définir les couleurs pour les lames (notes)
    SDL_Color noteColors[] = {
            {220, 20,  60,  255},   // Rouge (Do - C)
            {255, 140, 0,   255},   // Orange (Ré - D)
            {255, 215, 0,   255},   // Jaune (Mi - E)
            {50,  205, 50,  255},   // Vert (Fa - F)
            {135, 206, 235, 255}, // Bleu ciel (Sol - G)
            {65,  105, 225, 255},  // Bleu royal (La - A)
            {138, 43,  226, 255},  // Violet (Si - B)
            {220, 20,  60,  255},   // Rouge (Do - C)
            {255, 140, 0,   255},   // Orange (Ré - D)
            {255, 215, 0,   255},   // Jaune (Mi - E)
            {50,  205, 50,  255},   // Vert (Fa - F)
            {135, 206, 235, 255}  // Bleu ciel (Sol - G)
    };

    // Dimensions des lames
    float barAreaWidth = xylophoneWidth * 0.85f;
    float barAreaX = xylophoneX + (xylophoneWidth - barAreaWidth) / 2;
    float barHeight = xylophoneHeight * 0.1f;
    float totalBarsSpace = xylophoneHeight * 0.75f;
    float barSpacing = totalBarsSpace / numBars;

    // Dessiner les lames horizontales
    for (int i = 0; i < numBars; i++) {
        // La largeur diminue progressivement pour les notes plus hautes
        float barWidth = barAreaWidth * (1.0f - (i * 0.03f));
        float barX = barAreaX + (barAreaWidth - barWidth) / 2;
        float barY = xylophoneY + xylophoneHeight * 0.15f + i * barSpacing;

        // Dessiner la lame
        SDL_SetRenderDrawColor(renderer, noteColors[i].r, noteColors[i].g, noteColors[i].b, noteColors[i].a);
        SDL_FRect bar = {barX, barY, barWidth, barHeight};
        SDL_RenderFillRect(renderer, &bar);

        // Bordure noire pour la lame
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderRect(renderer, &bar);

        // Dessiner les cordons qui soutiennent la lame
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        float leftStringX = barX + barWidth * 0.25f;
        float rightStringX = barX + barWidth * 0.75f;

        // Cordon gauche
        SDL_FRect leftString = {leftStringX, barY + barHeight - 2, 2, 10};
        SDL_RenderFillRect(renderer, &leftString);

        // Cordon droit
        SDL_FRect rightString = {rightStringX, barY + barHeight - 2, 2, 10};
        SDL_RenderFillRect(renderer, &rightString);
    }

    // Dessiner les maillets
    float malletLength = xylophoneHeight * 0.25f;
    float malletHeadSize = barHeight * 0.8f;
    float malletStickWidth = 3.0f;

    // Maillet gauche - maintenant positionnés sur les côtés plutôt qu'au-dessus
    SDL_SetRenderDrawColor(renderer, 70, 40, 10, 255); // Couleur du bâton
    SDL_FRect leftMalletStick = {
            xylophoneX - malletLength * 0.8f,
            xylophoneY + xylophoneHeight * 0.3f,
            malletLength,
            malletStickWidth
    };
    SDL_RenderFillRect(renderer, &leftMalletStick);

    // Tête du maillet gauche
    SDL_SetRenderDrawColor(renderer, 240, 230, 140, 255); // Couleur de la tête
    SDL_FRect leftMalletHead = {
            leftMalletStick.x - malletHeadSize * 0.8f,
            leftMalletStick.y - (malletHeadSize - malletStickWidth) / 2,
            malletHeadSize,
            malletHeadSize
    };
    SDL_RenderFillRect(renderer, &leftMalletHead);

    // Maillet droit - maintenant positionnés sur les côtés plutôt qu'au-dessus
    SDL_SetRenderDrawColor(renderer, 70, 40, 10, 255);
    SDL_FRect rightMalletStick = {
            xylophoneX + xylophoneWidth,
            xylophoneY + xylophoneHeight * 0.6f,
            malletLength,
            malletStickWidth
    };
    SDL_RenderFillRect(renderer, &rightMalletStick);

    // Tête du maillet droit
    SDL_SetRenderDrawColor(renderer, 240, 230, 140, 255);
    SDL_FRect rightMalletHead = {
            rightMalletStick.x + malletLength,
            rightMalletStick.y - (malletHeadSize - malletStickWidth) / 2,
            malletHeadSize,
            malletHeadSize
    };
    SDL_RenderFillRect(renderer, &rightMalletHead);
}