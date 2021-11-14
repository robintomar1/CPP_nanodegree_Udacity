#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
    Processor();
    float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    long _previousIdle;
    long _previousWorking;
    long _previousTotal;
};

#endif