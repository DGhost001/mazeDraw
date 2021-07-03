#include "Image.hpp"
#include "Exception.hpp"

#include <SDL.h>
#include <SDL_image.h>

Image::Image(const std::string &path, std::shared_ptr<SDL_Renderer> render)
{
    /* Load the image */
    std::shared_ptr<SDL_Surface> loadedSurface(IMG_Load(path.c_str()), SDL_FreeSurface);

    /* In case the loading failed ... */
    if(!loadedSurface) {
        throw ImgError();
    }

    imageTexture_ = std::shared_ptr<SDL_Texture>(
                        SDL_CreateTextureFromSurface(render.get(), loadedSurface.get()),
                        SDL_DestroyTexture);
    if(!imageTexture_)
    {
        throw  SDLError();
    }
}

Image::~Image()
{
}
