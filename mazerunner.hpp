#ifndef MAZERUNNER_HPP
#define MAZERUNNER_HPP

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

    struct RunnerStep {
        std::chrono::milliseconds timeStamp; /* When the maze runner reached this point */
        size_t x;                             /* The x coordinate in the maze */
        size_t y;                             /* The y coordinate in the maze */
        unsigned points;
    };

    explicit MazeRunner(const std::string &executable);
    virtual ~MazeRunner( void );

    unsigned getPoints( void ) const { return points_;} /* Get the number of points */
    bool wasSuccessful( void ) const { return !failed_; } /* Runner was successfull */
    bool runerFailed( void ) const { return failed_; }

    RunnerStep getStep(size_t const step) const { return steps_[step]; }
    size_t getNumberOfSteps( void ) const {return steps_.size(); }

private:
    int points_;

    size_t cx_;
    size_t cy_;
    std::chrono::time_point lastRead_;

    bool failed_;
    bool finished_;
    bool collectedTreshure_;
    bool isEof_;
    std::shared_ptr<subprocess::Popen> process_;
    std::shared_ptr<Labyrinth> lab_;

    std::vector<RunnerStep> steps_;

    std::array<char, 1024> buffer_;
    ssize_t bytesInBuffer_;
    size_t bufferReadPointer_;

    void parseNextStep( const std::string& line);
    std::string readLine();
};

#endif // MAZERUNNER_HPP
