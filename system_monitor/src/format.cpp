#include <string>
#include <sstream>
#include "format.h"

using std::string;
/**
 * @brief convert input seconds to hh:mm:ss format
 * 
 * @param sec 
 * @return string 
 */
string Format::ElapsedTime(long int sec) {
    int hours = sec/(SECONDS_PER_HOUR);
    int remainingSeconds = sec%(SECONDS_PER_HOUR);
    int minutes = remainingSeconds/SECONDS_PER_MINUTE;
    remainingSeconds = sec%SECONDS_PER_MINUTE;
    std::stringstream sStream;
    sStream << hours << ":" <<minutes<< ":" <<remainingSeconds;
    return sStream.str(); 
}