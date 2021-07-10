#include "Labyrinth.hpp"
#include "Image.hpp"

#include <SDL.h>

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

}

void Labyrinth::save(const std::string &path) const
{

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

}

Labyrinth::MazeContent Labyrinth::get( size_t const x, size_t const y) const
{

}
