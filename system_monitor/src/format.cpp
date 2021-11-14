#include <string>
#include <sstream>
#include "format.h"

using std::string;

std::string Format::NumToStr(const long num) {
    std::stringstream sStream;
    sStream << num;
    return sStream.str();
}

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long sec) {
    long hours = sec/(SECONDS_PER_HOUR);
    long remainingSeconds = sec%(SECONDS_PER_HOUR);
    long minutes = remainingSeconds/SECONDS_PER_MINUTE;
    remainingSeconds = sec%SECONDS_PER_MINUTE;
    std::stringstream sStream;
    sStream << NumToStr(hours) << ":" <<NumToStr(minutes) << ":" <<NumToStr(remainingSeconds);
    return sStream.str(); 
}