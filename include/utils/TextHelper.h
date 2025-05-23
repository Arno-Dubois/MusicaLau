#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string>

/**
 * Fonctions d'aide pour le rendu de texte qui s'adaptent aux changements d'API entre les versions de SDL_ttf
 */
namespace TextHelper {
    /**
     * Cherche et charge une police en essayant plusieurs chemins
     * @param fontName Nom du fichier de police
     * @param ptsize Taille de la police en points
     * @return Police chargée ou nullptr si échec
     */
    TTF_Font *LoadFont(const std::string &fontName, int ptsize);

    /**
     * Fonction d'aide qui enveloppe TTF_RenderText_Solid pour s'adapter à la SDL3
     * @param font La police à utiliser
     * @param text Le texte à rendre
     * @param fg La couleur du texte
     * @return Surface contenant le texte rendu
     */
    SDL_Surface *RenderTextSolid(TTF_Font *font, const std::string &text, SDL_Color fg);
}