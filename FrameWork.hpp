#ifndef FRAMEWORK_HPP
#define FRAMEWORK_HPP

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

    SDL_Renderer *renderer_;
    SDL_Window *window_;

    void drawCircle();


};

#endif // FRAMEWORK_HPP
