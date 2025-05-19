#include "../../include/controller/XylophoneAppController.h"
#include "../../include/model/Xylophone.h"
#include "../../include/view/XylophoneView.h"
#include "../../include/core/Note.h"
#include "../../include/audio/AudioEngine.h"

XylophoneAppController::XylophoneAppController(int windowWidth, int windowHeight, MusicApp::Audio::AudioEngine *audioE)
        : Controller(audioE) {
    // Mettre à jour les dimensions selon la taille de la fenêtre
    updateDimensions(windowWidth, windowHeight);

    // Calcul des dimensions relatives
    float mainAreaX = calculateRelativeWidth(windowWidth, 0.035f);  // ~3.5% de la largeur
    float mainAreaY = toolbarY + buttonHeight + calculateRelativeHeight(windowHeight, 0.03f);  // 3% spacing
    float mainAreaWidth = calculateRelativeWidth(windowWidth, 0.93f);  // ~93% de la largeur
    float headerHeight = calculateRelativeHeight(windowHeight, 0.05f);  // ~5% de la hauteur
    float instrumentPanelHeight = calculateRelativeHeight(windowHeight, 0.146f);  // ~14.6% de la hauteur
    float instrumentY = mainAreaY + headerHeight + instrumentPanelHeight +
                        calculateRelativeHeight(windowHeight, 0.02f);  // 2% spacing
    float instrumentHeight = calculateRelativeHeight(windowHeight, 0.342f);  // ~34.2% de la hauteur

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

void XylophoneAppController::handleXylophoneKeyClick(float mouseX, float mouseY) {
    if (!xylophone || !audioEngine) {
        return;
    }

    int barIndex = xylophone->getBarAt(mouseX, mouseY);
    if (barIndex != -1) {
        // Convertir l'index de la lame en note (Do, Ré, Mi, etc.)
        // Pour l'instant, nous utilisons une note simple basée sur l'index
        std::string noteName = "XyloNote" + std::to_string(barIndex);
        MusicApp::Core::Note note(noteName);

        // Jouer le son correspondant
        audioEngine->playSound("Xylophone", note);
    }
}

void XylophoneAppController::handleXylophoneKeyHover(float mouseX, float mouseY) {
    if (!xylophone) {
        return;
    }

    // Mise à jour de l'état de survol des lames du xylophone
    xylophone->updateHoveredBar(mouseX, mouseY);
}

void XylophoneAppController::render(SDL_Renderer *renderer, int windowWidth, int windowHeight) {
    // Mettre à jour les dimensions des boutons et éléments UI en fonction des dimensions de la fenêtre
    updateDimensions(windowWidth, windowHeight);

    // Dessiner la surface de travail (background)
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_FRect workspace = {0, 0, (float) windowWidth, (float) windowHeight};
    SDL_RenderFillRect(renderer, &workspace);

    // Barre du haut
    SDL_Color brownColor = {90, 60, 55, 255};
    SDL_SetRenderDrawColor(renderer, brownColor.r, brownColor.g, brownColor.b, brownColor.a);
    float topBarHeight = calculateRelativeHeight(windowHeight, 0.126f);  // ~12.6% de la hauteur
    SDL_FRect topBar = {0, 0, (float) windowWidth, topBarHeight};
    SDL_RenderFillRect(renderer, &topBar);

    // Dessiner les boutons
    Controller::renderButtons(renderer, buttons);

    // Dimensions pour l'interface principale - utiliser des proportions relatives
    float mainAreaX = calculateRelativeWidth(windowWidth, 0.035f);  // ~3.5% de la largeur
    float mainAreaY = toolbarY + buttonHeight + calculateRelativeHeight(windowHeight, 0.03f);  // 3% spacing
    float mainAreaWidth = calculateRelativeWidth(windowWidth, 0.93f);  // ~93% de la largeur
    float headerHeight = calculateRelativeHeight(windowHeight, 0.05f);  // ~5% de la hauteur
    float instrumentPanelHeight = calculateRelativeHeight(windowHeight, 0.146f);  // ~14.6% de la hauteur

    // Mise à jour des dimensions du xylophone si nécessaire
    float newXyloWidth = mainAreaWidth;
    float newXyloHeight = calculateRelativeHeight(windowHeight, 0.342f);
    float newXyloX = mainAreaX;
    float newXyloY = mainAreaY + headerHeight + instrumentPanelHeight + calculateRelativeHeight(windowHeight, 0.02f);

    if (xylophone->getWidth() != newXyloWidth || xylophone->getHeight() != newXyloHeight) {
        xylophone->setDimensions(newXyloWidth, newXyloHeight);
    }

    if (xylophone->getX() != newXyloX || xylophone->getY() != newXyloY) {
        xylophone->setPosition(newXyloX, newXyloY);
    }

    // En-tête instrument
    SDL_SetRenderDrawColor(renderer, 185, 211, 230, 255);
    SDL_FRect instrumentHeader = {mainAreaX, mainAreaY, mainAreaWidth, headerHeight};
    SDL_RenderFillRect(renderer, &instrumentHeader);

    // Panneau du xylophone
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_FRect instrumentPanel = {mainAreaX, mainAreaY + headerHeight + calculateRelativeHeight(windowHeight, 0.01f),
                                 mainAreaWidth, instrumentPanelHeight};
    SDL_RenderFillRect(renderer, &instrumentPanel);

    // Zone d'affichage pour le nom de fichier
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_FRect fileNameDisplay = {instrumentPanel.x + instrumentPanel.w * 0.25f,
                                 instrumentPanel.y + instrumentPanelHeight * 0.17f,
                                 instrumentPanel.w * 0.5f, instrumentPanelHeight * 0.66f};
    SDL_RenderFillRect(renderer, &fileNameDisplay);

    // Zones d'affichage pour les chiffres
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    float digitSize = instrumentPanelHeight * 0.66f;
    float digitMargin = calculateRelativeWidth(windowWidth, 0.021f);  // ~2.1% de la largeur
    SDL_FRect leftDigit = {instrumentPanel.x + digitMargin, instrumentPanel.y + instrumentPanelHeight * 0.17f,
                           digitSize, digitSize};
    SDL_FRect rightDigit = {instrumentPanel.x + instrumentPanel.w - digitSize - digitMargin,
                            instrumentPanel.y + instrumentPanelHeight * 0.17f, digitSize, digitSize};
    SDL_RenderFillRect(renderer, &leftDigit);
    SDL_RenderFillRect(renderer, &rightDigit);

    // Rendre le xylophone
    xylophoneView->render(renderer, windowWidth, windowHeight);
}