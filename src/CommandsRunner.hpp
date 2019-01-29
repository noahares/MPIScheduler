#pragma once

#include "Command.hpp"
#include "Instance.hpp"
#include "RanksAllocator.hpp"

namespace MPIScheduler {

class SVGDrawer;

class CommandsContainer {
public:
  CommandsContainer() {}
  explicit CommandsContainer(const string &commandsFilename,
      bool addFakeExecutableName);

  CommandPtr getCommand(string id) const;
  vector<CommandPtr> &getCommands() {return _commands;}
  const vector<CommandPtr> &getCommands() const {return _commands;}
private:
  void addCommand(CommandPtr command);

  vector<CommandPtr> _commands;
  map<string, CommandPtr> _dicoCommands;
};

class CommandsRunner {
public:
  CommandsRunner(const CommandsContainer &commandsContainer,
      shared_ptr<RanksAllocator> allocator,
      const string &outputDir,
      bool jobFailureFatal);
  void run();
  void runOpenMP();
  const InstancesHistoric &getHistoric() const {return _historic;} 
private:
  
  static bool compareCommands(CommandPtr c1, CommandPtr c2);
  CommandPtr getPendingCommand() {return *_commandIterator;}
  bool isCommandsEmpty() {return _commandIterator == _commandsVector.end();}
  
  bool executePendingCommand();
  void onFinishedInstance(InstancePtr instance);
  
  const CommandsContainer &_commandsContainer;
  
  const string _outputDir;

  shared_ptr<RanksAllocator> _allocator;
  vector<CommandPtr> _commandsVector;
  vector<CommandPtr>::iterator _commandIterator;
  Checkpoint _checkpoint;
  InstancesHistoric _historic;
  int _finishedInstancesNumber;
  bool _verbose;
  bool _jobFailureFatal;
};

}
