#include "mazerunner.hpp"
#include "Labyrinth.hpp"

#include "Exception.hpp"

#include <subprocess.hpp>
#include <iostream>

static std::string trim(const std::string& str,
                        const std::string& whitespace = " \t\r\n")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}


MazeRunner::MazeRunner(const std::string &executable, std::shared_ptr<Labyrinth> labyrinth):
    points_(100),
    cx_(0),
    cy_(0),
    lastRead_(std::chrono::steady_clock::now()),
    tmpFileName_("/tmp/runMaze.maze"),
    executable_(executable),
    failed_(false),
    finished_(false),
    collectedTreshure_(false),
    isEof_(false),
    lab_(labyrinth),
    bytesInBuffer_(0),
    bufferReadPointer_(0)
{
    /* Save the loaded lab to the file, so it can be used by the runner */
    lab_->save(tmpFileName_);
    findStartPoint();
}

MazeRunner::~MazeRunner( void )
{
    /* Restore the lab from the saved file */

    if(process_) {
        process_.reset();
    }

    lab_->load(tmpFileName_);
}


RunnerSteps::StepList MazeRunner::run()
{
    auto start = std::chrono::steady_clock::now();
    /* Create the process and make it running */
    process_ = std::make_shared<subprocess::Popen>(
                subprocess::RunBuilder({executable_, tmpFileName_})
                .cout(subprocess::PipeOption::pipe)
                .popen());


    RunnerSteps::Steps const initial = {
        .timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start),
        .x = cx_,
        .y = cy_,
        .points = points_
    };

    steps_.push_back(initial);

    while(!isEof_ && !finished_ && !failed_) {
        std::string line = readLine();
        parseNextStep(line);

        RunnerSteps::Steps const step = {
            .timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start),
            .x = cx_,
            .y = cy_,
            .points = points_
        };

        steps_.push_back(step);

        //std::cout<<"x: "<<cx_<<"\ty: "<<cy_<<"\tPoints:"<<points_<<std::endl;
    }

    return steps_;
}

void MazeRunner::parseNextStep( const std::string& line)
{
    std::string const trimed = trim(line);

    if(trimed.length() != 1) {
        failed_ = true;
    } else {
        /* Handle the output from the maze runner */
        switch (trimed[0]) {
        case 'L': --cx_; break;
        case 'R': ++cx_; break;
        case 'O': --cy_; break;
        case 'U': ++cy_; break;
        default:
            failed_ = true;
            break;
        }

        /* check that the runner is still in the maze */
        if(cx_ < 0 || cx_ >= 1000 ||
           cy_ < 0 || cy_ >= 1000) {
            failed_ = true;
        }

        if(!failed_) {
            /* Compute the points for the current step */

            points_ -= (collectedTreshure_ ? 2 : 1);

            Labyrinth::MazeContent pos = lab_->get(cx_, cy_);

            switch (pos) {
            case Labyrinth::MazeContent::Wall: failed_ = true; break;
            case Labyrinth::MazeContent::Treasure:
                collectedTreshure_ = true;
                points_ += 100;
                lab_->set(cx_,cy_,Labyrinth::MazeContent::Grass);
                break;
            case Labyrinth::MazeContent::Waterbottle:
                points_ += 25;
                lab_->set(cx_,cy_,Labyrinth::MazeContent::Grass);
                break;
            case Labyrinth::MazeContent::Start: finished_ = collectedTreshure_; break;
            default:
                break;
            }

            failed_ = points_ <= 0;
        }
    }
}

std::string MazeRunner::readLine()
{
    std::string result;

    do {
        if(bytesInBuffer_ <= bufferReadPointer_  ) {
            bufferReadPointer_ = 0;
            ssize_t  bytesRead = subprocess::pipe_read(process_->cout, &buffer_[0], buffer_.size());

            if(bytesRead >= 0) {
                bytesInBuffer_ = bytesRead;

                if(bytesRead > 0) {
                    lastRead_ = std::chrono::steady_clock::now();
                }

            } else {
                isEof_ = true;
            }
        } else {
            unsigned i;
            for(i = bufferReadPointer_;i < bytesInBuffer_ && buffer_[i] != '\n'; ++i) {}

            result.append(buffer_.cbegin()+bufferReadPointer_, buffer_.cbegin()+i+1);
            bufferReadPointer_ = i+1;
        }
#if 1
        if(std::chrono::steady_clock::now() - lastRead_ > std::chrono::seconds(15)) {
            failed_ = true;
            finished_ = false;
            collectedTreshure_ = false;
            isEof_ = true;
        }
#endif
    } while (!result.ends_with('\n') && !isEof_);

    return result;
}

void MazeRunner::findStartPoint()
{
    for(size_t x = 0; x < 1000; ++x)
        for(size_t y = 0; y < 1000; ++y) {
            if(lab_->get(x,y) == Labyrinth::MazeContent::Start)
            {
                cx_ = x;
                cy_ = y;

                /* break here */
                return;
            }
        }

    throw std::out_of_range("No start point in labyrinth!");
}
