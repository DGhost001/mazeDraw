cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 20)
project(MazeDraw)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
find_package(SDL2_gfx REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDLGFX_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/nanogui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/subprocess/src/cpp)

add_subdirectory(third_party)

set(SOURCE
    main.cpp
    FrameWork.cpp
    Exception.cpp
    Image.cpp
    Labyrinth.cpp
    RepeatDelay.cpp
    wallselector.cpp
    gui.cpp
    mazerunner.cpp
    RunnerSteps.cpp
)

set(HEADERS
    FrameWork.hpp
    Exception.hpp
    Image.hpp
    Labyrinth.hpp
    RepeatDelay.hpp
    wallselector.hpp
    gui.hpp
    mazerunner.hpp
    RunnerSteps.hpp
)

add_executable(mazeDraw
 ${SOURCE}
 ${HEADERS}
)

message(STATUS "FOUND SDL2 GFX Library: ${SDLGFX_LIBRARY}")

target_link_libraries(mazeDraw
   nanogui
   subprocess
   ${SDLGFX_LIBRARY}
   ${SDL2_LIBRARIES}
   ${SDL2IMAGE_LIBRARIES}
   m
)
