
#include <iostream>
#include <string>

#include "RanksAllocator.hpp"
#include "ParallelImplementation.hpp"
#include "Command.hpp"
#include "Common.hpp"
#include "CommandsRunner.hpp"
#include "RunStatistics.hpp"

namespace MPIScheduler {

  
int main_scheduler(int argc, char **argv, void* comm)
{
  // Init
  SchedulerArgumentsParser arg(argc, argv);
  ParallelImplementation implem(arg.implem);
  if (!implem.isValid()) {
    cerr << "Invalid scheduler implementation: " << arg.implem << endl;
    return 1;
  }
  implem.initParallelContext(argc, argv, comm);
  if (implem.slavesToStart()) {
    implem.startSlaves(argc, argv);
    if (implem.getRank() != implem.getRanksNumber() - 1) {
      implem.closeParallelContext();
      return 0;
    }
  }
  Time begin = Common::getTime();
  CommandsContainer commands(arg.commandsFilename,
      implem.addFakeExecutableName());
  auto allocator = implem.getRanksAllocator(arg, implem.getRanksNumber());
  for (auto command: commands.getCommands()) {
    allocator->preprocessCommand(command);
  }
  
  // Run 
  CommandsRunner runner(commands, allocator, arg.outputDir, arg.jobFailureFatal);
  runner.run(implem.isMPI());
  cerr << "end of run" << endl;
  // End
  Time end = Common::getTime();
  RunStatistics statistics(runner.getHistoric(), begin, end, implem.getRanksNumber() - 1);
  statistics.printGeneralStatistics();
  if (runner.getHistoric().size()) {
    statistics.exportSVG(Common::getIncrementalLogFile(arg.outputDir, "statistics", "svg"));
  }
  allocator->terminate();
  implem.closeParallelContext();
  cout << "End of Multiraxml run" << endl;
  return 0;
}

} // namespace MPIScheduler



#ifdef MPISCHEDULER_BUILD_AS_LIBRARY

extern "C" int mpi_scheduler_main(int argc, char** argv, void* comm)
{
  int res =  MPIScheduler::main_scheduler(argc, argv, comm);
  MPI_Barrier(*((MPI_Comm*)comm)); 
  return res;
}

#else

int main(int argc, char** argv) 
{
  exit(MPIScheduler::main_scheduler(argc, argv, 0));
}


#endif
