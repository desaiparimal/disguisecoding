// DEAR CANDIDATE, WELCOME TO NETWORKSIMULATOR.HPP
// You are welcome to view this file and it's Example Usage of functions in the Utility namespace
// Note: None of the implementation details in this file are relevant to completing the test.
// PLEASE DO NOT MODIFY THIS DOCUMENT!

// DISGUISE CODE STARTS HERE ========================================================
#include <string>
#include <thread>
#include <deque>
#include <mutex>
#include <vector>
#include <atomic>
#include <sstream>
#include <chrono>

// UTILITY NAMESPACE STARTS HERE =====================================================
namespace Utility
{
    const static std::chrono::steady_clock::time_point programStartTime = std::chrono::steady_clock::now();

    // Example usage of splitString: 
    // std::vector<std::string> stringArray = Utility::splitString("Test|String|Here", '|');
    // stringArray == { "Test", "String", "Here"}
    std::vector<std::string> splitString(const std::string& message, const char separator)
    {
        std::vector<std::string> out;
        std::istringstream f(message);
        std::string s;
        while (getline(f, s, separator)) {
            out.push_back(s);
        }
        return out;
    }

    // Example Usage of tNowSec:
    // double timeNow = Utility::tNowSec();
    // timeNow == 10.5
    double tNowSec()
    {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::milli>(now - programStartTime).count() / 1000.f;
    }
}
// UTILITY NAMESPACE ENDS HERE =======================================================


// The below code is an implementation of the network simulator tool which is NOT REQUIRED TO BE READ FOR THE TEST
namespace Network
{
  class Machine;
  class Director;
  class Actor;
}

// This class simulates receiving messages from a network.
class NetworkSimulator
{
  friend class Network::Machine;
  friend class Network::Director;
  friend class Network::Actor;
  public:
    NetworkSimulator();
    ~NetworkSimulator();
    // Note: These calls will block until a message is available.
    std::string popHeartbeatMessage();
    std::string popSessionMessage();

  private:
    void writeSessionMessage(const std::string& msg);
    void writeHeartbeatMessage(const std::string& msg);
    std::string popMessage(std::deque<std::string>& msgQue);
    std::mutex m_lock;
    std::deque<std::string> m_heartbeatMessageQue;
    std::deque<std::string> m_sessionMessageQue;
    std::thread* m_t1;
    std::atomic<bool> m_runThread;
};

// DEAR CANDIDATE, YOU CAN FREELY IGNORE EVERYTHING IN THIS NAMESPACE.
// The implementation of the network is just a detail.
namespace Network
{
  class Machine
  {
    public:
    Machine(const std::string& sessionName, const std::string& machineID, NetworkSimulator& msgSupplier);
    void update();

    protected:
    enum State
    {
      STOPPED,
      STARTING,
      ACTIVE,
      CRASHED
    } m_state;
    void enterState(const State newState);
    virtual void onBecomeActive() {};
    std::string m_sessionName;
    std::string m_machineID;
    NetworkSimulator* m_messageSupplier;
    int m_timer;
  };

  class Director : public Machine
  {
    public:
    Director(const std::string& sessionName, const std::string& machineID, const std::vector<std::string>& machineIDs, NetworkSimulator& msgSupplier);
    private:
    std::vector<std::string> m_machineIDs;
    void onBecomeActive() override;
  };

  class Actor : public Machine
  {
  public:
    Actor(const std::string& sessionName, const std::string& machineID, NetworkSimulator& msgSupplier);
  private:
    void onBecomeActive() override;
  };
}

// ------------------ NetworkSimulator ------------------ //
NetworkSimulator::NetworkSimulator()
{
  std::srand(1);
  m_t1 = new std::thread([this](){
    Network::Director d("my session", "paulspc", std::vector<std::string>({"peterspc", "rachelspc", "bobspc"}), *this);
  Network::Actor a1("my session", "peterspc", *this);
  Network::Actor a2("my session", "rachelspc", *this);
  Network::Actor a3("my session", "bobspc", *this);
  Network::Actor a4("test session", "aliceslaptop", *this);
    
  std::vector<Network::Machine*> machines({&d, &a1, &a2, &a3, &a4});

  m_runThread = true;
    
  while(m_runThread)
    {
      // update all the machines
      for (Network::Machine* machine : machines)
      {
        machine->update();
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }
  });
}

NetworkSimulator::~NetworkSimulator() 
{
  m_runThread = false;
  if (m_t1)
  {
    m_t1->join();
    delete m_t1;
  }
}

std::string NetworkSimulator::popHeartbeatMessage() { return popMessage(m_heartbeatMessageQue); }
std::string NetworkSimulator::popSessionMessage() { return popMessage(m_sessionMessageQue); };

void NetworkSimulator::writeSessionMessage(const std::string& msg)
{
  std::lock_guard<std::mutex> guard(m_lock);
  m_sessionMessageQue.push_back(msg);
  if (m_sessionMessageQue.size() > 50)
    m_sessionMessageQue.pop_front();
}
void NetworkSimulator::writeHeartbeatMessage(const std::string& msg)
{
  std::lock_guard<std::mutex> guard(m_lock);
  m_heartbeatMessageQue.push_back(msg);
  if (m_heartbeatMessageQue.size() > 50)
    m_heartbeatMessageQue.pop_front();
}

std::string NetworkSimulator::popMessage(std::deque<std::string>& msgQue)
{
  for (;;)
  {
    {
      std::lock_guard<std::mutex> guard(m_lock);
      if (!msgQue.empty())
      {
          std::string result = msgQue.front();
          msgQue.pop_front();
          return result;
      }
    }
  }
}

namespace Network
{
  int rand(const int min, const int max)
  {
      const int diff = max - min;
      const int rand = std::rand();
      const float randNorm = float(rand) / float(RAND_MAX);
      return min + int(diff * randNorm);
  }

  Machine::Machine(const std::string& sessionName, const std::string& machineID, NetworkSimulator& msgSupplier)
  {
    m_sessionName = sessionName;
    m_machineID = machineID;
    m_messageSupplier = &msgSupplier;
    enterState(STOPPED);
  }

  void Machine::enterState(const State newState)
  {
    if (newState == STOPPED)
    {
      m_timer = rand(10, 100);
    }
    else if (newState == STARTING)
    {
      // send heartbeat
      const std::string msg = "HELLO|" + m_machineID;
      m_messageSupplier->writeHeartbeatMessage(msg);
      m_timer = rand(5, 10);
    }
    else if (newState == ACTIVE)
    {
      if (m_state != ACTIVE)
      {
        onBecomeActive();
      }
      m_timer = rand(1, 5);
    }
    else if (newState == CRASHED)
    {
      m_timer = rand(1, 50);
    }
    m_state = newState;
  }

  void Machine::update()
  {
    m_timer--;
    if (m_timer == 0)
    {
      switch (m_state)
      {
        case STOPPED:
          enterState(STARTING);
          break;
        case STARTING:
          enterState(ACTIVE);
          break;
        case ACTIVE:
          if (rand(0, 100) < 95)
          {
            // send heartbeat
            const std::string msg = "MACHINESTATUS|" + m_machineID + "|11.2|60";
            m_messageSupplier->writeHeartbeatMessage(msg);
            enterState(ACTIVE);
          }
          else
          {
            enterState(CRASHED);
          }
          break;
        case CRASHED:
          enterState(ACTIVE);
          break;
        default:
          break;
      }
    }
  }

  Director::Director(const std::string& sessionName, const std::string& machineID, const std::vector<std::string>& machineIDs, NetworkSimulator& msgSupplier) :
  Machine(sessionName, machineID, msgSupplier)
  {
    m_machineIDs = machineIDs;
  }

  void Director::onBecomeActive()
  {
    std::string msg = "SESSION|" + m_sessionName + "|" + m_machineID;
    for (const std::string& s : m_machineIDs)
    {
      msg += "|" + s;
    }
    // send session
    m_messageSupplier->writeSessionMessage(msg);
  }

  Actor::Actor(const std::string& sessionName, const std::string& machineID, NetworkSimulator& msgSupplier) :
    Machine(sessionName, machineID, msgSupplier)
    {}

  void Actor::onBecomeActive()
  {
    std::string msg = "MACHINE|" + m_machineID + "|" + m_sessionName;
    // send session
    m_messageSupplier->writeSessionMessage(msg);
  }
}
// DISGUISE CODE ENDS HERE ==========================================================