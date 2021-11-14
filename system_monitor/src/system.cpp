#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() : 
    _operatingSystem(LinuxParser::OperatingSystem()),
    _kernelVersion(LinuxParser::Kernel())
{ 
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return _cpu; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    set<int> container;
    for (Process p : _processes) {
        container.insert(p.Pid());
    }
    std::vector<int> ids = LinuxParser::Pids();
    for (int id: ids) {
        if (container.find(id) == container.end()) {
            _processes.emplace_back(Process(id));
        }
    }
    for (Process& process: _processes) {
        process.CpuUtilization();
    }
    std::sort(_processes.begin(), _processes.end(), std::greater<Process>());
    return _processes; 
}

// // TODO: Return the system's kernel identifier (string)
// std::string System::Kernel() { 
//     return _kernelVersion;
// }

// // TODO: Return the system's memory utilization
// float System::MemoryUtilization() { 
//     return LinuxParser::MemoryUtilization(); 
// }

// // TODO: Return the operating system name
// std::string System::OperatingSystem() { 
//     return _operatingSystem;
// }

// // TODO: Return the number of processes actively running on the system
// int System::RunningProcesses() { 
//     return LinuxParser::RunningProcesses(); 
// }

// // TODO: Return the total number of processes on the system
// int System::TotalProcesses() { 
//     return LinuxParser::TotalProcesses();
// }

// // TODO: Return the number of seconds since the system started running
// long int System::UpTime() { 
//     return LinuxParser::UpTime(); 
// }