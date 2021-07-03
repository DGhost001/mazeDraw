#include "Image.hpp"
#include "Exception.hpp"

#include <SDL.h>
#include <SDL_image.h>

Image::Image(const std::string &path, std::shared_ptr<SDL_Renderer> renderer)
{
    /* Load the image */
    std::shared_ptr<SDL_Surface> loadedSurface(IMG_Load(path.c_str()), SDL_FreeSurface);

    /* In case the loading failed ... */
    if(!loadedSurface) {
        throw ImgError();
    }

    imageTexture_ = std::shared_ptr<SDL_Texture>(
                        SDL_CreateTextureFromSurface(renderer.get(), loadedSurface.get()),
                        SDL_DestroyTexture);
    if(!imageTexture_)
    {
        throw  SDLError();
    }
}

Image::~Image()
{
}

void Image::render(std::shared_ptr<SDL_Renderer> renderer,
                   int const x, int const y, int const w, int const h)
{
    SDL_Rect rect {.x = x, .y = y, .w=w, .h=h };

    /* In case no scaling is given, use set it to 1:1 */
    if(w < 0 || h < 0)
    {
        SDL_QueryTexture(imageTexture_.get(),nullptr, nullptr, &rect.w, &rect.h);
    }

    SDL_RenderCopy(renderer.get(), imageTexture_.get(), nullptr, &rect);
}
