#include "DynamicLibrary.hpp"
#include <dlfcn.h>
#include <fstream>
#include <iostream>

DynamicLibrary::DynamicLibrary():
  _handle(0)
{

}
  
shared_ptr<DynamicLibrary> DynamicLibrary::getLibrary(const string &libraryPath)
{
  auto library = make_shared<DynamicLibrary>();
  if (!library->setPath(libraryPath)) {
    return 0;
  }
  return library;
}
  
int DynamicLibrary::run(const string &logsFile,
      const string &errFile,
      const vector<string> &args,
      void *comm) 
{
  std::ofstream out(logsFile);
  std::streambuf *coutbuf = std::cout.rdbuf(); 
  std::cout.rdbuf(out.rdbuf()); 
  std::ofstream err(errFile);
  std::streambuf *cerrbuf = std::cerr.rdbuf(); 
  std::cerr.rdbuf(err.rdbuf());
  int argc = args.size(); 
  char **argv = new char*[argc];
  for (int i = 0; i < argc; ++i) {
    argv[i] = (char*)args[i].c_str();
    cout << argv[i] << " ";
  }
  cout << endl;
  int res = _raxmlMain(argc, argv, (void*)&comm);
  delete[] argv;
  std::cout.rdbuf(coutbuf);
  std::cerr.rdbuf(cerrbuf);
  out.close();
  err.close();
  return res;
}

bool DynamicLibrary::setPath(const string &libraryPath)
{
  _handle = dlopen(libraryPath.c_str(), RTLD_LAZY);
  if (!_handle) {
    cerr << "Cannot open shared library " << libraryPath << endl;
    cerr << "Error: " << dlerror() << endl;
    return false;
  }
  _raxmlMain = (mainFct) dlsym(_handle, "dll_main");
  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    cerr << "Cannot load symbole dll_main " << dlsym_error << endl;
    dlclose(_handle);
    _handle = 0;
    return false;
  }
  return true;
}

DynamicLibrary::~DynamicLibrary()
{
  if (_handle)
    dlclose(_handle);
}



