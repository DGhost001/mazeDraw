#ifndef WALLSELECTOR_HPP
#define WALLSELECTOR_HPP

#include "Labyrinth.hpp"
#include "cstdint"

struct SDL_Renderer;
class Image;

class WallSelector
{
public:
    explicit WallSelector(std::shared_ptr<SDL_Renderer> renderer);
    /**
     * @brief render Renders the Labyrinth
     * @param renderer This is the renderer to render to
     * @param cx the center of the rendered part
     * @param cy the center of the rendered part
     * @param w the width of the render window in tiles
     */
    void render(std::shared_ptr<SDL_Renderer> renderer,
                size_t const sx,
                size_t const sy,
                size_t const w
                ) const;

    void step( void );


    Labyrinth::MazeContent getSelectedWall( void ) const;
    Labyrinth::MazeContent getBgSelectedWall( void ) const;

    size_t getCellSelected( void ) const { return selectedCell_; }
    size_t getBgSelectedCell( void ) const { return bgSelectedCell_; }


    void cellSelected( size_t cell );
    void bgCellSelected( size_t cell );

private:
    size_t selectedCell_;
    size_t bgSelectedCell_;
    std::array< std::shared_ptr< Image >, static_cast<size_t>(Labyrinth::MazeContent::Count)> textures_;


};

#endif // WALLSELECTOR_HPP
