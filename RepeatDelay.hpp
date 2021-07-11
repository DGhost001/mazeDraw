#ifndef REPEATDELAY_HPP
#define REPEATDELAY_HPP

#include <chrono>
#include <functional>

class RepeatDelay
{
public:
    explicit RepeatDelay(std::chrono::milliseconds const &timeSpawn);

    void run(std::function<void(void)> func);

private:
    std::chrono::milliseconds timeSpawn_;
    std::chrono::steady_clock::time_point latestExcecution_;
};

#endif // REPEATDELAY_HPP
