#ifndef FRAMEWORK_HPP
#define FRAMEWORK_HPP

#include "RepeatDelay.hpp"

#include <memory>
#include <map>

#include <cstdlib>

/* Bunch of forward declarations, so we don't need to include the headers */
/* This saves us a fair ammount of compile time :) */
struct SDL_Renderer;
struct SDL_Window;
class Image;
class Labyrinth;
class WallSelector;
class Example;

/* The framework class definition */
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
    std::map<int, bool> mouseButtonMap_;

    std::shared_ptr<SDL_Renderer> renderer_; /* This is the renderer */
    std::shared_ptr<SDL_Window> window_; /* The SDL Window */
    std::shared_ptr<Image> image_; /* The default clearing background */
    std::shared_ptr<Labyrinth> labyrinth_; /* The labyrinth to draw */
    std::shared_ptr<WallSelector> wallSelector_; /* This is the wall selector at the bottom of the screen */
    std::shared_ptr<Example> example_; /* This is just an example GUI window */

    RepeatDelay upDelay_;
    RepeatDelay downDelay_;
    RepeatDelay leftDelay_;
    RepeatDelay rightDelay_;
    RepeatDelay stepFunction_;

    void drawLabyrinth( void ) const;
    void handleKeyboard( void );
    void handleMouseInput( size_t const cellX, size_t const cellY);
    void step( void );
};

#endif // FRAMEWORK_HPP
