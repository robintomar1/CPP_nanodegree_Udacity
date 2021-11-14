#ifndef FORMAT_H
#define FORMAT_H

#include <string>
#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_MINUTE 60
namespace Format {
std::string ElapsedTime(long times);  
};                                    // namespace Format

#endif