#ifndef FRAMEWORK_HPP
#define FRAMEWORK_HPP

#include <memory>

#include <cstdlib>

struct SDL_Renderer;
struct SDL_Window;
class Image;
class Labyrinth;

class FrameWork
{
public:
    FrameWork( int const width, int const height);
    ~FrameWork();

    void run( void );

private:
    int height_;
    int width_;

    size_t posx_; /* This is the x center position of the lab */
    size_t posy_; /* This is the y center position of the lab */

    std::shared_ptr<SDL_Renderer> renderer_; /* This is the renderer */
    std::shared_ptr<SDL_Window> window_; /* The SDL Window */
    std::shared_ptr<Image> image_; /* The default clearing background */
    std::shared_ptr<Labyrinth> labyrinth_; /* The labyrinth to draw */

    void drawLabyrinth( void ) const;
};

#endif // FRAMEWORK_HPP
