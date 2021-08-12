#ifndef RUNNERSTEPS_HPP
#define RUNNERSTEPS_HPP

#include <cstdlib>
#include <vector>
#include <list>
#include <memory>
#include <chrono>

struct SDL_Renderer;

class RunnerSteps
{
public:

    struct Steps {
        std::chrono::milliseconds timeStamp; /* When the maze runner reached this point */
        size_t x;                             /* The x coordinate in the maze */
        size_t y;                             /* The y coordinate in the maze */
        int points;
    };

    typedef std::vector<Steps> StepList;

    explicit RunnerSteps( void );
    virtual ~RunnerSteps ( void );


    /**
     * @brief drawSteps Draw the steps currently in the steps list
     * @param cx this is the x center of the window to draw in
     * @param cy  this is the y center of the window to draw in
     * @param width This is the window width in segments
     * @param height This is the window height in segments
     */
    void drawSteps(std::shared_ptr<SDL_Renderer> renderer, size_t const cx, size_t const cy, const size_t width, const size_t height) const;

    /** This function sets the step vector **/
    void setSteps(const StepList &steps) { steps_ = steps; }

    /** This function returns a const reference of the current step vector **/
    const StepList& getSteps( void ) const { return steps_; }


private:
    typedef std::list<std::pair< RunnerSteps::StepList::const_iterator, RunnerSteps::StepList::const_iterator > > SegmentList;

    StepList steps_;


    /** Compute the sections that are currently visible in the window **/
    SegmentList computeDrawSegments(size_t const x1, size_t const y1, size_t const x2, size_t const y2 ) const;
};

#endif // RUNNERSTEPS_HPP
