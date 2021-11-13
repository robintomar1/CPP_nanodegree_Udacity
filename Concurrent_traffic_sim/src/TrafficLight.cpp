#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */
 using namespace std::chrono_literals;
 
template <typename T>
T MessageQueue<T>::receive()
{
    // Create a lock and pass it to the condition variable
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] { return !_queue.empty(); }); // pass unique lock to condition variable

    // Get the latest element and remove it from the queue
    T msg = std::move(_queue.back());
    _queue.pop_back();

    return msg; // will not be copied due to return value optimization (RVO) in C++
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> uLock(_mutex);
    // Move into queue
    _queue.push_back(std::move(msg));
    _cond.notify_one(); // notify client
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
	//randomly choose initial state of traffic light
    _currentPhase = TrafficLightPhase(rand() % 2);

}

void TrafficLight::waitForGreen()
{
    while (true) {

         // Wait for 1ms
        std::this_thread::sleep_for(1ms);

        // Return when received green light message
        if(_messageQueue.receive() == TrafficLightPhase::green){
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // Use current time as 
    // seed for random generator
	srand(time(0));
	//random time in millisecond between 4 and 6 sec
	int loopTime = (rand() % 2000 )+ 4000;
    // Timer to measure the time between two loop cycles
    std::chrono::time_point<std::chrono::steady_clock> lastUpdate = std::chrono::steady_clock::now();
    // Infinite loop
    while (true) 
    {
        // Wait for 1 ms
	   
        std::this_thread::sleep_for(1ms);

        // Timer to measure the time between two loop cycles
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastUpdate).count();

        if (timeSinceLastUpdate >= loopTime){
            // Toggles the current phase of the traffic light between red and green
			_currentPhase = (_currentPhase == TrafficLightPhase::red) ? TrafficLightPhase::green : TrafficLightPhase::red;
            // Send update method to the message queue using move semantics
			_messageQueue.send(std::move(_currentPhase));
			loopTime = (rand() % 2000 )+ 4000;
			lastUpdate = std::chrono::steady_clock::now();
        }
    }
}