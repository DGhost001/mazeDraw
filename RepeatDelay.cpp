#include "RepeatDelay.hpp"

RepeatDelay::RepeatDelay(std::chrono::milliseconds const &timeSpawn):
    timeSpawn_(timeSpawn),
    latestExcecution_(std::chrono::steady_clock::now() - timeSpawn)
{    
}

void RepeatDelay::run(std::function<void(void)> func)
{

    std::chrono::time_point now = std::chrono::steady_clock::now();

    if((latestExcecution_ + timeSpawn_) < now)
    {
        latestExcecution_ = now;
        func();
    }
}
