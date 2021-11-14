#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(const int id) :
    _id(id)
{
    _userName = LinuxParser::User(_id);
    _command = LinuxParser::Command(_id);
}

/**
 * @brief Return this process's CPU utilization
 * 
 * @return float 
 */
float Process::CpuUtilization() {
    int systemUpTime = LinuxParser::UpTime();
    int totalTimeActive = LinuxParser::ActiveJiffies(_id);
    int processUpTime = LinuxParser::UpTime(_id);
    int totalTimeSinceStartUp = systemUpTime - processUpTime;

    _cpuUtilization = static_cast<float>(totalTimeActive)/totalTimeSinceStartUp;
    return (_cpuUtilization);
}
