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
    explicit Image(const std::string &path, std::shared_ptr<SDL_Renderer> renderer);
    virtual ~Image();
    void render(std::shared_ptr<SDL_Renderer> renderer,
                int const x, int const y, int const w = -1, int const h = -1);
private:
    std::shared_ptr<SDL_Texture> imageTexture_;

};

#endif // IMAGE_HPP
