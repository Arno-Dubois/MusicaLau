cmake_minimum_required(VERSION 3.20)
project(MusicaLau VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Put the executable (and, on Windows, the SDL DLLs) in one folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach (cfg Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${cfg} CFG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFG} ${CMAKE_BINARY_DIR}/bin)
endforeach ()

# ---------------------------------------------------------------------------
# Dependencies: use SDL3 / SDL3_ttf from the system if available,
# otherwise download and build them automatically.
# ---------------------------------------------------------------------------
option(MUSICALAU_FETCH_SDL "Download SDL3 and SDL3_ttf if not found on the system" ON)

find_package(SDL3 CONFIG QUIET)
find_package(SDL3_ttf CONFIG QUIET)

if (NOT (SDL3_FOUND AND SDL3_ttf_FOUND))
    if (NOT MUSICALAU_FETCH_SDL)
        message(FATAL_ERROR "SDL3 / SDL3_ttf not found. Install them or enable MUSICALAU_FETCH_SDL.")
    endif ()
    message(STATUS "SDL3 / SDL3_ttf not found on the system, fetching them")
    include(FetchContent)
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
    set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
    set(SDLTTF_SAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-3.2.10
            GIT_SHALLOW TRUE)
    FetchContent_Declare(SDL3_ttf
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
            GIT_TAG release-3.2.2
            GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(SDL3 SDL3_ttf)
endif ()

# ---------------------------------------------------------------------------
# Application
# ---------------------------------------------------------------------------
add_executable(MusicaLau
        src/main.cpp
        src/Application.cpp

        # Model
        src/Model/Instrument.cpp
        src/Model/Piano.cpp
        src/Model/Xylophone.cpp
        src/Model/VideoGame.cpp

        # View
        src/View/View.cpp
        src/View/InstrumentSelectionView.cpp
        src/View/PianoView.cpp
        src/View/XylophoneView.cpp
        src/View/VideoGameView.cpp
        src/View/ConsoleView.cpp
        src/View/ButtonView.cpp

        # Controller
        src/Controller/Controller.cpp
        src/Controller/InstrumentController.cpp
        src/Controller/PianoAppController.cpp
        src/Controller/XylophoneAppController.cpp
        src/Controller/VideoGameAppController.cpp
        src/Controller/MusicController.cpp

        # Audio
        src/Audio/SDLAudioEngine.cpp
        src/Audio/MusicFileReader.cpp
        src/Audio/SongPlayer.cpp

        # Instruments
        src/Instruments/SimpleSynthInstrument.cpp
        src/Instruments/PianoInstrument.cpp
        src/Instruments/GuitarInstrument.cpp
        src/Instruments/XylophoneInstrument.cpp

        # Utils
        src/Utils/DropdownMenu.cpp
        src/Utils/TextHelper.cpp
)

target_include_directories(MusicaLau PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(MusicaLau PRIVATE SDL3_ttf::SDL3_ttf SDL3::SDL3)

option(MUSICALAU_DEBUG "Print debug traces to the console" OFF)
if (MUSICALAU_DEBUG)
    target_compile_definitions(MusicaLau PRIVATE MUSICALAU_DEBUG)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(MusicaLau PRIVATE -Wall -Wextra)
elseif (MSVC)
    target_compile_options(MusicaLau PRIVATE /W3 /utf-8)
    target_compile_definitions(MusicaLau PRIVATE _USE_MATH_DEFINES)
endif ()

# Copy fonts and example songs next to the executable
add_custom_command(TARGET MusicaLau POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:MusicaLau>/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/songs $<TARGET_FILE_DIR:MusicaLau>/songs)

# On Windows, copy the SDL DLLs next to the executable
if (WIN32)
    add_custom_command(TARGET MusicaLau POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:MusicaLau> $<TARGET_FILE_DIR:MusicaLau>
            COMMAND_EXPAND_LISTS)
endif ()
