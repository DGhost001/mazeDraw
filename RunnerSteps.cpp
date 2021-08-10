#include "RunnerSteps.hpp"

RunnerSteps::RunnerSteps( void )
{

}

RunnerSteps::~RunnerSteps ( void )
{

}

void RunnerSteps::drawSteps( std::shared_ptr<SDL_Renderer> renderer,
                             size_t const cx, size_t const cy, const size_t width, const size_t height) const
{
    size_t const x1 = cx > width / 2 ? cx - width / 2 : 0;
    size_t const x2 = cx + width / 2;
    size_t const y1 = cy > height / 2 ? cy - height / 2 : 0;
    size_t const y2 = cy + height / 2;

    SegmentList const segments(computeDrawSegments(x1,y1,x2,y2));


}

RunnerSteps::SegmentList RunnerSteps::computeDrawSegments(size_t const x1,
                                 size_t const y1,
                                 size_t const x2,
                                 size_t const y2 ) const
{

    SegmentList result;
    RunnerSteps::StepList::const_iterator begin = steps_.cend();
    RunnerSteps::StepList::const_iterator end = steps_.cend();

    for(auto it = steps_.cbegin(); it != steps_.cend(); ++it)
    {

        // Check if the it is within the window
        if( ((x1 <= it->x)  && (it->x <= x2)) &&
            ((y1 <= it->y)  && (it->y <= y2))){

            if(begin == steps_.end()) {
                begin = it;
            }

            end = it;
        } else if(begin != steps_.end())
        {
            //When we reach the end of a streak, just save it in the list
            result.push_back(std::make_pair(begin, end));

            //Clear the begin and end iterators
            begin = steps_.cend();
            end = steps_.cend();
        }
    }

    //Finally we need to check if we have a streak still open
    if(begin != steps_.end())
    {
        result.push_back(std::make_pair(begin, end));
    }

    return result;
}
