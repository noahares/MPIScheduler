

#include "Command.hpp"

namespace MPIScheduler {
  

Command::Command(const std::string &id, 
    unsigned int ranks,
    long estimatedCost,
    const std::vector<std::string> &arguments):
  _id(id),
  _args(arguments),
  _ranksNumber(ranks),
  _estimatedCost(estimatedCost)
{
}

}
