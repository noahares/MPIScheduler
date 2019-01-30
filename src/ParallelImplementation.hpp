#pragma once

#include <memory>
#include <string>

using namespace std;

namespace MPIScheduler {

class RanksAllocator;
class SchedulerArgumentsParser;

class ParallelImplementation {
  enum Impl {
    split,
    onecore,
    fork,
    invalid
  };
public:
  ParallelImplementation(const string &implem);
  bool isValid() const;
  int getRank() const;
  int getRanksNumber() const;
  bool isMPI() const;
  void initParallelContext(int argc, char **argv);
  void closeParallelContext();
  bool slavesToStart() const;
  void startSlaves(int argc, char **argv);
  shared_ptr<RanksAllocator> getRanksAllocator(SchedulerArgumentsParser &arg,
                                    int ranksNumber);
  bool addFakeExecutableName() {return _impl == split;}
private:
  Impl _impl;
  int _rank;
  int _ranksNumber;
};

}

