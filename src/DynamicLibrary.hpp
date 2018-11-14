#pragma once

#include <memory>
#include <string>
#include <vector>

using namespace std;
typedef int (*mainFct)(int,char**,void*);  

class DynamicLibrary {
public:
  DynamicLibrary();
  ~DynamicLibrary();
  static shared_ptr<DynamicLibrary> getLibrary(const string &libraryPath);
  int run(const string &logFile,
      const string &errFile,
      const vector<string> &args,
      void *comm); 
private:
  bool setPath(const string &libraryPath);
  void *_handle;
  mainFct _raxmlMain;
};




