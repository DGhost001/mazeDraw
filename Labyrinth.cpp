#include "Labyrinth.hpp"
#include "Image.hpp"
#include "Exception.hpp"

#include <SDL.h>
#include <iostream>
#include <fstream>

size_t Labyrinth::indexOf(MazeContent const content) const noexcept
{
    return static_cast<size_t>(content);
}

Labyrinth::Labyrinth(std::shared_ptr<SDL_Renderer> renderer)
{
    /* Clear the maze with Grass on initialization of the Labyrith */
    for(auto& it: maze_) {
        for(auto &it2: it) {
            it2 = MazeContent::Grass;
        }
    }

    /* Load all required texture for the rendering on construction of the Labyrinth */
    textures_[indexOf(MazeContent::Grass)] = std::make_shared<Image>("assets/grass.png", renderer);
    textures_[indexOf(MazeContent::Wall)]  = std::make_shared<Image>("assets/wall.pnh", renderer);
    textures_[indexOf(MazeContent::Waterbottle)] = std::make_shared<Image>("assets/bottle.png", renderer);
    textures_[indexOf(MazeContent::Start)] = std::make_shared<Image>("assets/start.png", renderer);
    textures_[indexOf(MazeContent::Treashure)] = std::make_shared<Image>("assets/treshure.png", renderer);
}

void Labyrinth::load(const std::string &path)
{
    /* Clear the maze with Grass before loading anything*/
    for(auto& it: maze_) {
        for(auto &it2: it) {
            it2 = MazeContent::Grass;
        }
    }

    std::ifstream inFile(path, std::ios::in);
    size_t y = 0;
    size_t x = 0;

    while(!inFile.eof() && y < maze_.size()) {
        char c;
        inFile >> c;

        switch (c) {
        case '+':
        case '|':
        case '-': maze_[y][x] = MazeContent::Wall; break;
        case 'W': maze_[y][x] = MazeContent::Waterbottle; break;
        case 'S': maze_[y][x] = MazeContent::Start; break;
        case 'T': maze_[y][x] = MazeContent::Treashure; break;
        case '\n': ++y; x = 0; break;
        default: break;
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
            case MazeContent::Treashure: {
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

}

void Labyrinth::set(size_t const x, size_t const y, MazeContent const cell)
{
    maze_[y][x] = cell;
}

Labyrinth::MazeContent Labyrinth::get( size_t const x, size_t const y) const
{
    return maze_[y][x];
}
