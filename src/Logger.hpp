#pragma once

#include <iostream>
#include <fstream>

namespace MPIScheduler {

using namespace std;

class Logger {
public:
  Logger(): _os(0) {

  }

  ~Logger() {
    delete _os;
  }

  void redirectLogs(const string &file) {
    getCout() << "Redirecting logs to " << file << endl;
    delete _os;
    _os = new ofstream(file);
  }

  ostream &getCout() {
    if (_os) {
      return *_os;
    }
    return cout;
  };

  ostream &getCerr() {
    if (_os) {
      return *_os;
    }
    return cerr;
  }

private:
  ofstream *_os;
};










}
