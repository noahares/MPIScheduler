#pragma once

#include "Common.hpp"
#include <string>
#include <memory>
#include <vector>




#include <map>
#include <stack>
#include "Checkpoint.hpp"

using namespace std;

namespace MPIScheduler {


class Command {
public:
  Command(const string &id, 
      unsigned int ranks,
      long estimatedCost,
      const vector<string> &arguments);
  virtual ~Command() {}
 
  const string &getId() const {return _id;}
  long getEstimatedCost() const {return _estimatedCost;}
  void setRanksNumber(int ranks) {_ranksNumber = ranks;}
  int getRanksNumber() const {return _ranksNumber;}
  string toString() const;
  const vector<string> &getArgs() const {return _args;}
public:
  // initial information
  const string _id;
  const vector<string> _args;
  int _ranksNumber;
  long _estimatedCost;
};
using CommandPtr = shared_ptr<Command>;

}
