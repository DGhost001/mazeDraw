#include <iostream>

#include "FrameWork.hpp"
#include "Exception.hpp"

int main( void )
{
    try
    {
        FrameWork frameWork(640, 480);

        frameWork.run();
    }catch(const std::exception &e)
    {
        std::cerr << "Cought unhandled Exception: '" << e.what() << "'" << std::endl;
        return -1;
    }catch(const ImgError &e)
    {
        std::cerr << "Cought unhandled Exception: '" << e.what() << "'" << std::endl;
        return -2;
    }

    return 0;
}
