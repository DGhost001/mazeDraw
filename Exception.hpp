#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>

#define SDL_CHECK_ERROR(X) do{ if((X) < 0) throw SDLError();  }while(0)
#define IMG_CHECK_ERROR(X, F) do { if(0 == ((X) & F)) throw ImgError(); } while(0)


class SDLError: std::exception
{
public:
    explicit SDLError();
    virtual ~SDLError() override;
    virtual char const * what() const noexcept override;
};

class ImgError: std::exception
{
public:
    explicit ImgError();
    virtual ~ImgError() override;
    virtual char const * what() const noexcept override;
};

#endif // EXCEPTION_HPP
