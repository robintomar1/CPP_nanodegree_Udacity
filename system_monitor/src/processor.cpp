#include "linux_parser.h"
#include "processor.h"

Processor::Processor() :
    _previousIdle(LinuxParser::IdleJiffies()),
    _previousWorking(LinuxParser::ActiveJiffies()),
    _previousTotal(_previousIdle + _previousWorking) {}

/**
 * @brief Return the aggregate CPU utilization
 * 
 * @return float 
 */
float Processor::Utilization() { 
    long currentIdle = LinuxParser::IdleJiffies(); 
    long currentWorking = LinuxParser::ActiveJiffies(); 
    long currentTotal = currentIdle + currentWorking;
    long deltaIdle = currentIdle - _previousIdle;
    long deltaTotal = currentTotal - _previousTotal;
    _previousIdle = currentIdle;
    _previousWorking = currentWorking;
    _previousTotal = currentTotal;
    float cpuUsage = deltaTotal == 0 ? 0.0f : (static_cast<float>(deltaTotal - deltaIdle))/deltaTotal;
    return cpuUsage;
}