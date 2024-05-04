# Instructions
Welcome to the disguise code test! Congratulations on getting this this phase of the interview process, we're excited to see your coding skills! This test has a time limit of 4 hours but we expect it not to take that long (We build in extra time for potential distractions!).

Good Luck!

## Problem Description
In a typical d3 set up a user will have one Director machine and several Actor machines. These machines are bound together in a session. The Director controls the session and knows which machines should be allowed to join. Directors and Actors both broadcast a message when they start up and then send a regular heartbeat message. These two message types use a similar protocol but are received through different means.

We would like you to write a small program in C++ to monitor these messages and display the current status of the network. We’ve split the work in to 5 tasks which should be completed in-order. We do not expect all applicants to complete all the tasks, you should work through as many as you can in the time and try to complete them 1 at a time. The tasks are outlined in comments in the main.cpp file and also copied here below the protocol specifications. You may use any environment or tooling that you feel comfortable with. 

We have provided a NetworkSimulator.hpp file which includes a class called NetworkSimulator which provides an interface to receive these messages. An example of how to receive some of the messages is shown in the main.cpp file. This file also includes several utility methods in the Utility namespace that should be used when appropriate. We have provided these tools to help you avoid writing unnecessary code, so please use them freely.

Note: Please do not modify the NetworkSimulator.hpp file. Your work should be in the main.cpp file or any other files you choose to create.

## Scoring Criteria
We are particularly interested in:  
- Code style
- How you check for and handle errors
- How you organize your data structures
- How you handle memory and threading


## Protocol Specification

### Protocol 1
Each message is contained within a single string and can be retrieved from the NetworkSimulator class by calling popSessionMessage. The message is a plaintext string and the method is thread-safe.

Directors send this on startup:
SESSION|sessionname|creatormachineid|machineid1|machineid2|machineid3|..|machineidn

Example:
SESSION|my session|alicespc|bobspc|carolspc

Actors send this on startup:
MACHINE|machineid|sessionname

Example:  
MACHINE|davidspc|my session

There are other message types sent over the protocol that you should ignore.

### Protocol 2
Each message is contained within a single string and can be retrieved from the NetworkSimulator class by calling popHeartbeatMessage. The message is a plaintext string and the method is thread-safe.

Every machine sends this at least once a second:  
MACHINESTATUS|machineid|version|fps

Example:  
MACHINESTATUS|evespc|11.2|60

There are other message types sent over the protocol that you should ignore.

## Tasks
Here is a copy of the tasks which already exist in the main.cpp file in case you delete them by accident, We recommend pushing a commit after each task at minimum (though you are welcome to make more commits):

Task 1: Using an environment of your choice, build the attached code and run it to see some of the messages described in the protocols printed to the console.

Task 2: Using the provided Utility::splitString() function (example usage and implementation can be found in the NetworkSimulator.hpp file), create a function that parses machine startup messages and adds data to a data structure of your choice that reflects the network.

Task 3: Using your data structures, print to the console a rolling summary of each Session along with which machines are in the session and which one is the Director.

Task 4: Create a function that parses the heartbeat messages and uses the Utility::tNowSec() function (example usage and implementation can be found in the NetworkSimulator.hpp file) to determine which machines in the network are “Online”. Add this status to the logging you introduced in task 2.

Task 5: You probably noticed by now that popSessionMessage and popHeartbeatMessage are blocking. They are also thread-safe. Using threads, ensure you are receiving all of the messages and filling your data structure in a thread-safe way.