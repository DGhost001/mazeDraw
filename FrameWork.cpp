#include "FrameWork.hpp"
#include "Exception.hpp"
#include "Image.hpp"
#include "Labyrinth.hpp"
#include "RepeatDelay.hpp"
#include "wallselector.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include <sdlgui/screen.h>
#include <sdlgui/window.h>
#include <sdlgui/label.h>
#include <sdlgui/button.h>

static constexpr size_t const& cellSize = 32;

class Example : public sdlgui::Screen
{
public:
    explicit Example(std::shared_ptr<SDL_Window> win, size_t const width, size_t const height)
        : Screen(win.get(), sdlgui::Vector2i{width, height}, "GUI Test")
    {
        sdlgui::Window& win1 = dynamic_cast<sdlgui::Window&> (window("Sample Window", sdlgui::Vector2i{15,15}).withLayout<sdlgui::GroupLayout>());
        win1.label("Push buttons", "sans-bold")._and()
               .button("Plain button 1", [&win1] { win1.dispose(); std::cout << "pushed!" << std::endl; })
                  .withTooltip("This is plain button tips");
        performLayout(mSDL_Renderer);
    }

    virtual ~Example() {
        /* Nothing to see here ... go on please */
    }
};



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

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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

    example_ = std::make_shared<Example>(window_, width_, height_);

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

    example_->drawAll();

    SDL_RenderPresent(renderer_.get());
}

void  FrameWork::handleKeyboard( void )
{
    if((keyMap_[SDLK_UP] || keyMap_[SDLK_w]) && posy_ > 0)       upDelay_.   run([this]{--posy_;});
    if((keyMap_[SDLK_DOWN] || keyMap_[SDLK_s]) && posy_ < 1000)  downDelay_. run([this]{++posy_;});
    if((keyMap_[SDLK_LEFT] || keyMap_[SDLK_a]) && posx_ > 0)     leftDelay_. run([this]{--posx_;});
    if((keyMap_[SDLK_RIGHT] || keyMap_[SDLK_d]) && posx_ < 1000) rightDelay_.run([this]{++posx_;});
    if((keyMap_[SDLK_1])) wallSelector_->cellSelected(0);
    if((keyMap_[SDLK_2])) wallSelector_->cellSelected(1);
    if((keyMap_[SDLK_3])) wallSelector_->cellSelected(2);
    if((keyMap_[SDLK_4])) wallSelector_->cellSelected(3);
    if((keyMap_[SDLK_5])) wallSelector_->cellSelected(4);
    if((keyMap_[SDLK_6])) wallSelector_->cellSelected(5);
    if((keyMap_[SDLK_7])) wallSelector_->cellSelected(6);
    if((keyMap_[SDLK_8])) wallSelector_->cellSelected(7);
    if((keyMap_[SDLK_9])) wallSelector_->cellSelected(8);
    if((keyMap_[SDLK_0])) wallSelector_->cellSelected(9);
}

void FrameWork::handleMouseInput(const size_t cellX, const size_t cellY)
{
    const size_t topLeftX = posx_ - (width_ / cellSize / 2);
    const size_t topLeftY = posy_ - (height_ / cellSize / 2);

    if(cellY == (height_ / cellSize - 1) && mouseButtonMap_[SDL_BUTTON_LEFT])
    {
        wallSelector_->cellSelected(cellX);
    }else if (mouseButtonMap_[SDL_BUTTON_LEFT]) {

        labyrinth_->set(topLeftX + cellX, topLeftY + cellY, wallSelector_->getSelectedWall());
    }else if (mouseButtonMap_[SDL_BUTTON_RIGHT]) {
        labyrinth_->set(topLeftX + cellX, topLeftY + cellY, Labyrinth::MazeContent::Grass);
    }else if (mouseButtonMap_[SDL_BUTTON_MIDDLE]) {
        labyrinth_->set(topLeftX + cellX, topLeftY + cellY, Labyrinth::MazeContent::Waterbottle);
    }
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
        SDL_PollEvent(&event);

        if(!example_->onEvent(event)) {
            if(SDL_KEYDOWN == event.type) {
                keyMap_[event.key.keysym.sym] = true;
            } else if(SDL_KEYUP == event.type && !event.key.repeat) {
                keyMap_[event.key.keysym.sym] = false;
            } else if(SDL_MOUSEBUTTONDOWN == event.type) {
                mouseButtonMap_[event.button.button] = true;
                handleMouseInput(event.button.x / cellSize, event.button.y / cellSize);
            } else if(SDL_MOUSEBUTTONUP == event.type) {
                mouseButtonMap_[event.button.button] = false;
                handleMouseInput(event.button.x / cellSize, event.button.y / cellSize);
            } else if(SDL_MOUSEMOTION == event.type) {
                handleMouseInput(event.motion.x / cellSize, event.motion.y / cellSize);
            }
        }

    }while(SDL_QUIT != event.type && !keyMap_[SDLK_ESCAPE]);
}
