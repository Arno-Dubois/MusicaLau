//
// Created by coren on 24/03/2025.
//

#include "../../include/Model/VideoGame.h"

VideoGame::VideoGame(float x, float y, float width, float height, int keys)
        : x(x), y(y), width(width), height(height), keys(keys),
          hoveredButtonIndex(-1), hoveredControlIndex(-1) {
    calculateButtonsLayout();
    calculateControlsLayout();
}

void VideoGame::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
    calculateButtonsLayout();
    calculateControlsLayout();
}

void VideoGame::setDimensions(float newWidth, float newHeight) {
    width = newWidth;
    height = newHeight;
    calculateButtonsLayout();
    calculateControlsLayout();
}

void VideoGame::addKeys() {
    if (keys < 24) keys++;
    calculateButtonsLayout();
}

void VideoGame::removeKeys() {
    if (keys > 1) keys--;
    calculateButtonsLayout();
}

void VideoGame::calculateButtonsLayout() {
    buttons.clear();

    const int NOTES_PER_OCTAVE = 12;
    int actualKeys = keys > NOTES_PER_OCTAVE * 2 ? NOTES_PER_OCTAVE * 2 : keys;

    float consoleWidth = width * 0.98f;
    float consoleHeight = height * 0.95f;
    float consoleX = x + (width - consoleWidth) / 2;
    float consoleY = y + (height - consoleHeight) / 2;

    float screenMargin = consoleHeight * 0.025f;
    float screenWidth = consoleWidth - screenMargin * 2;
    float screenHeight = consoleHeight * 0.35f;
    float screenX = consoleX + screenMargin;
    float screenY = consoleY + screenMargin;

    // Zone clavier - sous l'écran
    float keyboardMargin = screenMargin;
    float keyboardHeight = consoleHeight * 0.45f;
    float keyboardWidth = screenWidth;
    float keyboardX = screenX;
    float keyboardY = screenY + screenHeight + screenMargin;

    float octaveHeight = keyboardHeight * 0.48f; // Chaque octave prend un peu moins de la moitié
    float octaveSpacing = keyboardHeight * 0.04f; // Espacement entre les octaves
    float octave1Y = keyboardY + octaveHeight * 0.05f;
    float octave2Y = octave1Y + octaveHeight + octaveSpacing;

    const int NATURAL_NOTES = 7;

    float keySpacingX = keyboardWidth * 0.01f;
    float naturalKeyWidth = (keyboardWidth - keySpacingX * (NATURAL_NOTES + 1)) / NATURAL_NOTES;
    float naturalKeyHeight = octaveHeight * 0.85f;
    float accidentalKeyWidth = naturalKeyWidth * 0.6f;
    float accidentalKeyHeight = naturalKeyHeight * 0.6f;

    const bool hasAccidental[7] = {true, true, false, true, true, true, false};

    int noteIndex = 0;

    float octaveY[2] = {octave1Y, octave2Y};

    std::vector<GameButton> naturalButtons;
    std::vector<GameButton> accidentalButtons;

    for (int octave = 0; octave < 2 && noteIndex < actualKeys; octave++) {
        float currentY = octaveY[octave];

        for (int i = 0; i < NATURAL_NOTES && noteIndex < actualKeys; i++) {
            float keyX = keyboardX + keySpacingX + i * (naturalKeyWidth + keySpacingX);

            GameButton naturalButton;
            naturalButton.note = noteIndex++;
            naturalButton.isHovered = false;
            naturalButton.rect.x = keyX;
            naturalButton.rect.y = currentY;
            naturalButton.rect.w = naturalKeyWidth;
            naturalButton.rect.h = naturalKeyHeight;
            naturalButtons.push_back(naturalButton);

            if (hasAccidental[i] && noteIndex < actualKeys) {
                GameButton accidentalButton;
                accidentalButton.note = noteIndex++;
                accidentalButton.isHovered = false;
                accidentalButton.rect.x = keyX + naturalKeyWidth - accidentalKeyWidth * 0.5f;
                accidentalButton.rect.y = currentY - accidentalKeyHeight * 0.15f;
                accidentalButton.rect.w = accidentalKeyWidth;
                accidentalButton.rect.h = accidentalKeyHeight;
                accidentalButtons.push_back(accidentalButton);
            }
        }
    }

    // Ajouter d'abord les touches naturelles (blanches), puis les touches accidentelles (noires)
    buttons.insert(buttons.end(), naturalButtons.begin(), naturalButtons.end());
    buttons.insert(buttons.end(), accidentalButtons.begin(), accidentalButtons.end());

    // Assurer que hoveredButtonIndex est réinitialisé lors de la recalculation du layout
    hoveredButtonIndex = -1;
}

void VideoGame::calculateControlsLayout() {
    controls.clear();

    float consoleWidth = width * 0.98f;
    float consoleHeight = height * 0.95f;
    float consoleX = x + (width - consoleWidth) / 2;
    float consoleY = y + (height - consoleHeight) / 2;

    float screenMargin = consoleHeight * 0.025f;
    float screenWidth = consoleWidth - screenMargin * 2;
    float screenHeight = consoleHeight * 0.35f;
    float screenX = consoleX + screenMargin;
    float screenY = consoleY + screenMargin;

    // Zone clavier - sous l'écran
    float keyboardMargin = screenMargin;
    float keyboardHeight = consoleHeight * 0.45f;
    float keyboardWidth = screenWidth;
    float keyboardX = screenX;
    float keyboardY = screenY + screenHeight + screenMargin;

    // Zone contrôles - tout en bas
    float controlsHeight = consoleHeight * 0.12f;
    float controlsWidth = screenWidth;
    float controlsX = screenX;
    float controlsY = keyboardY + keyboardHeight + screenMargin * 0.5f;

    // D-pad (croix directionnelle) à gauche
    float dpadSize = controlsHeight * 0.8f;
    float dpadX = controlsX + controlsWidth * 0.15f - dpadSize / 2;
    float dpadY = controlsY + (controlsHeight - dpadSize) / 2;
    float dpadThickness = dpadSize * 0.3f;

    // D-pad haut
    ConsoleControl dpadUp;
    dpadUp.type = DPAD_UP;
    dpadUp.isHovered = false;
    dpadUp.rect.x = dpadX + dpadSize / 2 - dpadThickness / 2;
    dpadUp.rect.y = dpadY;
    dpadUp.rect.w = dpadThickness;
    dpadUp.rect.h = dpadSize / 2;
    controls.push_back(dpadUp);

    // D-pad droite
    ConsoleControl dpadRight;
    dpadRight.type = DPAD_RIGHT;
    dpadRight.isHovered = false;
    dpadRight.rect.x = dpadX + dpadSize / 2;
    dpadRight.rect.y = dpadY + dpadSize / 2 - dpadThickness / 2;
    dpadRight.rect.w = dpadSize / 2;
    dpadRight.rect.h = dpadThickness;
    controls.push_back(dpadRight);

    // D-pad bas
    ConsoleControl dpadDown;
    dpadDown.type = DPAD_DOWN;
    dpadDown.isHovered = false;
    dpadDown.rect.x = dpadX + dpadSize / 2 - dpadThickness / 2;
    dpadDown.rect.y = dpadY + dpadSize / 2;
    dpadDown.rect.w = dpadThickness;
    dpadDown.rect.h = dpadSize / 2;
    controls.push_back(dpadDown);

    // D-pad gauche
    ConsoleControl dpadLeft;
    dpadLeft.type = DPAD_LEFT;
    dpadLeft.isHovered = false;
    dpadLeft.rect.x = dpadX;
    dpadLeft.rect.y = dpadY + dpadSize / 2 - dpadThickness / 2;
    dpadLeft.rect.w = dpadSize / 2;
    dpadLeft.rect.h = dpadThickness;
    controls.push_back(dpadLeft);

    // Boutons A, B à droite
    float buttonDiameter = controlsHeight * 0.5f;
    float buttonSpacing = buttonDiameter * 0.3f;
    float buttonsX = controlsX + controlsWidth * 0.85f - buttonDiameter;
    float buttonsY = controlsY + (controlsHeight - buttonDiameter) / 2;

    // Bouton A
    ConsoleControl buttonA;
    buttonA.type = BUTTON_A;
    buttonA.isHovered = false;
    buttonA.rect.x = buttonsX - buttonSpacing - buttonDiameter;
    buttonA.rect.y = buttonsY;
    buttonA.rect.w = buttonDiameter;
    buttonA.rect.h = buttonDiameter;
    controls.push_back(buttonA);

    // Bouton B
    ConsoleControl buttonB;
    buttonB.type = BUTTON_B;
    buttonB.isHovered = false;
    buttonB.rect.x = buttonsX;
    buttonB.rect.y = buttonsY;
    buttonB.rect.w = buttonDiameter;
    buttonB.rect.h = buttonDiameter;
    controls.push_back(buttonB);

    // Petits boutons Start et Select au centre
    float smallButtonWidth = buttonDiameter;
    float smallButtonHeight = buttonDiameter * 0.4f;
    float smallButtonSpacing = smallButtonWidth * 0.3f;
    float smallButtonsX = controlsX + controlsWidth * 0.5f - smallButtonWidth / 2 - smallButtonSpacing / 2;

    // Bouton Select
    ConsoleControl selectButton;
    selectButton.type = BUTTON_SELECT;
    selectButton.isHovered = false;
    selectButton.rect.x = smallButtonsX;
    selectButton.rect.y = controlsY + (controlsHeight - smallButtonHeight) / 2;
    selectButton.rect.w = smallButtonWidth;
    selectButton.rect.h = smallButtonHeight;
    controls.push_back(selectButton);

    // Bouton Start
    ConsoleControl startButton;
    startButton.type = BUTTON_START;
    startButton.isHovered = false;
    startButton.rect.x = smallButtonsX + smallButtonWidth + smallButtonSpacing;
    startButton.rect.y = controlsY + (controlsHeight - smallButtonHeight) / 2;
    startButton.rect.w = smallButtonWidth;
    startButton.rect.h = smallButtonHeight;
    controls.push_back(startButton);

    // Réinitialiser l'index du contrôle survolé
    hoveredControlIndex = -1;
}

std::string VideoGame::getNoteAt(float mouseX, float mouseY) const {
    int buttonIndex = getButtonAt(mouseX, mouseY);
    if (buttonIndex == -1) {
        return "";
    }

    return "8bit_" + std::to_string(buttonIndex);
}

bool VideoGame::getNoteInfoAt(float mouseX, float mouseY, std::string &noteName, SDL_FRect &buttonRect) const {
    int buttonIndex = getButtonAt(mouseX, mouseY);
    if (buttonIndex == -1) {
        noteName = "";
        return false;
    }

    noteName = "8bit_" + std::to_string(buttonIndex);
    buttonRect = buttons[buttonIndex].rect;
    return true;
}

int VideoGame::getButtonAt(float mouseX, float mouseY) const {
    // Vérifier les touches noires d'abord (en ordre inversé pour priorité)
    for (int i = static_cast<int>(buttons.size()) - 1; i >= 0; i--) {
        const GameButton &button = buttons[i];
        if (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w &&
            mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h) {
            return i;
        }
    }
    return -1;
}

bool VideoGame::updateHoveredButton(float mouseX, float mouseY) {
    int previousHoveredIndex = hoveredButtonIndex;

    // Réinitialiser tous les états de survol
    for (GameButton &button: buttons) {
        button.isHovered = false;
    }

    // Obtenir le bouton survolé (en priorité les touches noires qui sont en fin de tableau)
    hoveredButtonIndex = getButtonAt(mouseX, mouseY);

    // Mettre à jour l'état de survol du bouton trouvé
    if (hoveredButtonIndex != -1) {
        buttons[hoveredButtonIndex].isHovered = true;
    }

    return previousHoveredIndex != hoveredButtonIndex;
}

void VideoGame::clearHoveredButtons() {
    hoveredButtonIndex = -1;
    for (GameButton &button: buttons) {
        button.isHovered = false;
    }
}

int VideoGame::getControlAt(float mouseX, float mouseY) const {
    for (size_t i = 0; i < controls.size(); i++) {
        const ConsoleControl &control = controls[i];
        if (mouseX >= control.rect.x && mouseX <= control.rect.x + control.rect.w &&
            mouseY >= control.rect.y && mouseY <= control.rect.y + control.rect.h) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

bool VideoGame::updateHoveredControl(float mouseX, float mouseY) {
    int previousHoveredIndex = hoveredControlIndex;

    for (ConsoleControl &control: controls) {
        control.isHovered = false;
    }

    hoveredControlIndex = getControlAt(mouseX, mouseY);

    if (hoveredControlIndex != -1) {
        controls[hoveredControlIndex].isHovered = true;
    }

    return previousHoveredIndex != hoveredControlIndex;
}

void VideoGame::clearHoveredControls() {
    hoveredControlIndex = -1;
    for (ConsoleControl &control: controls) {
        control.isHovered = false;
    }
}