#ifndef MAZERUNNER_HPP
#define MAZERUNNER_HPP

#include "RunnerSteps.hpp"

#include <memory>
#include <vector>
#include <chrono>

#include <cstdlib> /* For size_t */

/* Forward definitions of some classes */
namespace subprocess {
    class Popen;
}
class Labyrinth;


/* The Maze Runner Class which will execute the maze runners and collect their results */
class MazeRunner
{
public:

    explicit MazeRunner(const std::string &executable, std::shared_ptr<Labyrinth> labyrinth);
    virtual ~MazeRunner( void );


    RunnerSteps::StepList run();

    unsigned getPoints( void ) const { return points_;} /* Get the number of points */
    bool wasSuccessful( void ) const { return !failed_; } /* Runner was successfull */
    bool runerFailed( void ) const { return failed_; }

    RunnerSteps::Steps getStep(size_t const step) const { return steps_[step]; }
    size_t getNumberOfSteps( void ) const {return steps_.size(); }

private:
    int points_;

    size_t cx_;
    size_t cy_;
    std::chrono::steady_clock::time_point lastRead_;
    std::string const tmpFileName_;
    std::string const executable_;

    bool failed_;
    bool finished_;
    bool collectedTreshure_;
    bool isEof_;
    std::shared_ptr<subprocess::Popen> process_;
    std::shared_ptr<Labyrinth> lab_;

    RunnerSteps::StepList steps_;

    std::array<char, 1024> buffer_;
    ssize_t bytesInBuffer_;
    size_t bufferReadPointer_;

    void parseNextStep( const std::string& line);
    std::string readLine();
    void findStartPoint( void );
};

#endif // MAZERUNNER_HPP
