#ifndef FRAMEWORK_HPP
#define FRAMEWORK_HPP

#include "RepeatDelay.hpp"

#include <memory>
#include <map>

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

    std::map<int, bool> keyMap_;

    std::shared_ptr<SDL_Renderer> renderer_; /* This is the renderer */
    std::shared_ptr<SDL_Window> window_; /* The SDL Window */
    std::shared_ptr<Image> image_; /* The default clearing background */
    std::shared_ptr<Labyrinth> labyrinth_; /* The labyrinth to draw */

    RepeatDelay upDelay_;
    RepeatDelay downDelay_;
    RepeatDelay leftDelay_;
    RepeatDelay rightDelay_;

    void drawLabyrinth( void ) const;
    void handleKeyboard( void );
};

#endif // FRAMEWORK_HPP
