#include "Exception.hpp"

#include <SDL.h>

SDLError::SDLError() {}
SDLError::~SDLError() {}

char const * SDLError::what() const noexcept {
    return SDL_GetError();
}
