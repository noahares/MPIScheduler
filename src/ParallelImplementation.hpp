#pragma once

#include <memory>
#include <string>

#ifdef WITH_MPI
#include <mpi.h>
#endif

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
  void initParallelContext(int argc, char **argv, void *comm);
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
  bool _ownMPIContext;
#ifdef WITH_MPI
  MPI_Comm _comm;
#endif
};

}

