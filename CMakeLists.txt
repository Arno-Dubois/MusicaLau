cmake_minimum_required(VERSION 3.20) # Version 3.20 ou supérieure recommandée
project(MusicaLau)

# Spécifier le fichier toolchain de vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Ajouter le chemin de vcpkg pour trouver SDL3
set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows")

include_directories("C:/vcpkg/installed/x64-windows/include")
set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows")

# Trouver SDL3
find_package(SDL3 CONFIG REQUIRED)

# Définir l'exécutable AVANT de lier les bibliothèques
add_executable(MusicaLau main.cpp) # Ajoute tous tes fichiers .cpp

# Lier SDL3 à l'exécutable
target_link_libraries(MusicaLau PRIVATE SDL3::SDL3)

# Définir la version de C++
set(CMAKE_CXX_STANDARD 20) # C++20 est un bon choix
