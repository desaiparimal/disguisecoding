#include <string>
#include <sstream>
#include <iostream>

#include <deque>
#include <vector>

#include <thread>
#include <mutex>
#include <atomic>

#include <chrono>

#include "NetworkSimulator.hpp"

int main() {

  NetworkSimulator networkSimulator;

  // Example code for getting some messages. Run this to see what the messages look like.
  while(1)
  {
    const std::string sessionMsg = networkSimulator.popSessionMessage();
    std::cout << sessionMsg << "\n";
    
    const std::string heartbeatMsg = networkSimulator.popHeartbeatMessage();
    std::cout << heartbeatMsg << "\n";
  }

  // Hint: We recommend pushing at least 1 commit after each task (though you are welcome to push more frequently)
  
  // Task 1: Using an environment of your choice, build the attached code and run it to see some of the messages described in the protocols printed to the console.

  // Task 2: Using the provided Utility::splitString() function (example usage and implementation can be found in the NetworkSimulator.hpp file), create a function that parses machine startup messages and adds data to a data structure of your choice that reflects the network.

  // Task 3: Using your data structures, print to the console a rolling summary of each Session along with which machines are in the session and which one is the Director.

  // Task 4: Create a function that parses the heartbeat messages and uses the Utility::tNowSec() function (example usage and implementation can be found in the NetworkSimulator.hpp file) to determine which machines in the network are “Online”. Add this status to the logging you introduced in task 2.

  // Task 5: You probably noticed by now that popSessionMessage and popHeartbeatMessage are blocking. They are also thread-safe. Using threads, ensure you are receiving all of the messages and filling your data structure in a thread-safe way.
  return 0;
}