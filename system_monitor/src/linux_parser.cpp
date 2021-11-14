#include <dirent.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T readValueFromFile(const std::string &path, const std::string &argumentName) {
  T value;
  std::ifstream filestream(path);

  if (filestream.is_open()) {
    std::string line;
    
    while (std::getline(filestream, line)) {
      std::istringstream lineStream(line);
      std::string argument;
      lineStream >> argument;
      
      if (argument == argumentName) {
        lineStream >> value;
        return value;
      }
    }
  }

  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  const std::string memInfoFile(kProcDirectory + kMeminfoFilename);
  const float memTotal = readValueFromFile <float> (memInfoFile, "MemTotal:");
  const float memFree = readValueFromFile <float> (memInfoFile, "MemFree:");
  const float memUsed = memTotal - memFree;
  return memUsed == 0.0 ? 0.0 : (memUsed/memTotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  long upTimeInSeconds = 0; 
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) 
  {
    filestream >> upTimeInSeconds;
  }
  return upTimeInSeconds;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{ 
  long totalTime = 0;
  std::stringstream procInfoFilePath;
  procInfoFilePath << kProcDirectory << pid << kStatFilename; 
  std::ifstream fStream(procInfoFilePath.str());
  if (fStream.is_open()) 
  {
    int position = 13;
    std::string uselessValue;
    for (int i = 0; i < position; i++) 
    {
      if (!(fStream >> uselessValue)) 
      {

        return 1;
      }
    }
    long userJiffies, kernelJiffies = 0, userChildrenJiffies = 0, kernelChildrenJiffies = 0;
    if (fStream >> userJiffies >> kernelJiffies >> userChildrenJiffies >> kernelChildrenJiffies) {
        totalTime = (userJiffies + kernelJiffies + userChildrenJiffies + kernelChildrenJiffies);
    }
  }
  return totalTime/sysconf(_SC_CLK_TCK);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{ 
  std::vector <long> cpuJiffies = CpuUtilization();
  return cpuJiffies[kUser_] + cpuJiffies[kNice_] + cpuJiffies[kSystem_] + 
         cpuJiffies[kIRQ_] + cpuJiffies[kSoftIRQ_] + cpuJiffies[kSteal_];
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  std::vector <long> cpuJiffies = CpuUtilization();
  return cpuJiffies[kIdle_] + cpuJiffies[kIOwait_];
}

// TODO: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() 
{ 
  std::vector<long> cpuJiffies;
  std::ifstream fStream(kProcDirectory + kStatFilename);
  if (fStream.is_open()) {
    std::string cpu;
    fStream >> cpu;
    if (cpu == "cpu") {
      for (int i = 0; i < CPUStates::null_; i++) {
        long value;
        fStream >> value;
        cpuJiffies.emplace_back(value);
      }
    }
  }
  return cpuJiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  int numberProcesses = 0; 
  std::string procInfoFilePath(kProcDirectory + kStatFilename);
  numberProcesses = readValueFromFile <int> (procInfoFilePath, "processes");
  return numberProcesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  uint runningProcesses = 0; 
  std::string procInfoFilePath(kProcDirectory + kStatFilename);
  runningProcesses = readValueFromFile <int> (procInfoFilePath, "procs_running");
  return runningProcesses;
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{ 
  std::string command = "ghost";
  std::string line;
  std::stringstream procInfoFilePath;
  procInfoFilePath << kProcDirectory << pid << kCmdlineFilename;
  std::ifstream fStream(procInfoFilePath.str());
    if (fStream.is_open()) 
    {
      if (std::getline(fStream, line)) 
      {
        command = line;
      }
    }

  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{ 
  std::stringstream procInfoFilePath;
  std::stringstream ramInMB;
  procInfoFilePath << kProcDirectory << pid << kStatusFilename;
  long ramInBytes = readValueFromFile <long> (procInfoFilePath.str(), "VmSize:");
  ramInMB << (ramInBytes/(1024));
  return ramInMB.str(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)
{ 
  std::string uId = "0000";
  std::stringstream procInfoFilePath;
  procInfoFilePath << kProcDirectory << pid << kStatusFilename;
  uId = readValueFromFile <std::string> (procInfoFilePath.str(), "Uid:");
  return uId;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  std::string userId = Uid(pid);
  std::string currentId;
  std::string unusedField;
  string userName = "ghost";
  std::ifstream iStream(kPasswordPath);
  
  if (iStream.is_open()) 
  {
    std::string line;
    while (std::getline(iStream, line)) 
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lStream(line);
      if (lStream >> userName >> unusedField >> currentId)
      {
        if (currentId == userId) 
        {
          return userName;
        }
      }
    }
  }
  return userName;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  long procUpTimeInClockTicks = 0;
  std::stringstream procInfoFilePath;
  procInfoFilePath << kProcDirectory << pid << kStatFilename; 
  std::ifstream iStream(procInfoFilePath.str());
  if (iStream.is_open()) 
  {
  int position = 22;
    std::string value;
    for (int i = 0; i < position; i++) 
    {
      if (!(iStream >> value)) 
      {
        return 1000;
      }
    }
    procUpTimeInClockTicks = std::stol(value);
  }
  return procUpTimeInClockTicks/sysconf(_SC_CLK_TCK);
}