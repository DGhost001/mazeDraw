#ifndef FRAMEWORK_HPP
#define FRAMEWORK_HPP

#include <memory>

struct SDL_Renderer;
struct SDL_Window;

class FrameWork
{
public:
    FrameWork( int const width, int const height);
    ~FrameWork();

    void run( void );

private:
    int height_;
    int width_;

    std::shared_ptr<SDL_Renderer> renderer_;
    std::shared_ptr<SDL_Window> window_;

    void drawCircle();


};

#endif // FRAMEWORK_HPP
