#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
    Processor();
    float Utilization();  

 private:
    long _previousIdle;
    long _previousWorking;
    long _previousTotal;
};

#endif