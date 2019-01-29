#include "ForkImplem.hpp"
#include <cassert>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream> 
namespace MPIScheduler {

ForkRanksAllocator::ForkRanksAllocator(int availableRanks, 
    const string &execPath,
    const string &outputDir):
  _totalAvailableCores(availableRanks - 1),
  _coresInUse(0),
  _outputDir(outputDir),
  _execPath(execPath)
{
  Common::makedir(Common::joinPaths(outputDir, "per_job_logs"));
  Common::makedir(Common::joinPaths(outputDir, "running_jobs"));

}

bool ForkRanksAllocator::ranksAvailable()
{
  return _coresInUse < _totalAvailableCores;
}

bool ForkRanksAllocator::allRanksAvailable()
{
  return !_coresInUse;
}
  
InstancePtr ForkRanksAllocator::allocateRanks(int requestedRanks, 
      CommandPtr command)
{
  shared_ptr<ForkInstance> instance(new ForkInstance(_outputDir,
    _execPath,
    1,
    command));
  _runningInstances.insert(instance);
  _coresInUse +=1;
  return  instance;
}

void ForkRanksAllocator::freeRanks(InstancePtr instance) 
{
  _coresInUse -= instance->getRanksNumber();
}

vector<InstancePtr> ForkRanksAllocator::checkFinishedInstances()
{
  vector<InstancePtr> res;
  auto instanceIt = _runningInstances.begin();
  while (instanceIt != _runningInstances.end()) {
    if ((*instanceIt)->checkFinished()) {
      res.push_back(*instanceIt);
      instanceIt = _runningInstances.erase(instanceIt);
    } else {
      instanceIt++;
    }
  }
  return res;
}

void ForkRanksAllocator::terminate()
{

}
  
ForkInstance::ForkInstance(const string &outputDir, 
      const string &execPath,
      int cores, 
      CommandPtr command):
  Instance(command, 0, cores, outputDir),
  _pid(0),
  _execPath(execPath)
{

}

bool ForkInstance::execute(InstancePtr self)
{
  pid_t pid = fork();
  assert(pid >= 0);
  if (pid == 0) {
    executeChild(_command, getOutputDir());
    exit(0);
  } else if (pid > 0) {
    _pid = pid; 
  }
  return true;
}

int ForkInstance::executeChild(const CommandPtr command, 
    const string &outputDir) 
{
  string logsFile = Common::joinPaths(outputDir, "per_job_logs", command->getId() + "_out.txt");
  string runningFile = Common::joinPaths(outputDir, "running_jobs", command->getId());
  ofstream os(runningFile);
  os << logsFile << endl;
  os.close();
  const vector<string> &args  = command->getArgs();
  string systemCommand = _execPath;
  for (auto &arg: args) {
    systemCommand = systemCommand + " " + arg;
  }
  int result = systemCall(systemCommand, logsFile);
  remove(runningFile.c_str());
  return result;
}

bool ForkInstance::checkFinished()
{
  int status;
  pid_t result = waitpid(_pid, &status, WNOHANG);
  assert(result != -1);
  return result != 0;

}

}
