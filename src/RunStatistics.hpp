#pragma once

#include "Instance.hpp"
#include "Common.hpp"

#include <string>

namespace MPIScheduler {

class RunStatistics {
public:
  RunStatistics(const InstancesHistoric &historic, 
      Time begin, 
      Time end,
      int availableRanks);
  void printGeneralStatistics();
  void exportSVG(const string &svgfile);
private:
  const InstancesHistoric &_historic;
  Time _begin;
  Time _end;
  int _availableRanks;
  double _lbRatio;
};

} // namespace MPIScheduler



