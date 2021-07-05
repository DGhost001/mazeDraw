#ifndef LABYRINTH_HPP
#define LABYRINTH_HPP

#include <string>
#include <array>
#include <memory>
#include <cstddef>


/* Just a simplification for multidimensional arrays *
 * see http://cpptruths.blogspot.com/2011/10/multi-dimensional-arrays-in-c11.html
 **/
template <class T, size_t ROW, size_t COL>
using Matrix = std::array<std::array<T, COL>, ROW>;

struct SDL_Renderer;
class Image;


class Labyrinth
{
public:
    enum class MazeContent {
        Grass,
        Wall,
        Waterbottle,
        Start,
        Treashure,
        Count /* Keep this at the END! */
    };



    Labyrinth();

    void load(const std::string &path);
    void save(const std::string &path) const;

    /**
     * @brief render Renders the Labyrinth
     * @param renderer This is the renderer to render to
     * @param cx the center of the rendered part
     * @param cy the center of the rendered part
     * @param w the width of the render window in tiles
     * @param h the height of the render window in tiles
     */
    void render(std::shared_ptr<SDL_Renderer> renderer,
                size_t const cx,
                size_t const cy,
                size_t const w,
                size_t const h
                ) const;

    /* Sets a cell in the maze */
    void set(size_t const x, size_t const y, MazeContent const cell);
    MazeContent get( size_t const x, size_t const y) const;

private:
    Matrix<MazeContent, 1000, 1000> maze_;
    std::array< std::shared_ptr< Image >, static_cast<size_t>(MazeContent::Count)> textures_;

};

#endif // LABYRINTH_HPP
