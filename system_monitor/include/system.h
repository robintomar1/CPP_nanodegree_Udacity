#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();                   
  std::vector<Process>& Processes();  
  float MemoryUtilization(){return LinuxParser::MemoryUtilization();}         
  long UpTime(){return LinuxParser::UpTime(); }                      
  int TotalProcesses(){return LinuxParser::TotalProcesses();}              
  int RunningProcesses(){return LinuxParser::RunningProcesses();}             
  std::string Kernel(){return _kernelVersion;}               
  std::string OperatingSystem(){return _operatingSystem;}     

  
 private:
  Processor _cpu = {};
  std::vector<Process> _processes = {};
  std::string _operatingSystem;
  std::string _kernelVersion;
};

#endif