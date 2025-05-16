//
// Created by coren on 24/03/2025.
//

#include "../../include/View/VideoGameView.h"
#include "../../include/Model/VideoGame.h"
#include <SDL3/SDL.h>

VideoGameView::VideoGameView(VideoGame *videoGame) : videoGame(videoGame) {
}

void VideoGameView::render(SDL_Renderer *renderer, int windowWidth, int windowHeight) {
    float x = videoGame->getX();
    float y = videoGame->getY();
    float w = videoGame->getWidth();
    float h = videoGame->getHeight();
    int numKeys = videoGame->getKeys();

    // Dimensions et positions de la console rétro
    float consoleWidth = w * 0.9f;
    float consoleHeight = h * 0.8f;
    float consoleX = x + (w - consoleWidth) / 2;
    float consoleY = y + (h - consoleHeight) / 2;

    // Couleurs 8-bit
    SDL_Color consoleGray = {80, 80, 80, 255};
    SDL_Color screenBorder = {40, 40, 40, 255};
    SDL_Color screenColor = {20, 20, 20, 255};
    SDL_Color pixelColors[] = {
            {255, 0,   0,   255},     // Rouge vif
            {255, 165, 0,   255},   // Orange
            {255, 255, 0,   255},   // Jaune
            {0,   255, 0,   255},     // Vert vif
            {0,   255, 255, 255},   // Cyan
            {0,   120, 255, 255},   // Bleu électrique
            {255, 0,   255, 255},   // Magenta
            {255, 105, 180, 255}, // Rose néon
    };

    // Dessiner le corps de la console
    SDL_SetRenderDrawColor(renderer, consoleGray.r, consoleGray.g, consoleGray.b, consoleGray.a);
    SDL_FRect consoleBody = {consoleX, consoleY, consoleWidth, consoleHeight};
    SDL_RenderFillRect(renderer, &consoleBody);

    // Bordure de la console
    SDL_SetRenderDrawColor(renderer, screenBorder.r, screenBorder.g, screenBorder.b, screenBorder.a);
    SDL_RenderRect(renderer, &consoleBody);

    // Écran de la console
    float screenMargin = consoleHeight * 0.1f;
    float screenWidth = consoleWidth - screenMargin * 2;
    float screenHeight = consoleHeight * 0.4f;
    float screenX = consoleX + screenMargin;
    float screenY = consoleY + screenMargin;

    // Bordure de l'écran
    SDL_SetRenderDrawColor(renderer, screenBorder.r, screenBorder.g, screenBorder.b, screenBorder.a);
    SDL_FRect screenBorderRect = {screenX - 5, screenY - 5, screenWidth + 10, screenHeight + 10};
    SDL_RenderFillRect(renderer, &screenBorderRect);

    // Écran lui-même
    SDL_SetRenderDrawColor(renderer, screenColor.r, screenColor.g, screenColor.b, screenColor.a);
    SDL_FRect screenRect = {screenX, screenY, screenWidth, screenHeight};
    SDL_RenderFillRect(renderer, &screenRect);

    // Dessiner une grille de "pixels" de style 8-bit sur l'écran
    float pixelSize = screenWidth / 32; // 32 pixels de largeur
    int pixelRows = static_cast<int>(screenHeight / pixelSize);

    for (int row = 0; row < pixelRows; row++) {
        for (int col = 0; col < 32; col++) {
            // Ne dessiner que certains pixels pour former un motif de jeu 8-bit
            // Cette logique peut être modifiée pour créer différents motifs
            if ((row + col) % 4 == 0 || (row * col) % 7 == 0) {
                SDL_Color pixelColor = pixelColors[(row + col) % 8];
                SDL_SetRenderDrawColor(renderer, pixelColor.r, pixelColor.g, pixelColor.b, pixelColor.a);

                SDL_FRect pixel = {
                        screenX + col * pixelSize,
                        screenY + row * pixelSize,
                        pixelSize - 1,
                        pixelSize - 1
                };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    // Dessiner les contrôles (boutons colorés et D-pad)
    float controlAreaY = screenY + screenHeight + screenMargin;
    float controlAreaHeight = consoleHeight - screenHeight - screenMargin * 3;

    // D-pad (croix directionnelle)
    float dpadSize = controlAreaHeight * 0.8f;
    float dpadX = consoleX + screenMargin * 1.5f;
    float dpadY = controlAreaY + (controlAreaHeight - dpadSize) / 2;
    float dpadThickness = dpadSize * 0.3f;

    // D-pad horizontal
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_FRect dpadHorizontal = {
            dpadX,
            dpadY + (dpadSize - dpadThickness) / 2,
            dpadSize,
            dpadThickness
    };
    SDL_RenderFillRect(renderer, &dpadHorizontal);

    // D-pad vertical
    SDL_FRect dpadVertical = {
            dpadX + (dpadSize - dpadThickness) / 2,
            dpadY,
            dpadThickness,
            dpadSize
    };
    SDL_RenderFillRect(renderer, &dpadVertical);

    // Boutons A, B
    float buttonDiameter = dpadThickness * 1.2f;
    float buttonsX = consoleX + consoleWidth - screenMargin * 2 - buttonDiameter * 2;
    float buttonsY = controlAreaY + (controlAreaHeight - buttonDiameter) / 2;

    // Bouton A
    SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255); // Rouge
    SDL_FRect buttonA = {
            buttonsX,
            buttonsY,
            buttonDiameter,
            buttonDiameter
    };
    SDL_RenderFillRect(renderer, &buttonA);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &buttonA);

    // Bouton B
    SDL_SetRenderDrawColor(renderer, 50, 50, 255, 255); // Bleu
    SDL_FRect buttonB = {
            buttonsX + buttonDiameter * 1.5f,
            buttonsY,
            buttonDiameter,
            buttonDiameter
    };
    SDL_RenderFillRect(renderer, &buttonB);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &buttonB);

    // Petits boutons Start et Select au centre
    float smallButtonWidth = buttonDiameter * 1.5f;
    float smallButtonHeight = buttonDiameter * 0.5f;
    float smallButtonsX = consoleX + consoleWidth / 2 - smallButtonWidth / 2;
    float smallButtonsY = controlAreaY + controlAreaHeight / 2;

    // Bouton Select
    SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255); // Gris
    SDL_FRect selectButton = {
            smallButtonsX - smallButtonWidth * 0.7f,
            smallButtonsY,
            smallButtonWidth,
            smallButtonHeight
    };
    SDL_RenderFillRect(renderer, &selectButton);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &selectButton);

    // Bouton Start
    SDL_FRect startButton = {
            smallButtonsX + smallButtonWidth * 0.7f,
            smallButtonsY,
            smallButtonWidth,
            smallButtonHeight
    };
    SDL_RenderFillRect(renderer, &startButton);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &startButton);
}