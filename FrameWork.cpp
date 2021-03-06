#include "FrameWork.hpp"
#include "Exception.hpp"
#include "Image.hpp"
#include "Labyrinth.hpp"
#include "RepeatDelay.hpp"
#include "wallselector.hpp"
#include "gui.hpp"
#include "mazerunner.hpp"
#include "RunnerSteps.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include <sdlgui/screen.h>
#include <sdlgui/messagedialog.h>

static constexpr size_t const& cellSize = 32;

FrameWork::FrameWork(int const width, int const height):
    height_(height),
    width_(width),
    quit_(false),
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

    gui_ = std::make_shared<Gui>(window_, width_, height_,
                                 [this](const std::string &filename)
    {
        steps_.clear();
        runnerSteps_->setSteps(steps_);
        labyrinth_->load(filename);
    },
    [this](const std::string &filename) { labyrinth_->save(filename);},
    [this]{
        steps_.clear();
        runnerSteps_->setSteps(steps_);
        labyrinth_->clear();
    },
    [this]{quit_ = true;},
    wallSelector_,
    [this](const std::string &filename){executeRunner(filename);}
    );

    runnerSteps_ = std::make_shared<RunnerSteps>();

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
    runnerSteps_->drawSteps(renderer_,posx_, posy_,width_/cellSize, (height_ / cellSize)-1);

    gui_->getScreen()->drawAll();

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
        labyrinth_->set(topLeftX + cellX, topLeftY + cellY, wallSelector_->getBgSelectedWall());
    }else if (mouseButtonMap_[SDL_BUTTON_MIDDLE]) {
        labyrinth_->set(topLeftX + cellX, topLeftY + cellY, Labyrinth::MazeContent::Waterbottle);
    }
}

void FrameWork::step( void )
{
}

void FrameWork::executeRunner( const std::string& executable)
{
    try {
        MazeRunner mrun(executable, labyrinth_);
        steps_ = mrun.run();

        char buffer[33];

        std::string result =  "Points: " + std::string(SDL_itoa(steps_[steps_.size() - 1].points, buffer, 10)) + "\n";
                    result += "Steps: " + std::string(SDL_itoa(steps_.size(), buffer, 10)) + "\n";
                    result += "Time: " + std::string(SDL_itoa(steps_[steps_.size() - 1].timeStamp.count(), buffer, 10)) + "\n";

                    if(mrun.wasSuccessful()) {
                        result = "Runner finished succesfully!\n" + result;
                    } else {
                        result = "Runner finished failed!\n" + result;
                    }

        gui_->getScreen()->msgdialog(sdlgui::MessageDialog::Type::Information,
                              "Runner Finished",
                              result.c_str());

        runnerSteps_->setSteps(steps_);


    }catch(std::exception const &e)
    {
        gui_->getScreen()->msgdialog(sdlgui::MessageDialog::Type::Information,
                              "Error during runner execution",
                              e.what());
    }catch(...)
    {
        gui_->getScreen()->msgdialog(sdlgui::MessageDialog::Type::Information,
                              "Error during runner execution",
                              "Execution failure");
    }
}


void FrameWork::run( void )
{
    SDL_Event event;


    do {
        SDL_Delay(10);
        /* Clear the viewing area */
        SDL_SetRenderDrawColor(renderer_.get(), 0,0,0,255);
        SDL_RenderClear(renderer_.get());


        stepFunction_.run([this]{step();});

        handleKeyboard();
        drawLabyrinth();

        while(SDL_PollEvent(&event) && SDL_QUIT != event.type)
        {
            if(!gui_->getScreen()->onEvent(event)) {
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
        }

    }while(SDL_QUIT != event.type && !keyMap_[SDLK_ESCAPE] && !quit_);
}
