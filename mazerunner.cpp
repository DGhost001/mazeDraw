#include "mazerunner.hpp"

#include <subprocess.hpp>

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


MazeRunner::MazeRunner(const std::string &executable)
{
    std::string tmpFile = "/tmp/runMaze.maze";

    /* Create the process and make it running */
    process_ = std::make_shared<subprocess::Popen>(
                subprocess::RunBuilder({executable, tmpFile})
                .cin(subprocess::PipeOption::pipe)
                .popen());
}

MazeRunner::~MazeRunner( void )
{

}

void MazeRunner::parseNextStep( const std::string& line)
{

}

std::string MazeRunner::readLine()
{
    if(bytesInBuffer_ <= bufferReadPointer_  ) {
        bufferReadPointer_ = 0;
        ssize_t  bytesRead = subprocess::pipe_read(process_->cout, &buffer_[0], buffer_.size());

        if(bytesRead >= 0) {
            bytesInBuffer_ = bytesRead;
        } else {
            isEof_ = true;
        }
    }
}
