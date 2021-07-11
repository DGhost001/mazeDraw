#include "FrameWork.hpp"
#include "Exception.hpp"
#include "Image.hpp"
#include "Labyrinth.hpp"

#include <SDL.h>
#include <SDL_image.h>

FrameWork::FrameWork(int const width, int const height):
    height_(height),
    width_(width),
    renderer_(nullptr),
    window_(nullptr)
{

    SDL_Renderer *rawRenderer = nullptr;
    SDL_Window *rawWindow = nullptr;

    /* Initialize SDL and open a window */
    SDL_CHECK_ERROR(SDL_Init(SDL_INIT_VIDEO));
    SDL_CHECK_ERROR(SDL_CreateWindowAndRenderer(width_,
                                                height_,
                                                0,
                                                &rawWindow,
                                                &rawRenderer
                                                ));

    renderer_ = std::shared_ptr<SDL_Renderer>(rawRenderer, SDL_DestroyRenderer);
    window_ = std::shared_ptr<SDL_Window>(rawWindow, SDL_DestroyWindow);

    /* Load the PNG Support */
    IMG_CHECK_ERROR(IMG_Init(IMG_INIT_PNG), IMG_INIT_PNG);


    /* Initialize the Window with some black color below */
    SDL_CHECK_ERROR(SDL_SetRenderDrawColor(renderer_.get(), 0,0,0,255));
    SDL_CHECK_ERROR(SDL_RenderClear(renderer_.get()));
    SDL_RenderPresent(renderer_.get());


    /* Load the background image */

    image_ = std::make_shared<Image>("assets/pebble.png", renderer_);
    labyrinth_ = std::make_shared<Labyrinth>(renderer_);
    labyrinth_->load("maze/sample.maze");

    posx_ = width_ / 64;
    posy_ = height_ / 64;
}

FrameWork::~FrameWork()
{
    /* Teardown SDL and Image library */
    renderer_.reset();
    window_.reset();
    IMG_Quit();
    SDL_Quit();
}

void FrameWork::drawLabyrinth( void ) const
{
    for(int x = 0; x < width_ / 32; ++x )
        for(int y = 0; y < height_ / 32; ++y)
        {
            image_->render(renderer_, x*32, y*32);
        }

    labyrinth_->render(renderer_, posx_, posy_, width_/32, (height_ / 32)-1);


    SDL_RenderPresent(renderer_.get());
}

void FrameWork::run( void )
{
    SDL_Event event;

    do {
        /* Clear the viewing area */
        SDL_SetRenderDrawColor(renderer_.get(), 0,0,0,255);
        SDL_RenderClear(renderer_.get());

        drawLabyrinth();
        SDL_Delay(10); /* Just for now ... */
        SDL_PollEvent(&event);
    }while(SDL_QUIT != event.type);
}
