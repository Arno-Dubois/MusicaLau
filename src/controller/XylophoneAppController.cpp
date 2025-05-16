#include "../../include/controller/XylophoneAppController.h"
#include "../../include/model/Xylophone.h"
#include "../../include/view/XylophoneView.h"

XylophoneAppController::XylophoneAppController(int windowWidth, int windowHeight) : Controller() {
    // Position et dimensions du xylophone
    float mainAreaX = 50.0f;
    float mainAreaY = toolbarY + buttonHeight + 30.0f;
    float mainAreaWidth = windowWidth - 100.0f;
    float headerHeight = 50.0f;
    float instrumentPanelHeight = 150.0f;
    float instrumentY = mainAreaY + headerHeight + instrumentPanelHeight + 20.0f;
    float instrumentHeight = 350.0f;

    xylophone = new Xylophone(mainAreaX, instrumentY, mainAreaWidth, instrumentHeight);
    xylophoneView = new XylophoneView(xylophone);
}

XylophoneAppController::~XylophoneAppController() {
    delete xylophone;
    delete xylophoneView;
}

void XylophoneAppController::processButtonAction(int buttonIndex) {
    if (buttonIndex == -1) return;

    switch (buttonIndex) {
        case 1:  // Remove Bar
            xylophone->removeBar();
            break;
        case 2:  // Add Bar
            xylophone->addBar();
            break;
            // Autres actions de boutons à ajouter...
    }
}

void XylophoneAppController::render(SDL_Renderer *renderer, int windowWidth, int windowHeight) {
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

    // Panneau du xylophone
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

    // Rendre le xylophone
    xylophoneView->render(renderer);
}