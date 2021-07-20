#include "Labyrinth.hpp"
#include "Image.hpp"
#include "Exception.hpp"

#include <SDL.h>
#include <iostream>
#include <fstream>

size_t Labyrinth::indexOf(MazeContent const content) noexcept
{
    return static_cast<size_t>(content);
}

Labyrinth::Labyrinth(std::shared_ptr<SDL_Renderer> renderer)
{
    /* Clear the maze with Grass on initialization of the Labyrith */
    clear();

    /* Load all required texture for the rendering on construction of the Labyrinth */
    textures_[indexOf(MazeContent::Grass)] = std::make_shared<Image>("assets/grass.png", renderer);
    textures_[indexOf(MazeContent::Wall)]  = std::make_shared<Image>("assets/brick.png", renderer);
    textures_[indexOf(MazeContent::Waterbottle)] = std::make_shared<Image>("assets/bottle.png", renderer);
    textures_[indexOf(MazeContent::Start)] = std::make_shared<Image>("assets/start.png", renderer);
    textures_[indexOf(MazeContent::Treasure)] = std::make_shared<Image>("assets/treasure.png", renderer);
}

void Labyrinth::clear( void )
{
    /* Clear the maze with Grass before loading anything*/
    for(auto& it: maze_) {
        for(auto &it2: it) {
            it2 = MazeContent::Grass;
        }
    }

}

void Labyrinth::load(const std::string &path)
{
    clear();

    std::ifstream inFile(path, std::ios::in | std::ios::binary);
    size_t y = 0;
    size_t x = 0;

    while(!inFile.eof() && y < maze_.size()) {
        char const c = inFile.get();

        switch (c) {
        case '+':
        case '|':
        case '-': maze_[y][x] = MazeContent::Wall; ++x; break;
        case 'W': maze_[y][x] = MazeContent::Waterbottle; ++x; break;
        case 'S': maze_[y][x] = MazeContent::Start; ++x; break;
        case 'T': maze_[y][x] = MazeContent::Treasure; ++x; break;
        case '\n': ++y; x = 0; break;
        default: ++x; break;
        }

        if(y < maze_.size()) {
            if (x > maze_[y].size()) {
                throw std::overflow_error("Invalid Maze Size");
            }
        }
    }

    inFile.close();
}

void Labyrinth::save(const std::string &path) const
{

    std::ofstream saveFile(path, std::ios::out);

    for(size_t y=0; y < maze_.size(); ++y) {
        size_t grassRunLength = 0;
        for(size_t x=0; x < maze_[y].size(); ++x) {
            switch (maze_[y][x]) {
            case MazeContent::Grass: {
                ++grassRunLength;
            }break;
            case MazeContent::Wall: {
                for(;grassRunLength > 0; --grassRunLength) saveFile<< " ";

                bool const wallAbove = y > 0 ? MazeContent::Wall == maze_[y-1][x] : false;
                bool const wallBelow = y < maze_.size()-1 ? MazeContent::Wall == maze_[y+1][x] : false;
                bool const wallLeft  = x > 0 ? MazeContent::Wall == maze_[y][x-1]: false;
                bool const wallRight = x < maze_[y].size()-1 ? MazeContent::Wall == maze_[y][x+1]: false;

                if ((wallLeft || wallRight) && !(wallAbove || wallBelow)) {
                    saveFile << "-";
                } else if ((wallAbove || wallBelow) && !(wallLeft || wallRight)) {
                    saveFile << "|";
                } else {
                    saveFile << "+";
                }
            } break;
            case MazeContent::Start: {
                for(;grassRunLength > 0; --grassRunLength) saveFile<< " ";
                saveFile << "S";
            } break;
            case MazeContent::Treasure: {
                for(;grassRunLength > 0; --grassRunLength) saveFile<< " ";
                saveFile << "T";
            } break;
            case MazeContent::Waterbottle: {
                for(;grassRunLength > 0; --grassRunLength) saveFile<< " ";
                saveFile << "W";
            } break;
            default: {
                ++grassRunLength;
            }
            }
        }
        /* intentionally do not save white spaces that only fill up to the end */
        saveFile << std::endl;
    }
    saveFile.close();
}

void Labyrinth::render(std::shared_ptr<SDL_Renderer> renderer,
            size_t const cx,
            size_t const cy,
            size_t const w,
            size_t const h
            ) const
{

    size_t const h2 = h / 2;
    size_t const w2 = w / 2;

    for(int y = 0; y < h; ++y) {

        ssize_t const my = y - h2 + cy;

        for(int x = 0; x < w; ++x) {

           ssize_t const mx = x - w2 + cx;

           if( mx  >= 0 && my >= 0 && my < maze_.size() && mx < maze_[my].size()) {
               textures_[indexOf(maze_[my][mx])]->render(
                           renderer,
                           x * 32,
                           y * 32);
           }
       }
    }
}

void Labyrinth::set(size_t const x, size_t const y, MazeContent const cell)
{
    maze_[y][x] = cell;
}

Labyrinth::MazeContent Labyrinth::get( size_t const x, size_t const y) const
{
    return maze_[y][x];
}
