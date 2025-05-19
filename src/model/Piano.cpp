#include "../../include/Model/Piano.h"
#include <array> // For pitch names

Piano::Piano(float x, float y, float width, float height, int octaves)
        : x(x), y(y), width(width), height(height), octaves(octaves) {
    calculateKeyLayout(); // Calculate keys on construction
}

void Piano::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
    calculateKeyLayout(); // Recalculate on position change
}

void Piano::setDimensions(float newWidth, float newHeight) {
    width = newWidth;
    height = newHeight;
    calculateKeyLayout(); // Recalculate on dimension change
}

void Piano::addOctave() {
    octaves++;
    calculateKeyLayout(); // Recalculate when octaves change
}

void Piano::removeOctave() {
    if (octaves > 1) {
        octaves--;
        calculateKeyLayout(); // Recalculate when octaves change
    }
}

void Piano::calculateKeyLayout() {
    pianoKeys.clear();

    if (width <= 0 || height <= 0 || octaves <= 0) {
        return; // Cannot draw if dimensions or octaves are invalid
    }

    // Standard names for the 12 notes in an octave (starting with C)
    const std::array<std::string, 12> noteNamesChromatic = {
            "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"
    };
    // Indicates if a note at a chromatic index is a black key
    const std::array<bool, 12> isChromaticBlack = {
            false, true, false, true, false, false, true, false, true, false, true, false
    };
    // White key indices in a chromatic scale (0=C, 2=D, 4=E, 5=F, 7=G, 9=A, 11=B)
    const std::array<int, 7> whiteKeyChromaticIndices = {0, 2, 4, 5, 7, 9, 11};

    const int totalWhiteKeys = octaves * 7;
    if (totalWhiteKeys == 0) return;

    float whiteKeyWidth = width / totalWhiteKeys;
    float blackKeyWidth = whiteKeyWidth * 0.55f; // Typical relative width
    float blackKeyHeight = height * 0.65f;   // Typical relative height

    float currentX = x;
    int startingOctaveNumber = 4; // Example: Middle C is C4. Adjust if needed.

    // First pass: Layout all white keys
    for (int o = 0; o < octaves; ++o) {
        for (int i = 0; i < 7; ++i) { // 7 white keys per octave
            int chromaticIndex = whiteKeyChromaticIndices[i];
            std::string pitch = noteNamesChromatic[chromaticIndex] + std::to_string(startingOctaveNumber + o);

            PianoKey key;
            key.rect = {currentX, y, whiteKeyWidth, height};
            key.pitchName = pitch;
            key.isBlack = false;
            pianoKeys.push_back(key);
            currentX += whiteKeyWidth;
        }
    }

    // Second pass: Layout black keys, positioning them relative to white keys
    currentX = x; // Reset currentX for black key positioning
    int whiteKeyCounter = 0;
    for (int o = 0; o < octaves; ++o) {
        for (int i = 0; i < 7; ++i) { // Iterate based on white key structure
            // `j` is the index in the 12-note chromatic scale for the current white key
            int whiteKeyChromaIndex = whiteKeyChromaticIndices[i];

            // Check if a black key follows this white key (before the next white key)
            // Black keys are C#, D#, F#, G#, A#
            // Chromatic indices for these are 1, 3, 6, 8, 10
            // These occur after white keys C, D, F, G, A (chromatic indices 0, 2, 5, 7, 9)
            if (whiteKeyChromaIndex == 0 || whiteKeyChromaIndex == 2 || whiteKeyChromaIndex == 5 ||
                whiteKeyChromaIndex == 7 || whiteKeyChromaIndex == 9) {
                if ((whiteKeyChromaIndex + 1) < 12 && isChromaticBlack[whiteKeyChromaIndex + 1]) {
                    std::string pitch =
                            noteNamesChromatic[whiteKeyChromaIndex + 1] + std::to_string(startingOctaveNumber + o);

                    // Calculate position for the black key
                    // It should be at the boundary of the current white key and the next.
                    // currentX at this point is the START of the current white key.
                    float blackKeyX = currentX + whiteKeyWidth - (blackKeyWidth / 2.0f);

                    PianoKey key;
                    key.rect = {blackKeyX, y, blackKeyWidth, blackKeyHeight};
                    key.pitchName = pitch;
                    key.isBlack = true;
                    pianoKeys.push_back(key); // Add black key to the list
                }
            }
            currentX += whiteKeyWidth; // Advance to the start of the next white key
            whiteKeyCounter++;
        }
    }
}

std::string Piano::getPitchAt(float mouseX, float mouseY) const {
    // Iterate in reverse for black keys to be checked first (as they overlap white keys)
    for (auto it = pianoKeys.rbegin(); it != pianoKeys.rend(); ++it) {
        const auto &key = *it;
        if (mouseX >= key.rect.x && mouseX <= (key.rect.x + key.rect.w) &&
            mouseY >= key.rect.y && mouseY <= (key.rect.y + key.rect.h)) {
            return key.pitchName;
        }
    }
    return ""; // No key found at this position
}