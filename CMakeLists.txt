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
        src/models/Instrument.cpp
        src/models/Piano.cpp
        src/models/Xylophone.cpp
        src/models/VideoGame.cpp
        src/models/Score.cpp
        src/models/Note.cpp

        # Views
        src/views/View.cpp
        src/views/InstrumentSelectionView.cpp
        src/views/PianoView.cpp
        src/views/XylophoneView.cpp
        src/views/VideoGameView.cpp

        # Controllers
        src/controllers/Controller.cpp
        src/controllers/InstrumentController.cpp
        src/controllers/ScoreController.cpp

        # Utils
        src/utils/AudioGenerator.cpp
        src/utils/FileManager.cpp
        src/utils/RandomMelodyGenerator.cpp
        src/utils/audio_utils.cpp
        src/utils/file_utils.cpp
)

# Définir les en-têtes (headers) pour une meilleure visibilité dans l'IDE
set(LIB_HEADERS
        # Models
        include/models/Instrument.h
        include/models/Piano.h
        include/models/Xylophone.h
        include/models/VideoGame.h
        include/models/Score.h
        include/models/Note.h

        # Views
        include/views/View.h
        include/views/InstrumentSelectionView.h
        include/views/PianoView.h
        include/views/XylophoneView.h
        include/views/VideoGameView.h

        # Controllers
        include/controllers/Controller.h
        include/controllers/InstrumentController.h
        include/controllers/ScoreController.h

        # Utils
        include/utils/AudioGenerator.h
        include/utils/FileManager.h
        include/utils/RandomMelodyGenerator.h
        include/utils/audio_utils.h
        include/utils/file_utils.h
        devfile.cpp
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

# Définir la sortie de l'exécutable
set_target_properties(MusicaLau PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

# Messages de statut de configuration
message(STATUS "Project '${PROJECT_NAME}' configured with C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")