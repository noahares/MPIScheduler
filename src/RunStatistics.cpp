
#include "RunStatistics.hpp"

#include "SVGDrawer.hpp"

namespace MPIScheduler {

RunStatistics::RunStatistics(const InstancesHistoric &historic,
    Time begin,
    Time end,
    int availableRanks):
  _historic(historic),
  _begin(begin),
  _end(end),
  _availableRanks(availableRanks),
  _lbRatio(1.0)
{

}

void RunStatistics::printGeneralStatistics()
{
  long totalElapsedTime = Common::getElapsedMs(_begin, _end);
  long cumulatedTime = 0;
  for (auto instance: _historic) {
    cumulatedTime += instance->getElapsedMs() * instance->getRanksNumber();
  }
  _lbRatio = double(cumulatedTime) / double(_availableRanks * totalElapsedTime);
  
  cout << "Finished running commands. Total elasped time: ";
  cout << totalElapsedTime / 1000  << "s" << endl;
  cout << "Load balance ratio: " << _lbRatio << endl;
}


void RunStatistics::exportSVG(const string &svgfile) 
{
  Timer t;
  cout << "Saving svg output in " << svgfile << endl;
  int totalWidth = _availableRanks + 1;
  cout << "total width " << totalWidth << endl;
  int totalHeight = Common::getElapsedMs(_begin, _end);
  string caption = "t = " + to_string(totalHeight / 1000) + "s";
  caption += ", lb = " + to_string(_lbRatio);
  SVGDrawer svg(svgfile, double(totalWidth), double(totalHeight));
  svg.writeHeader(caption);
  for (auto instance: _historic) {
    instance->writeSVGStatistics(svg, _begin);
  }
  svg.writeFooter();
  cout << "Time spent writting svg: " << t.getElapsedMs() / 1000 << "s" << endl;
}

} // namespace MPIScheduler



