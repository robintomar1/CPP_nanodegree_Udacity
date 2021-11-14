#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization(){return LinuxParser::MemoryUtilization();}          // TODO: See src/system.cpp
  long UpTime(){return LinuxParser::UpTime(); }                      // TODO: See src/system.cpp
  int TotalProcesses(){return LinuxParser::TotalProcesses();}               // TODO: See src/system.cpp
  int RunningProcesses(){return LinuxParser::RunningProcesses();}             // TODO: See src/system.cpp
  std::string Kernel(){return _kernelVersion;}               // TODO: See src/system.cpp
  std::string OperatingSystem(){return _operatingSystem;}      // TODO: See src/system.cpp

  // TODO: Define any necessary private members
 private:
  Processor _cpu = {};
  std::vector<Process> _processes = {};

  std::string _operatingSystem;
  std::string _kernelVersion;
};

#endif