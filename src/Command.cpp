

#include "Command.hpp"

namespace MPIScheduler {
  

Command::Command(const string &id, 
    unsigned int ranks,
    long estimatedCost,
    const vector<string> &arguments):
  _id(id),
  _args(arguments),
  _ranksNumber(ranks),
  _estimatedCost(estimatedCost)
{
}

}
