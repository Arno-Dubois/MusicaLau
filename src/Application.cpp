#include "../include/Application.h"
#include <iostream>
#include <SDL3/SDL_ttf.h>
#include <unordered_map>

Application::Application(int width, int height)
        : window(nullptr),
          renderer(nullptr),
          mainController(nullptr),
          audioEngine(nullptr),
          windowWidth(width),
          windowHeight(height),
          initialized(false),
          currentInstrument(InstrumentType::PIANO),
          instrumentMenu(nullptr),
          currentPlayingNote(""),
isMouseButtonDown(false),
sdlAudioEngine(nullptr), // Initialize SDLAudioEngine pointer
songPlayer(nullptr),
          lastNotePlayTime(0) {
    // Initialiser le mapping clavier-notes
    initializeKeyboardMappings();
}

Application::~Application() {
    cleanup();
}

void Application::initializeInstrumentMenu() {
    // On utilise des valeurs relatives aux dimensions de la fenêtre
    float toolbarY = 20.0f;
    float buttonHeight = windowHeight * 0.09f; // ~9% de la hauteur de la fenêtre
    float spacing = windowHeight * 0.03f; // ~3% de la hauteur

    // Position et dimensions adaptatives du rectangle bleu
    float mainAreaX = windowWidth * 0.035f; // ~3.5% de la largeur
    float mainAreaY = toolbarY + buttonHeight + spacing;
    float mainAreaWidth = windowWidth * 0.93f; // ~93% de la largeur
    float headerHeight = windowHeight * 0.05f; // ~5% de la hauteur

    // Créer le menu déroulant avec le nom de l'instrument actif
    std::string headerLabel;
    switch (currentInstrument) {
        case InstrumentType::PIANO:
            headerLabel = "Piano";
            break;
        case InstrumentType::XYLOPHONE:
            headerLabel = "Xylophone";
            break;
        case InstrumentType::VIDEO_GAME:
            headerLabel = "Jeu vidéo";
            break;
    }

    instrumentMenu = new DropdownMenu(mainAreaX, mainAreaY, mainAreaWidth, headerHeight, headerLabel);

    // Ajouter les éléments au menu
    instrumentMenu->addItem("Piano", [this]() {
        setInstrument(InstrumentType::PIANO);
        // Mettre à jour le texte du menu après changement
        initializeInstrumentMenu();
    });

    instrumentMenu->addItem("Xylophone", [this]() {
        setInstrument(InstrumentType::XYLOPHONE);
        // Mettre à jour le texte du menu après changement
        initializeInstrumentMenu();
    });

    instrumentMenu->addItem("Jeu vidéo", [this]() {
        setInstrument(InstrumentType::VIDEO_GAME);
        // Mettre à jour le texte du menu après changement
        initializeInstrumentMenu();
    });
}

bool Application::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Initialiser SDL_ttf
    if (TTF_Init() < 0) {
        SDL_Log("SDL_ttf could not initialize! Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    std::cout << "Application::initialize: About to create SDLAudioEngine." << std::endl;
    sdlAudioEngine = new MusicApp::Audio::SDLAudioEngine();
    std::cout << "Application::initialize: SDLAudioEngine created at address: " << sdlAudioEngine << std::endl;

    if (!sdlAudioEngine) {
        std::cerr << "Application::initialize FATAL ERROR: new SDLAudioEngine failed!" << std::endl;
        SDL_Quit();
        return false;
    }
    if (!sdlAudioEngine->init()) {
        std::cerr << "Application::initialize FATAL ERROR: sdlAudioEngine->init() failed!" << std::endl; // Changed log
        delete sdlAudioEngine; // Cleanup
        sdlAudioEngine = nullptr;
        SDL_Quit();
        return false;
    }
    std::cout << "Application::initialize: sdlAudioEngine initialized successfully." << std::endl;
    audioEngine = sdlAudioEngine;

    std::cout << "Application::initialize: About to create SongPlayer. Passing sdlAudioEngine address: " << sdlAudioEngine << std::endl;
    songPlayer = new MusicApp::Audio::SongPlayer(sdlAudioEngine);
    std::cout << "Application::initialize: songPlayer created at address: " << songPlayer << std::endl;
    if (!songPlayer) {
        std::cerr << "Application::initialize FATAL ERROR: new SongPlayer failed (returned nullptr)!" << std::endl;
        // Handle error appropriately
    } else {
        std::cout << "Application::initialize: SongPlayer successfully created." << std::endl;
    }

    window = SDL_CreateWindow("MusicaLau - Instrument Interface", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    // Initialiser le menu déroulant
    try {
        initializeInstrumentMenu();
        if (!instrumentMenu) {
            SDL_Log("Failed to initialize instrument menu!");
            return false;
        }
    } catch (const std::exception &e) {
        SDL_Log("Exception during menu initialization: %s", e.what());
        return false;
    }

    // Créer le contrôleur par défaut (Piano)
    try {
        setInstrument(currentInstrument);
        if (!mainController) {
            SDL_Log("Failed to create main controller!");
            return false;
        }
    } catch (const std::exception &e) {
        SDL_Log("Exception during controller creation: %s", e.what());
        return false;
    }

    initialized = true;
    return true;
}

void Application::setInstrument(InstrumentType instrument) {
    // Nettoyer le contrôleur existant
    delete mainController;

    // Créer le nouveau contrôleur
    switch (instrument) {
        case InstrumentType::PIANO:
            mainController = new PianoAppController(windowWidth, windowHeight, audioEngine);
            break;
        case InstrumentType::XYLOPHONE:
            mainController = new XylophoneAppController(windowWidth, windowHeight, audioEngine);
            break;
        case InstrumentType::VIDEO_GAME:
            mainController = new VideoGameAppController(windowWidth, windowHeight, audioEngine);
            break;
    }

    currentInstrument = instrument;
}

bool Application::run() {
    if (!initialized) {
        if (!initialize()) {
            return false;
        }
    }

    bool quit = false;
    SDL_Event event;

    std::unordered_map<std::string, bool> activeNotes;

    Uint32 lastUpdateTime = SDL_GetTicks();

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    quit = true;
                } else {
                    handleKeyPress(event.key.key);
                }
            } else if (event.type == SDL_EVENT_KEY_UP) {
                handleKeyRelease(event.key.key);
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                float mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                bool clickHandled = instrumentMenu->handleClick(mouseX, mouseY);

                if (!clickHandled && !instrumentMenu->isMenuOpen()) {
                    int buttonClicked = mainController->handleButtonClick(mouseX, mouseY);

                    if (buttonClicked == 3) {
                        // "Import File" button
                        mainController->handleImportSong();
                    } else if (buttonClicked == 4) { // "Play Song" button
                        std::string currentInstrumentNameString;
                        switch (currentInstrument) {
                            case InstrumentType::PIANO: currentInstrumentNameString = "Piano"; break;
                            case InstrumentType::XYLOPHONE: currentInstrumentNameString = "Xylophone"; break;
                            case InstrumentType::VIDEO_GAME: currentInstrumentNameString = "8BitConsole"; break;
                        }
                        // Log before calling mainController methods
                        std::cout << "Application::run: About to call mainController->handlePlaySongClicked. mainController pointer: " << mainController << std::endl;
                        if (mainController) {
                            mainController->handlePlaySongClicked(currentInstrumentNameString);
                        } else {
                            std::cerr << "Application::run ERROR: mainController is NULL before handlePlaySongClicked!" << std::endl;
                        }

                        // Log before checking conditions and calling songPlayer
                        std::cout << "Application::run: About to check conditions for songPlayer->playSong." << std::endl;
                        std::cout << "Application::run: songPlayer pointer: " << songPlayer << std::endl;
                        if (mainController) {
                             std::cout << "Application::run: mainController->isSongReadyToPlay(): " << mainController->isSongReadyToPlay() << std::endl;
                        }


                        if (songPlayer && mainController && mainController->isSongReadyToPlay()) {
                           std::cout << "Application::run: Conditions MET. Calling songPlayer->playSong." << std::endl;
                           std::cout << "Application::run: songPlayer pointer before call: " << songPlayer << std::endl;
                           std::cout << "Application::run: mainController pointer for getters: " << mainController << std::endl;
                           // Ensure events are valid before passing
                           const auto& events = mainController->getLoadedSongEvents();
                           const std::string& instrName = mainController->getCurrentInstrumentForSong();
                           std::cout << "Application::run: Event count: " << events.size() << ", Instrument for song: " << instrName << std::endl;

                           songPlayer->playSong(events, instrName);
                           // Optionally: mainController->resetPlayRequestStatus();
                        } else {
                            std::cout << "Application::run: Conditions NOT MET for songPlayer->playSong." << std::endl;
                            if (!songPlayer) std::cerr << "Application::run: songPlayer is NULL." << std::endl;
                            if (!mainController) std::cerr << "Application::run: mainController is NULL." << std::endl;
                            else if (!mainController->isSongReadyToPlay()) std::cerr << "Application::run: mainController->isSongReadyToPlay() is false." << std::endl;
                        }
                    } else if (PianoAppController *pianoController = dynamic_cast<PianoAppController *>(
                        mainController)) {
                        if (buttonClicked != -1) {
                            pianoController->processButtonAction(buttonClicked);
                        } else {
                            std::string pitchName = pianoController->getPiano()->getPitchAt(mouseX, mouseY);
                            if (!pitchName.empty()) {
                                MusicApp::Core::Note note(pitchName);
                                audioEngine->playSound("Piano", note);
                                activeNotes[pitchName] = true;
                                currentPlayingNote = pitchName;
                                isMouseButtonDown = true;
                                lastNotePlayTime = SDL_GetTicks();
                            }
                        }
                    } else if (XylophoneAppController *xylophoneController = dynamic_cast<XylophoneAppController *>(
                        mainController)) {
                        if (buttonClicked != -1) {
                            xylophoneController->processButtonAction(buttonClicked);
                        } else {
                            int barIndex = xylophoneController->getXylophone()->getBarAt(mouseX, mouseY);
                            if (barIndex != -1) {
                                // Convertir l'index en note
                                std::string noteName;
                                switch (barIndex) {
                                    case 0:
                                        noteName = "C4";
                                        break;
                                    case 1:
                                        noteName = "D4";
                                        break;
                                    case 2:
                                        noteName = "E4";
                                        break;
                                    case 3:
                                        noteName = "F4";
                                        break;
                                    case 4:
                                        noteName = "G4";
                                        break;
                                    case 5:
                                        noteName = "A4";
                                        break;
                                    case 6:
                                        noteName = "B4";
                                        break;
                                    case 7:
                                        noteName = "C5";
                                        break;
                                    case 8:
                                        noteName = "D5";
                                        break;
                                    case 9:
                                        noteName = "E5";
                                        break;
                                    case 10:
                                        noteName = "F5";
                                        break;
                                    case 11:
                                        noteName = "G5";
                                        break;
                                    default:
                                        noteName = "C4";
                                        break;
                                }

                                // Stocker les informations de la note jouée
                                currentPlayingNote = noteName;
                                isMouseButtonDown = true;
                                lastNotePlayTime = SDL_GetTicks();
                            }

                            // Appeler la méthode de gestion du clic
                            xylophoneController->handleXylophoneKeyClick(mouseX, mouseY);
                        }
                    } else if (VideoGameAppController *videoGameController = dynamic_cast<VideoGameAppController *>(mainController)) {
                        if (buttonClicked != -1) {
                            videoGameController->processButtonAction(buttonClicked);
                        } else {
                            videoGameController->handleVideoGameKeyClick(mouseX, mouseY);

                            // Stocker l'information de la note actuellement jouée
                            std::string noteName = videoGameController->getVideoGame()->getNoteAt(mouseX, mouseY);
                            if (!noteName.empty()) {
                                currentPlayingNote = noteName;
                                isMouseButtonDown = true;
                                lastNotePlayTime = SDL_GetTicks();
                            }
                        }
                    }
                }
            } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                float mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                instrumentMenu->updateHoverState(mouseX, mouseY);

                if (!instrumentMenu->isMenuOpen()) {
                    if (PianoAppController *pianoController = dynamic_cast<PianoAppController *>(mainController)) {
                        pianoController->handlePianoKeyHover(mouseX, mouseY);
                    } else if (XylophoneAppController *xylophoneController = dynamic_cast<XylophoneAppController *>(mainController)) {
                        xylophoneController->handleXylophoneKeyHover(mouseX, mouseY);
                    } else if (VideoGameAppController *videoGameController = dynamic_cast<VideoGameAppController *>(mainController)) {
                        videoGameController->handleVideoGameKeyHover(mouseX, mouseY);
                    }
                }
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                float mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (PianoAppController *pianoController = dynamic_cast<PianoAppController *>(mainController)) {
                    std::string pitchName = pianoController->getPiano()->getPitchAt(mouseX, mouseY);
                    if (!pitchName.empty()) {
                        MusicApp::Core::Note note(pitchName);
                        MusicApp::Audio::SDLAudioEngine *engine = dynamic_cast<MusicApp::Audio::SDLAudioEngine *>(audioEngine);
                        if (engine) {
                            engine->stopSound("Piano", note);
                            currentPlayingNote = "";
                            isMouseButtonDown = false;
                        }
                    }
                } else if (XylophoneAppController *xylophoneController = dynamic_cast<XylophoneAppController *>(mainController)) {
                    int barIndex = xylophoneController->getXylophone()->getBarAt(mouseX, mouseY);
                    if (barIndex != -1) {
                        std::string noteName;
                        switch (barIndex) {
                            case 0:
                                noteName = "C4";
                                break;
                            case 1:
                                noteName = "D4";
                                break;
                            case 2:
                                noteName = "E4";
                                break;
                            case 3:
                                noteName = "F4";
                                break;
                            case 4:
                                noteName = "G4";
                                break;
                            case 5:
                                noteName = "A4";
                                break;
                            case 6:
                                noteName = "B4";
                                break;
                            case 7:
                                noteName = "C5";
                                break;
                            case 8:
                                noteName = "D5";
                                break;
                            case 9:
                                noteName = "E5";
                                break;
                            case 10:
                                noteName = "F5";
                                break;
                            case 11:
                                noteName = "G5";
                                break;
                            default:
                                noteName = "C4";
                                break;
                        }

                        MusicApp::Core::Note note(noteName);
                        MusicApp::Audio::SDLAudioEngine *engine = dynamic_cast<MusicApp::Audio::SDLAudioEngine *>(audioEngine);
                        if (engine) {
                            engine->stopSound("Xylophone", note);
                            currentPlayingNote = "";
                            isMouseButtonDown = false;
                        }
                    }
                } else if (VideoGameAppController *videoGameController = dynamic_cast<VideoGameAppController *>(mainController)) {
                    std::string noteName = videoGameController->getVideoGame()->getNoteAt(mouseX, mouseY);
                    if (!noteName.empty()) {
                        MusicApp::Core::Note note(noteName);
                        MusicApp::Audio::SDLAudioEngine *engine = dynamic_cast<MusicApp::Audio::SDLAudioEngine *>(audioEngine);
                        if (engine) {
                            engine->stopSound("8BitConsole", note);
                            currentPlayingNote = "";
                            isMouseButtonDown = false;
                        }
                    }
                }
            } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                SDL_GetWindowSizeInPixels(window, &windowWidth, &windowHeight);

                if (instrumentMenu) {
                    delete instrumentMenu;
                    initializeInstrumentMenu();
                }

                if (mainController) {
                    setInstrument(currentInstrument);
                }
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastUpdateTime > 100) {
            lastUpdateTime = currentTime;

            if (isMouseButtonDown && !currentPlayingNote.empty() && (currentTime - lastNotePlayTime >= 450)) {
                if (PianoAppController *pianoController = dynamic_cast<PianoAppController *>(mainController)) {
                    MusicApp::Core::Note note(currentPlayingNote);
                    audioEngine->playSound("Piano", note);
                    lastNotePlayTime = currentTime;
                } else if (XylophoneAppController *xylophoneController = dynamic_cast<XylophoneAppController *>(mainController)) {
                    MusicApp::Core::Note note(currentPlayingNote);
                    audioEngine->playSound("Xylophone", note);
                    lastNotePlayTime = currentTime;
                } else if (VideoGameAppController *videoGameController = dynamic_cast<VideoGameAppController *>(mainController)) {
                    MusicApp::Core::Note note(currentPlayingNote);
                    audioEngine->playSound("8BitConsole", note);
                    lastNotePlayTime = currentTime;
                }
            }

            MusicApp::Audio::SDLAudioEngine *engine = dynamic_cast<MusicApp::Audio::SDLAudioEngine *>(audioEngine);
            if (engine) {
                engine->cleanupLongPlayingNotes(1000);
            }
        }

        SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
        SDL_RenderClear(renderer);

        mainController->render(renderer, windowWidth, windowHeight);
        instrumentMenu->render(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return true;
}

void Application::cleanup() {
    delete mainController;
    mainController = nullptr;

    delete instrumentMenu;
    instrumentMenu = nullptr;

    // Shutdown and delete AudioEngine
    if (audioEngine) {
        audioEngine->shutdown();
        delete audioEngine;
        audioEngine = nullptr;
    }

    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    TTF_Quit();
    SDL_Quit();
    initialized = false;
}

void Application::initializeKeyboardMappings() {
    // Créer le mapping selon la disposition demandée
    // Format: {code_touche, nom_note, octave}
    keyboardMappings.clear(); // S'assurer que le vecteur est vide avant d'ajouter des éléments

    // Ajouter toutes les correspondances touches/notes
    keyboardMappings.push_back({SDLK_Q, "C", 4});   // Q -> Do
    keyboardMappings.push_back({SDLK_Z, "C#", 4});  // Z -> Do#
    keyboardMappings.push_back({SDLK_S, "D", 4});   // S -> Ré
    keyboardMappings.push_back({SDLK_E, "D#", 4});  // E -> Ré#
    keyboardMappings.push_back({SDLK_D, "E", 4});   // D -> Mi
    keyboardMappings.push_back({SDLK_F, "F", 4});   // F -> Fa
    keyboardMappings.push_back({SDLK_T, "F#", 4});  // T -> Fa#
    keyboardMappings.push_back({SDLK_G, "G", 4});   // G -> Sol
    keyboardMappings.push_back({SDLK_Y, "G#", 4});  // Y -> Sol#
    keyboardMappings.push_back({SDLK_H, "A", 4});   // H -> La
    keyboardMappings.push_back({SDLK_U, "A#", 4});  // U -> La#
    keyboardMappings.push_back({SDLK_J, "B", 4});   // J -> Si
    keyboardMappings.push_back({SDLK_K, "C", 5});   // K -> Do (octave supérieure)
    keyboardMappings.push_back({SDLK_L, "D", 5});   // L -> Ré (octave supérieure)
    keyboardMappings.push_back({SDLK_M, "E", 5}); // M -> Mi (octave supérieure)
}

void Application::handleKeyPress(SDL_Keycode key) {
    std::string note = getNoteForKey(key);
    if (!note.empty()) {
        if (!keyboardNotesState[key]) {
            keyboardNotesState[key] = true;

            MusicApp::Core::Note musicNote(note);
            float velocity = getVelocityForKey();

            auto sdlAudioEngine = dynamic_cast<MusicApp::Audio::SDLAudioEngine *>(audioEngine);

            switch (currentInstrument) {
                case InstrumentType::PIANO:
                    if (sdlAudioEngine) {
                        sdlAudioEngine->playSound("Piano", musicNote, velocity);
                    } else if (audioEngine) {
                        audioEngine->playSound("Piano", musicNote);
                    }
                    break;

                case InstrumentType::XYLOPHONE:
                    if (sdlAudioEngine) {
                        sdlAudioEngine->playSound("Xylophone", musicNote, velocity);
                    } else if (audioEngine) {
                        audioEngine->playSound("Xylophone", musicNote);
                    }
                    break;

                case InstrumentType::VIDEO_GAME:
                    if (sdlAudioEngine) {
                        sdlAudioEngine->playSound("8BitConsole", musicNote, velocity);
                    } else if (audioEngine) {
                        audioEngine->playSound("8BitConsole", musicNote);
                    }
                    break;
            }

            SDL_Log("Note played via keyboard: %s, Instrument: %d", note.c_str(), static_cast<int>(currentInstrument));
        }
    }
}

void Application::handleKeyRelease(SDL_Keycode key) {
    std::string note = getNoteForKey(key);
    if (!note.empty()) {
        keyboardNotesState[key] = false;

        MusicApp::Core::Note musicNote(note);
        auto sdlAudioEngine = dynamic_cast<MusicApp::Audio::SDLAudioEngine *>(audioEngine);
        if (sdlAudioEngine) {
            switch (currentInstrument) {
                case InstrumentType::PIANO:
                    sdlAudioEngine->stopSound("Piano", musicNote);
                    break;
                case InstrumentType::XYLOPHONE:
                    sdlAudioEngine->stopSound("Xylophone", musicNote);
                    break;
                case InstrumentType::VIDEO_GAME:
                    sdlAudioEngine->stopSound("8BitConsole", musicNote);
                    break;
            }

            SDL_Log("Note stopped via keyboard: %s, Instrument: %d", note.c_str(), static_cast<int>(currentInstrument));
        }
    }
}

std::string Application::getNoteForKey(SDL_Keycode key) {
    for (const auto &mapping: keyboardMappings) {
        if (mapping.key == key) {
            return mapping.note + std::to_string(mapping.octave);
        }
    }
    return "";
}

float Application::getVelocityForKey() {
    return 0.8f;
}