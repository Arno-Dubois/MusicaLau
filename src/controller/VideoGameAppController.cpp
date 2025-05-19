#include "../../include/controller/VideoGameAppController.h"
#include "../../include/model/VideoGame.h"
#include "../../include/view/VideoGameView.h"

VideoGameAppController::VideoGameAppController(int windowWidth, int windowHeight, MusicApp::Audio::AudioEngine* audioE) : Controller(audioE) {
    // Position et dimensions du VideoGame
    float mainAreaX = 50.0f;
    float mainAreaY = toolbarY + buttonHeight + 30.0f;
    float mainAreaWidth = windowWidth - 100.0f;
    float headerHeight = 50.0f;
    float instrumentPanelHeight = 150.0f;
    float instrumentY = mainAreaY + headerHeight + instrumentPanelHeight + 20.0f;
    float instrumentHeight = 250.0f;

    videoGame = new VideoGame(mainAreaX, instrumentY, mainAreaWidth, instrumentHeight);
    videoGameView = new VideoGameView(videoGame);
}

VideoGameAppController::~VideoGameAppController() {
    delete videoGame;
    delete videoGameView;
}

void VideoGameAppController::processButtonAction(int buttonIndex) {
    if (buttonIndex == -1) return;

    switch (buttonIndex) {
        case 1:  // Remove Keys
            videoGame->removeKeys();
            break;
        case 2:  // Add Keys
            videoGame->addKeys();
            break;
            // Autres actions de boutons à ajouter...
    }
}

void VideoGameAppController::render(SDL_Renderer *renderer, int windowWidth, int windowHeight) {
    // Dessiner la surface de travail (background)
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_FRect workspace = {0, 0, (float) windowWidth, (float) windowHeight};
    SDL_RenderFillRect(renderer, &workspace);

    // Barre du haut
    SDL_Color brownColor = {90, 60, 55, 255};
    SDL_SetRenderDrawColor(renderer, brownColor.r, brownColor.g, brownColor.b, brownColor.a);
    SDL_FRect topBar = {0, 0, (float) windowWidth, 129.0f};
    SDL_RenderFillRect(renderer, &topBar);

    // Dessiner les boutons
    Controller::renderButtons(renderer, buttons);

    // Dimensions pour l'interface principale
    float mainAreaX = 50.0f;
    float mainAreaY = toolbarY + buttonHeight + 30.0f;
    float mainAreaWidth = windowWidth - 100.0f;
    float headerHeight = 50.0f;
    float instrumentPanelHeight = 150.0f;

    // En-tête instrument
    SDL_SetRenderDrawColor(renderer, 185, 211, 230, 255);
    SDL_FRect instrumentHeader = {mainAreaX, mainAreaY, mainAreaWidth, headerHeight};
    SDL_RenderFillRect(renderer, &instrumentHeader);

    // Panneau de l'instrument
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_FRect instrumentPanel = {mainAreaX, mainAreaY + headerHeight + 10, mainAreaWidth, instrumentPanelHeight};
    SDL_RenderFillRect(renderer, &instrumentPanel);

    // Zone d'affichage pour le nom de fichier
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_FRect fileNameDisplay = {instrumentPanel.x + instrumentPanel.w * 0.25f, instrumentPanel.y + 25,
                                 instrumentPanel.w * 0.5f,
                                 instrumentPanel.h - 50};
    SDL_RenderFillRect(renderer, &fileNameDisplay);

    // Zones d'affichage pour les chiffres
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    float digitSize = instrumentPanel.h - 50;
    SDL_FRect leftDigit = {instrumentPanel.x + 30, instrumentPanel.y + 25, digitSize, digitSize};
    SDL_FRect rightDigit = {instrumentPanel.x + instrumentPanel.w - digitSize - 30, instrumentPanel.y + 25, digitSize,
                            digitSize};
    SDL_RenderFillRect(renderer, &leftDigit);
    SDL_RenderFillRect(renderer, &rightDigit);

    // Rendre l'instrument de jeu vidéo
    videoGameView->render(renderer);
}