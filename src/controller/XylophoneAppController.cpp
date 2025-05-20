#include "../../include/controller/XylophoneAppController.h"
#include "../../include/model/Xylophone.h"
#include "../../include/view/XylophoneView.h"
#include "../../include/core/Note.h"
#include "../../include/audio/AudioEngine.h"
#include "../../include/audio/SDLAudioEngine.h"

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

    int barIndex;
    SDL_FRect barRect;
    bool isBarFound = xylophone->getBarInfoAt(mouseX, mouseY, barIndex, barRect);

    if (isBarFound && barIndex != -1) {
        // Convertir l'index de la lame en note réelle (notes de gamme de C4 à C5)
        // Gamme diatonique de Do majeur (C major)
        std::string noteName;

        // Répartir les 8 lames standard du xylophone sur une octave
        switch (barIndex) {
            case 0:
                noteName = "C4";
                break;  // Do
            case 1:
                noteName = "D4";
                break;  // Ré
            case 2:
                noteName = "E4";
                break;  // Mi
            case 3:
                noteName = "F4";
                break;  // Fa
            case 4:
                noteName = "G4";
                break;  // Sol
            case 5:
                noteName = "A4";
                break;  // La
            case 6:
                noteName = "B4";
                break;  // Si
            case 7:
                noteName = "C5";
                break;  // Do (octave supérieure)
            default:
                // Pour les xylophones avec plus de 8 lames, continuer la gamme
                if (barIndex == 8) noteName = "D5";
                else if (barIndex == 9) noteName = "E5";
                else if (barIndex == 10) noteName = "F5";
                else if (barIndex == 11) noteName = "G5";
                else noteName = "C4"; // Fallback
                break;
        }

        // Calculer la vélocité basée sur la position horizontale du clic
        // Le xylophone est plus sensible sur les bords qu'au centre
        float relativeX = (mouseX - barRect.x) / barRect.w;
        float distanceFromCenter = std::abs(relativeX - 0.5f) * 2.0f; // 0 au centre, 1 sur les bords
        float velocity = 0.4f + (distanceFromCenter * 0.6f); // Entre 0.4 et 1.0

        // La vélocité peut aussi être influencée par la position verticale
        // Plus on frappe en haut de la barre, plus le son est fort
        float relativeY = (mouseY - barRect.y) / barRect.h;
        float verticalFactor = 1.0f - relativeY; // 1.0 en haut, 0.0 en bas

        // Combiner les deux facteurs pour la vélocité finale
        velocity = 0.4f + ((distanceFromCenter * 0.5f) + (verticalFactor * 0.5f)) * 0.6f;
        velocity = SDL_clamp(velocity, 0.4f, 1.0f);

        MusicApp::Core::Note note(noteName);

        // Jouer le son avec la vélocité calculée
        auto sdlAudioEngine = dynamic_cast<MusicApp::Audio::SDLAudioEngine *>(audioEngine);
        if (sdlAudioEngine) {
            sdlAudioEngine->playSound("Xylophone", note, velocity);
        } else {
            // Fallback sur la version sans vélocité si le cast échoue
            audioEngine->playSound("Xylophone", note);
        }

        // Pour le débogage
        // SDL_Log("Xylophone bar clicked: %d, Note: %s, Velocity: %f", barIndex, noteName.c_str(), velocity);
    }
}

void XylophoneAppController::handleXylophoneKeyHover(float mouseX, float mouseY) {
    if (!xylophone) {
        return;
    }

    // Mise à jour de l'état de survol des lames du xylophone
    bool updated = xylophone->updateHoveredBar(mouseX, mouseY);

    // On pourrait ajouter ici du feedback audio léger au survol si nécessaire
    // Par exemple, jouer une note très douce ou afficher le nom de la note

    // Pour le mode de débogage:
    // if (updated && xylophone->getBarAt(mouseX, mouseY) != -1) {
    //     int barIndex = xylophone->getBarAt(mouseX, mouseY);
    //     SDL_Log("Xylophone bar hovered: %d", barIndex);
    // }
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