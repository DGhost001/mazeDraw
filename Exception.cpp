#include "Exception.hpp"

#include <SDL.h>
#include <SDL_image.h>

SDLError::SDLError() {}
SDLError::~SDLError() {}

char const * SDLError::what() const noexcept {
    return SDL_GetError();
}

ImgError::ImgError() {}
ImgError::~ImgError() {}
char const * ImgError::what() const noexcept
{
    return IMG_GetError();
}
