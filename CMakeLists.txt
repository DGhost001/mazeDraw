cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)
project(MazeDraw)

include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})

set(SOURCE
    main.cpp
)

set(HEADERS
)

add_executable(mazeDraw
 ${SOURCE}
 ${HEADERS}
)

target_link_libraries(mazeDraw
   ${SDL2_LIBRARIES}  
   m
)
