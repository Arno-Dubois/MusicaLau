cmake_minimum_required(VERSION 3.20)
project(MusicaLau)

set(CMAKE_CXX_STANDARD 17)

# Find SDL3
find_package(SDL3 REQUIRED)

if (SDL3_FOUND)
    include_directories(${SDL3_INCLUDE_DIRS})

    add_executable(MusicaLau src/main.cpp)

    # Link against SDL3
    target_link_libraries(MusicaLau ${SDL3_LIBRARIES})
else ()
    message(FATAL_ERROR "SDL3 not found. Please set CMAKE_PREFIX_PATH or SDL3_DIR.")
endif ()
