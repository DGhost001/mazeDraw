#include "FrameWork.hpp"
#include "Exception.hpp"
#include "Image.hpp"
#include "Labyrinth.hpp"
#include "RepeatDelay.hpp"
#include "wallselector.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

static constexpr size_t const& cellSize = 32;


FrameWork::FrameWork(int const width, int const height):
    height_(height),
    width_(width),
    renderer_(nullptr),
    window_(nullptr),
    upDelay_(std::chrono::milliseconds(100)),
    downDelay_(std::chrono::milliseconds(100)),
    leftDelay_(std::chrono::milliseconds(100)),
    rightDelay_(std::chrono::milliseconds(100)),
    stepFunction_(std::chrono::milliseconds(100))
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

    wallSelector_ = std::make_shared<WallSelector>(renderer_);

    posx_ = width_ / (2*cellSize);
    posy_ = height_ / (2*cellSize);
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
    for(int x = 0; x < width_ / cellSize; ++x )
        for(int y = 0; y < height_ / cellSize; ++y)
        {
            image_->render(renderer_, x*cellSize, y*cellSize);
        }

    labyrinth_->render(renderer_, posx_, posy_, width_/cellSize, (height_ / cellSize)-1);
    wallSelector_->render(renderer_, 0, height_ - cellSize, width_);

    SDL_RenderPresent(renderer_.get());
}

void  FrameWork::handleKeyboard( void )
{
    if((keyMap_[SDLK_UP] || keyMap_[SDLK_w]) && posy_ > 0)       upDelay_.   run([this]{--posy_;});
    if((keyMap_[SDLK_DOWN] || keyMap_[SDLK_s]) && posy_ < 1000)  downDelay_. run([this]{++posy_;});
    if((keyMap_[SDLK_LEFT] || keyMap_[SDLK_a]) && posx_ > 0)     leftDelay_. run([this]{--posx_;});
    if((keyMap_[SDLK_RIGHT] || keyMap_[SDLK_d]) && posx_ < 1000) rightDelay_.run([this]{++posx_;});
}

void FrameWork::handleMouseInput(const size_t cellX, const size_t cellY)
{

}

void FrameWork::step( void )
{
}

void FrameWork::run( void )
{
    SDL_Event event;


    do {
        /* Clear the viewing area */
        SDL_SetRenderDrawColor(renderer_.get(), 0,0,0,255);
        SDL_RenderClear(renderer_.get());


        stepFunction_.run([this]{step();});

        handleKeyboard();
        drawLabyrinth();
        SDL_Delay(10); /* Just for now ... */
        SDL_PollEvent(&event);

        if(SDL_KEYDOWN == event.type) {
            keyMap_[event.key.keysym.sym] = true;
        } else if(SDL_KEYUP == event.type && !event.key.repeat) {
            keyMap_[event.key.keysym.sym] = false;
        } else if(SDL_MOUSEBUTTONDOWN == event.type) {
            mouseButtonMap_[event.button.button] = true;
        } else if(SDL_MOUSEBUTTONUP == event.type) {
            mouseButtonMap_[event.button.button] = false;
        } else if(SDL_MOUSEMOTION == event.type) {
            handleMouseInput(event.motion.x / cellSize, event.motion.y / cellSize);
        }

    }while(SDL_QUIT != event.type && !keyMap_[SDLK_ESCAPE]);
}
