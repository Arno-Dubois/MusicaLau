#include "../../include/controller/VideoGameAppController.h"
#include "../../include/model/VideoGame.h"
#include "../../include/view/VideoGameView.h"
#include "../../include/view/ButtonView.h"
#include "../../include/core/Note.h"
#include "../../include/audio/AudioEngine.h"
#include "../../include/audio/SDLAudioEngine.h"

VideoGameAppController::VideoGameAppController(int windowWidth, int windowHeight, MusicApp::Audio::AudioEngine *audioE)
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
    float instrumentHeight = calculateRelativeHeight(windowHeight, 0.244f);  // ~24.4% de la hauteur

    videoGame = new VideoGame(mainAreaX, instrumentY, mainAreaWidth, instrumentHeight);
    videoGameView = new VideoGameView(videoGame);
}

VideoGameAppController::~VideoGameAppController() {
    delete videoGame;
    delete videoGameView;
}

void VideoGameAppController::handleVideoGameKeyClick(float mouseX, float mouseY) {
    if (!videoGame || !audioEngine) {
        return;
    }

    // Vérifiez si la souris est sur une touche et récupérez son indice et sa position
    std::string noteName;
    SDL_FRect buttonRect;
    bool isNoteFound = videoGame->getNoteInfoAt(mouseX, mouseY, noteName, buttonRect);

    if (isNoteFound && !noteName.empty()) {
        // Calculer la vélocité en fonction de la position verticale du clic sur la touche
        // Plus le clic est bas sur la touche, plus la vélocité est élevée
        float relativeY = (mouseY - buttonRect.y) / buttonRect.h;
        float velocity = 0.3f + (relativeY * 0.7f); // Entre 0.3 et 1.0

        // Limiter la vélocité entre 0.3 et 1.0
        velocity = SDL_clamp(velocity, 0.3f, 1.0f);

        MusicApp::Core::Note note(noteName);

        // Jouer la note avec la vélocité calculée
        auto sdlAudioEngine = dynamic_cast<MusicApp::Audio::SDLAudioEngine *>(audioEngine);
        if (sdlAudioEngine) {
            sdlAudioEngine->playSound("8BitConsole", note, velocity);
        } else {
            // Fallback sur la version sans vélocité si le cast échoue
            audioEngine->playSound("8BitConsole", note);
        }

        // Pour le débogage
        // SDL_Log("Videogame button clicked: %s with velocity: %f", noteName.c_str(), velocity);
    }
}

void VideoGameAppController::handleVideoGameKeyHover(float mouseX, float mouseY) {
    if (!videoGame) {
        return;
    }

    // Mettre à jour l'état de survol du bouton
    videoGame->updateHoveredButton(mouseX, mouseY);

    // Mettre à jour l'état de survol des contrôles
    videoGame->updateHoveredControl(mouseX, mouseY);
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
    float instrumentPanelHeight = calculateRelativeHeight(windowHeight, 0.146f);  // ~14.6% de la hauteur

    // Mise à jour des dimensions du jeu vidéo si nécessaire
    videoGame->setDimensions(mainAreaWidth, calculateRelativeHeight(windowHeight, 0.244f));
    videoGame->setPosition(mainAreaX, mainAreaY + headerHeight + instrumentPanelHeight +
                                      calculateRelativeHeight(windowHeight, 0.02f));

    // En-tête instrument
    SDL_SetRenderDrawColor(renderer, 185, 211, 230, 255);
    SDL_FRect instrumentHeader = {mainAreaX, mainAreaY, mainAreaWidth, headerHeight};
    SDL_RenderFillRect(renderer, &instrumentHeader);

    // Panneau de l'instrument
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

    // Rendre l'instrument de jeu vidéo
    videoGameView->render(renderer, windowWidth, windowHeight);
}