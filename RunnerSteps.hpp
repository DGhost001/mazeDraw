#ifndef RUNNERSTEPS_HPP
#define RUNNERSTEPS_HPP

#include <cstdlib>
#include <vector>
#include <list>

class RunnerSteps
{
public:

    struct Steps {
        std::chrono::milliseconds timeStamp; /* When the maze runner reached this point */
        size_t x;                             /* The x coordinate in the maze */
        size_t y;                             /* The y coordinate in the maze */
        unsigned points;
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
    void drawSteps( size_t const cx, size_t const cy, const size_t width, const size_t height) const;

    /** This function sets the step vector **/
    void setSteps(const StepList &steps) { steps_ = steps; }

    /** This function returns a const reference of the current step vector **/
    const StepList& getSteps( void ) const { return steps_; }


private:
    StepList steps_;


    /** Compute the sections that are currently visible in the window **/
    std::list<std::pair< StepList::const_iterator, StepList::const_iterator > computeDrawSegments(size_t const x1,
                                                                                                  size_t const y1,
                                                                                                  size_t const x2,
                                                                                                  size_t const y2 );
};

#endif // RUNNERSTEPS_HPP
