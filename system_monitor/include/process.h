#ifndef PROCESS_H
#define PROCESS_H
#include "linux_parser.h"
#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(const int id);
  int Pid(){return _id;}                               // TODO: See src/process.cpp
  std::string User(){return _userName;}                      // TODO: See src/process.cpp
  std::string Command(){return _command; }                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram(){return LinuxParser::Ram(_id);}                       // TODO: See src/process.cpp
  long int UpTime(){return LinuxParser::UpTime(_id);}                      // TODO: See src/process.cpp
  bool operator>(Process const& a) const{return this->_cpuUtilization > a._cpuUtilization;};  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int _id;
  std::string _userName;
  std::string _command;
  float _cpuUtilization;
};

#endif