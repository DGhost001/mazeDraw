#include "FrameWork.hpp"
#include "Exception.hpp"

#include <SDL.h>
#include <SDL_image.h>

FrameWork::FrameWork(int const width, int const height):
    height_(height),
    width_(width),
    renderer_(nullptr),
    window_(nullptr)
{

    SDL_CHECK_ERROR(SDL_Init(SDL_INIT_VIDEO));

    SDL_CHECK_ERROR(SDL_CreateWindowAndRenderer(width_,
                                                height_,
                                                0,
                                                &window_,
                                                &renderer_
                                                ));

    SDL_CHECK_ERROR(SDL_SetRenderDrawColor(renderer_, 0,0,0,255));
    SDL_CHECK_ERROR(SDL_RenderClear(renderer_));
    SDL_RenderPresent(renderer_);
}

FrameWork::~FrameWork()
{
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void FrameWork::drawCircle()
{
    SDL_CHECK_ERROR(SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255));

    for(unsigned i = 0; i<3600; ++i) {
        SDL_RenderDrawPointF(renderer_,
                             sinf((i /1800.0)*M_PI)*100 + 320,
                             cosf((i /1800.0)*M_PI)*100 + 200);

    }

    SDL_RenderPresent(renderer_);
}

void FrameWork::run( void )
{
    SDL_Event event;

    do {
        /* Clear the viewing area */
        SDL_SetRenderDrawColor(renderer_, 0,0,0,255);
        SDL_RenderClear(renderer_);

        drawCircle();
        SDL_Delay(10); /* Just for now ... */
        SDL_PollEvent(&event);
    }while(SDL_QUIT != event.type);
}
