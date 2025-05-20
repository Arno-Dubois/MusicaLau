#ifndef MUSICAPP_AUDIO_AUDIOENGINE_H
#define MUSICAPP_AUDIO_AUDIOENGINE_H

#include "../Core/Note.h"
#include <string>

namespace MusicApp {
    namespace Audio {

/**
 * @brief Interface de base pour le moteur audio.
 */
        class AudioEngine {
        public:
            virtual ~AudioEngine() = default;

            /**
             * @brief Initialise le moteur audio.
             * @return True si l'initialisation a réussi, false sinon.
             */
            virtual bool init() = 0;

            /**
             * @brief Ferme le moteur audio et libère les ressources.
             */
            virtual void shutdown() = 0;

            /**
             * @brief Joue un son correspondant à une note d'un instrument.
             *
             * @param instrumentName Identifiant de l'instrument
             * @param note La note musicale à jouer
             */
            virtual void playSound(const std::string &instrumentName, const Core::Note &note) = 0;
        };

    } // namespace Audio
} // namespace MusicApp

#endif // MUSICAPP_AUDIO_AUDIOENGINE_H
