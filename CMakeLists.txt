cmake_minimum_required(VERSION 3.20)
project(MusicaLau VERSION 1.0)

# Spécifier la version du C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Répertoires d'inclusion
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/SDL3)

# Définir les sources pour la bibliothèque (sans utiliser GLOB_RECURSE pour un meilleur contrôle)
set(LIB_SOURCES
        # Models
        Src/Model/Instrument.cpp
        Src/Model/Piano.cpp
        Src/Model/Xylophone.cpp
        Src/Model/VideoGame.cpp
        Src/Model/Score.cpp
        Src/Model/Note.cpp

        # Views
        Src/View/View.cpp
        Src/View/InstrumentSelectionView.cpp
        Src/View/PianoView.cpp
        Src/View/XylophoneView.cpp
        Src/View/VideoGameView.cpp

        # Controllers
        Src/Controller/Controller.cpp
        Src/Controller/InstrumentController.cpp
        Src/Controller/ScoreController.cpp
        Src/Controller/PianoAppController.cpp
        Src/Controller/XylophoneAppController.cpp
        Src/Controller/VideoGameAppController.cpp

        # Application
        Src/Application.cpp

        # Utils
        Src/Utils/AudioGenerator.cpp
        Src/Utils/FileManager.cpp
        Src/Utils/RandomMelodyGenerator.cpp
        Src/Utils/audio_utils.cpp
        Src/Utils/file_utils.cpp
        Src/Utils/DropdownMenu.cpp
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

        # Controllers
        include/Controller/Controller.h
        include/Controller/InstrumentController.h
        include/Controller/ScoreController.h
        include/Controller/PianoAppController.h
        include/Controller/XylophoneAppController.h
        include/Controller/VideoGameAppController.h

        # Application
        include/Application.h

        # Utils
        include/Utils/AudioGenerator.h
        include/Utils/FileManager.h
        include/Utils/RandomMelodyGenerator.h
        include/Utils/audio_utils.h
        include/Utils/file_utils.h
        include/Utils/DropdownMenu.h
)

# Ajouter les répertoires d'inclusion pour SDL3
include_directories(${PROJECT_SOURCE_DIR}/include/SDL3)

# Ajouter les bibliothèques SDL3
set(SDL3_LIBS
        ${PROJECT_SOURCE_DIR}/lib/libSDL2_mixer.a
        ${PROJECT_SOURCE_DIR}/lib/libSDL3.dll.a
        ${PROJECT_SOURCE_DIR}/lib/libSDL3_ttf.dll.a
)

# Création de la bibliothèque
add_library(MusicaLauLib ${LIB_SOURCES} ${LIB_HEADERS})

# Compilation de l'exécutable principal
add_executable(MusicaLau Src/main.cpp)
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

# Définir la sortie de l'exécutable
set_target_properties(MusicaLau PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

# Messages de statut de configuration
message(STATUS "Project '${PROJECT_NAME}' configured with C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")