#include "wallselector.hpp"
#include "Image.hpp"

#include <limits>
#include <SDL.h>


WallSelector::WallSelector(std::shared_ptr<SDL_Renderer> renderer):
    upCount_(true),
    green_(0),
    selectedCell_(0)
{
    textures_[Labyrinth::indexOf(Labyrinth::MazeContent::Grass)] = std::make_shared<Image>("assets/grass.png", renderer);
    textures_[Labyrinth::indexOf(Labyrinth::MazeContent::Wall)]  = std::make_shared<Image>("assets/brick.png", renderer);
    textures_[Labyrinth::indexOf(Labyrinth::MazeContent::Waterbottle)] = std::make_shared<Image>("assets/bottle.png", renderer);
    textures_[Labyrinth::indexOf(Labyrinth::MazeContent::Start)] = std::make_shared<Image>("assets/start.png", renderer);
    textures_[Labyrinth::indexOf(Labyrinth::MazeContent::Treasure)] = std::make_shared<Image>("assets/treasure.png", renderer);
}

void WallSelector::render(std::shared_ptr<SDL_Renderer> renderer,
                size_t const sx,
                size_t const sy,
                size_t const w
                ) const
{

    const size_t maxElements = Labyrinth::indexOf(Labyrinth::MazeContent::Count);

    for(size_t i = 0; i< maxElements; ++i) {
        textures_[i]->render(
                    renderer,
                    sx + i * 32,
                    sy);
    }

    SDL_SetRenderDrawColor(renderer.get(),0,green_,0, SDL_ALPHA_OPAQUE);
    SDL_Rect rect = {.x = static_cast<int>((sx + selectedCell_ *32)), .y = static_cast<int>(sy), .w = 32, .h = 32 };
    SDL_RenderDrawRect(renderer.get(), &rect);
}

void WallSelector::step( void )
{
    if(green_ < std::numeric_limits<typeof(green_)>::max() && upCount_ ) {
        ++green_;
    } else if(green_ > 0) {
        --green_;
    } else {
        upCount_ = !upCount_;
    }
}

Labyrinth::MazeContent WallSelector::getSelectedWall( void ) const
{
    return static_cast<Labyrinth::MazeContent>(selectedCell_);
}

void WallSelector::cellSelected( size_t const cell )
{
    if(cell < Labyrinth::indexOf(Labyrinth::MazeContent::Count))
    {
        selectedCell_ = cell;
    }
}
