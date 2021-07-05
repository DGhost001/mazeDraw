cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 20)
project(MazeDraw)

include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS})

set(SOURCE
    main.cpp
    FrameWork.cpp
    Exception.cpp
    Image.cpp
    Labyrinth.cpp
)

set(HEADERS
    FrameWork.hpp
    Exception.hpp
    Image.hpp
    Labyrinth.hpp
)

add_executable(mazeDraw
 ${SOURCE}
 ${HEADERS}
)

target_link_libraries(mazeDraw
   ${SDL2_LIBRARIES}
   ${SDL2IMAGE_LIBRARIES}
   m
)
