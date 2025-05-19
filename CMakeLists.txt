cmake_minimum_required(VERSION 3.20)
project(MusicaLau VERSION 1.0)

# Spécifier la version du C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Répertoires d'inclusion
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/SDL3)
include_directories(${PROJECT_SOURCE_DIR})

# Définir les sources pour la bibliothèque (sans utiliser GLOB_RECURSE pour un meilleur contrôle)
set(LIB_SOURCES
        # Models
        src/Model/Instrument.cpp
        src/Model/Piano.cpp
        src/Model/Xylophone.cpp
        src/Model/VideoGame.cpp
        src/Model/Score.cpp
        src/Model/Note.cpp

        # Views
        src/View/View.cpp
        src/View/InstrumentSelectionView.cpp
        src/View/PianoView.cpp
        src/View/XylophoneView.cpp
        src/View/VideoGameView.cpp
        src/View/ConsoleView.cpp

        # Controllers
        src/Controller/Controller.cpp
        src/Controller/InstrumentController.cpp
        src/Controller/ScoreController.cpp
        src/Controller/PianoAppController.cpp
        src/Controller/XylophoneAppController.cpp
        src/Controller/VideoGameAppController.cpp
        src/Controller/MusicController.cpp

        # Audio
        src/Audio/SDLAudioEngine.cpp

        # Instruments
        src/Instruments/SimpleSynthInstrument.cpp
        src/Instruments/PianoInstrument.cpp
        src/Instruments/GuitarInstrument.cpp
        src/Instruments/XylophoneInstrument.cpp

        # Application
        src/Application.cpp

        # Utils
        src/utils/AudioGenerator.cpp
        src/utils/FileManager.cpp
        src/utils/RandomMelodyGenerator.cpp
        src/utils/audio_utils.cpp
        src/utils/file_utils.cpp
        src/utils/DropdownMenu.cpp
        src/utils/TextHelper.cpp
        devfile.cpp
)

# Définir les en-têtes (headers) pour une meilleure visibilité dans l'IDE
set(LIB_HEADERS
        # Models
        include/Model/Instrument.h
        include/Model/Piano.h
        include/Model/Xylophone.h
        include/Model/VideoGame.h
        include/Model/Score.h
        include/Model/Note.h

        # Views
        include/View/View.h
        include/View/InstrumentSelectionView.h
        include/View/PianoView.h
        include/View/XylophoneView.h
        include/View/VideoGameView.h
        include/View/ConsoleView.h

        # Controllers
        include/Controller/Controller.h
        include/Controller/InstrumentController.h
        include/Controller/ScoreController.h
        include/Controller/PianoAppController.h
        include/Controller/XylophoneAppController.h
        include/Controller/VideoGameAppController.h
        include/Controller/MusicController.h

        # Audio
        include/Audio/AudioEngine.h
        include/Audio/SDLAudioEngine.h

        # Instruments
        include/Instruments/SimpleSynthInstrument.h
        include/Instruments/PianoInstrument.h
        include/Instruments/GuitarInstrument.h
        include/Instruments/XylophoneInstrument.h

        # Application
        include/Application.h

        # Utils
        include/utils/AudioGenerator.h
        include/utils/FileManager.h
        include/utils/RandomMelodyGenerator.h
        include/utils/audio_utils.h
        include/utils/file_utils.h
        include/utils/DropdownMenu.h
        include/utils/TextHelper.h

        #Core
        include/Core/Note.h
        include/Core/Instrument.h
)

# Ajouter les bibliothèques SDL3
set(SDL3_LIBS
        ${PROJECT_SOURCE_DIR}/lib/libSDL3.dll.a
        ${PROJECT_SOURCE_DIR}/lib/libSDL3_ttf.dll.a
        ${PROJECT_SOURCE_DIR}/lib/libSDL3_image.dll.a
        ${PROJECT_SOURCE_DIR}/lib/libSDL2_mixer.dll.a
)

# Définir des options de préprocesseur pour SDL_mixer
add_compile_definitions(SDL_MIXER_INCLUDED=1)

# Création de la bibliothèque
add_library(MusicaLauLib ${LIB_SOURCES} ${LIB_HEADERS})

# Spécifier des flags de compilation supplémentaires si nécessaire
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(MusicaLauLib PRIVATE -Wno-deprecated-declarations)
endif ()

# Compilation de l'exécutable principal
add_executable(MusicaLau src/main.cpp)
target_link_libraries(MusicaLau MusicaLauLib ${SDL3_LIBS})

# Configuration des tests
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
    add_subdirectory(tests)
else ()
    # Créer un répertoire de tests vide si besoin
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

    # Créer un CMakeLists.txt simple pour les tests
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt
            "# Tests Configuration\n\n# Ajoutez vos tests ici\n")

    add_subdirectory(tests)
endif ()

# Création des répertoires d'assets s'ils n'existent pas
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets/sounds)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets/scores)

# Copier les ressources dans le répertoire de sortie
file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Copier les DLLs SDL3 dans le répertoire de sortie
file(COPY ${PROJECT_SOURCE_DIR}/lib/SDL3.dll DESTINATION ${PROJECT_SOURCE_DIR}/bin)
file(COPY ${PROJECT_SOURCE_DIR}/lib/SDL3_ttf.dll DESTINATION ${PROJECT_SOURCE_DIR}/bin)
file(COPY ${PROJECT_SOURCE_DIR}/lib/SDL3_image.dll DESTINATION ${PROJECT_SOURCE_DIR}/bin)
# Pour SDL_mixer, nous utilisons la version SDL2 en la copiant directement
file(COPY ${PROJECT_SOURCE_DIR}/lib/SDL2_mixer.dll DESTINATION ${PROJECT_SOURCE_DIR}/bin)
message(STATUS "Note: SDL2_mixer.dll est utilisé comme SDL3_mixer.dll")

# Définir la sortie de l'exécutable
set_target_properties(MusicaLau PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

# Messages de statut de configuration
message(STATUS "Project '${PROJECT_NAME}' configured with C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")