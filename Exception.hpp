#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>

class SDLError: std::exception
{
public:
    explicit SDLError();
    virtual ~SDLError() override;
    virtual char const * what() const noexcept override;
};


#define SDL_CHECK_ERROR(X) do{ if((X) < 0) throw SDLError();  }while(0)


#endif // EXCEPTION_HPP
