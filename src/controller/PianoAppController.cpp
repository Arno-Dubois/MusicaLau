#include "../../include/controller/PianoAppController.h"
#include "../../include/model/Piano.h"
#include "../../include/view/PianoView.h"
#include "../../include/View/ButtonView.h"

PianoAppController::PianoAppController(int windowWidth, int windowHeight, MusicApp::Audio::AudioEngine *audioE)
        : Controller(audioE) {
    // Mettre à jour les dimensions selon la taille de la fenêtre
    updateDimensions(windowWidth, windowHeight);

    // Calcul des dimensions relatives
    float mainAreaX = calculateRelativeWidth(windowWidth, 0.035f);  // ~3.5% de la largeur
    float mainAreaY = toolbarY + buttonHeight + calculateRelativeHeight(windowHeight, 0.03f);  // 3% spacing
    float mainAreaWidth = calculateRelativeWidth(windowWidth, 0.93f);  // ~93% de la largeur
    float headerHeight = calculateRelativeHeight(windowHeight, 0.05f);  // ~5% de la hauteur
    float pianoPanelHeight = calculateRelativeHeight(windowHeight, 0.146f);  // ~14.6% de la hauteur
    float pianoY =
            mainAreaY + headerHeight + pianoPanelHeight + calculateRelativeHeight(windowHeight, 0.02f);  // 2% spacing
    float pianoHeight = calculateRelativeHeight(windowHeight, 0.244f);  // ~24.4% de la hauteur

    piano = new Piano(mainAreaX, pianoY, mainAreaWidth, pianoHeight);
    pianoView = new PianoView(piano);
}

PianoAppController::~PianoAppController() {
    delete piano;
    delete pianoView;
}

void PianoAppController::processButtonAction(int buttonIndex) {
    if (buttonIndex == -1) return;

    switch (buttonIndex) {
        case 1:  // Remove Octave
            piano->removeOctave();
            break;
        case 2:  // Add Octave
            piano->addOctave();
            break;
            // Autres actions de boutons à ajouter...
    }
}

void PianoAppController::handlePianoKeyClick(float mouseX, float mouseY) {
    if (!piano || !audioEngine) {
        return;
    }

    std::string pitchName = piano->getPitchAt(mouseX, mouseY);

    if (!pitchName.empty()) {
        MusicApp::Core::Note note(pitchName);
        // Assuming "Piano" is the instrument name the SDLAudioEngine expects for piano sounds.
        // This might need to be configurable or based on a more robust instrument mapping.
        audioEngine->playSound("Piano", note);
        // For debugging:
        // SDL_Log("Piano key clicked: %s", pitchName.c_str());
    }
}

void PianoAppController::handlePianoKeyHover(float mouseX, float mouseY) {
    if (!piano) {
        return;
    }

    piano->updateHoveredKey(mouseX, mouseY);
}

void PianoAppController::render(SDL_Renderer *renderer, int windowWidth, int windowHeight) {
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
    if (buttonView_) {
        buttonView_->renderButtons(renderer, buttons);
    }

    // Dimensions pour l'interface principale - utiliser des proportions relatives
    float mainAreaX = calculateRelativeWidth(windowWidth, 0.035f);  // ~3.5% de la largeur
    float mainAreaY = toolbarY + buttonHeight + calculateRelativeHeight(windowHeight, 0.03f);  // 3% spacing
    float mainAreaWidth = calculateRelativeWidth(windowWidth, 0.93f);  // ~93% de la largeur
    float headerHeight = calculateRelativeHeight(windowHeight, 0.05f);  // ~5% de la hauteur
    float pianoPanelHeight = calculateRelativeHeight(windowHeight, 0.146f);  // ~14.6% de la hauteur

    // Mise à jour des dimensions du piano si nécessaire
    float newPianoWidth = mainAreaWidth;
    float newPianoHeight = calculateRelativeHeight(windowHeight, 0.244f);
    float newPianoX = mainAreaX;
    float newPianoY = mainAreaY + headerHeight + pianoPanelHeight + calculateRelativeHeight(windowHeight, 0.02f);

    // Ne mettre à jour les dimensions que si elles ont changé
    if (piano->getWidth() != newPianoWidth || piano->getHeight() != newPianoHeight) {
        piano->setDimensions(newPianoWidth, newPianoHeight);
    }

    // Ne mettre à jour la position que si elle a changé
    if (piano->getX() != newPianoX || piano->getY() != newPianoY) {
        piano->setPosition(newPianoX, newPianoY);
    }

    // En-tête instrument
    SDL_SetRenderDrawColor(renderer, 185, 211, 230, 255);
    SDL_FRect instrumentHeader = {mainAreaX, mainAreaY, mainAreaWidth, headerHeight};
    SDL_RenderFillRect(renderer, &instrumentHeader);

    // Panneau du piano
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_FRect pianoPanel = {mainAreaX, mainAreaY + headerHeight + calculateRelativeHeight(windowHeight, 0.01f),
                            mainAreaWidth, pianoPanelHeight};
    SDL_RenderFillRect(renderer, &pianoPanel);

    // Zone d'affichage pour le nom de fichier
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_FRect fileNameDisplay = {pianoPanel.x + pianoPanel.w * 0.25f, pianoPanel.y + pianoPanelHeight * 0.17f,
                                 pianoPanel.w * 0.5f, pianoPanelHeight * 0.66f};
    SDL_RenderFillRect(renderer, &fileNameDisplay);

    // Zones d'affichage pour les chiffres
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    float digitSize = pianoPanelHeight * 0.66f;
    float digitMargin = calculateRelativeWidth(windowWidth, 0.021f);  // ~2.1% de la largeur
    SDL_FRect leftDigit = {pianoPanel.x + digitMargin, pianoPanel.y + pianoPanelHeight * 0.17f,
                           digitSize, digitSize};
    SDL_FRect rightDigit = {pianoPanel.x + pianoPanel.w - digitSize - digitMargin,
                            pianoPanel.y + pianoPanelHeight * 0.17f, digitSize, digitSize};
    SDL_RenderFillRect(renderer, &leftDigit);
    SDL_RenderFillRect(renderer, &rightDigit);

    // Rendre le piano
    pianoView->render(renderer, windowWidth, windowHeight);
}