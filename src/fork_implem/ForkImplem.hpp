#pragma once

#include "../CommandsRunner.hpp"
#include <memory>
#include <unordered_set>
#include <queue>

namespace MPIScheduler {

class ForkInstance: public Instance {
public:
  ForkInstance(const string &outputDir, 
      const string &execPath,
      int coresOffset, 
      int cores, 
      CommandPtr command,
      const string &threadsArg);

  virtual ~ForkInstance() {}
  virtual bool execute(InstancePtr self);
  bool checkFinished();
  int getReturnValue() {return _returnValue;}
private:
  int executeChild(const CommandPtr command, 
    const string &outputDir); 
  int _pid;
  int _returnValue;
  string _execPath;
  Timer _timer;
  string _threadsArg;
};

class ForkRanksAllocator: public RanksAllocator {
public:
  // available threads must be a power of 2
  ForkRanksAllocator(int availableRanks, 
      const string &execPath,
      const string &outputDir,
      const string &threadsArg);
  virtual ~ForkRanksAllocator() {}
  virtual bool ranksAvailable();
  virtual bool allRanksAvailable();
  virtual InstancePtr allocateRanks(int requestedRanks, 
      CommandPtr command);
  virtual void freeRanks(InstancePtr instance);
  virtual vector<InstancePtr> checkFinishedInstances();
  virtual void terminate();
  struct Slot {
    Slot():
      startingRank(0),
      ranksNumber(0)
    {}
    Slot(int _startingRank, int _ranksNumber) : 
      startingRank(_startingRank),
      ranksNumber(_ranksNumber)
    {}
    int startingRank; 
    int ranksNumber;
  };
  queue<Slot> _slots;
private:
  unordered_set<shared_ptr<ForkInstance> > _runningInstances; 
  int _totalAvailableCores;
  int _coresInUse;
  string _outputDir;
  string _execPath;
  string _threadsArg;
};


} // namespace MPIScheduler



