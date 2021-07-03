#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <memory>

/* Forward declaration of the SDL_Surface struct, no one needs that here */
struct SDL_Texture;
struct SDL_Renderer;

class Image
{
public:
    explicit Image(const std::string &path, std::shared_ptr<SDL_Renderer> render);
    virtual ~Image();
private:
    std::shared_ptr<SDL_Texture> imageTexture_;

};

#endif // IMAGE_HPP
