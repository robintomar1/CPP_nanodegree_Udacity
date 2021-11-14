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
  inline int Pid(){return _id;}                              
  inline std::string User(){return _userName;}                     
  inline std::string Command(){return _command; }                  
  float CpuUtilization();                  
  std::string Ram(){return LinuxParser::Ram(_id);}                      
  long int UpTime(){return LinuxParser::UpTime(_id);}                      
  bool operator>(Process const& a) const{return this->_cpuUtilization > a._cpuUtilization;};  

  
 private:
  int _id;
  std::string _userName;
  std::string _command;
  float _cpuUtilization;
};

#endif