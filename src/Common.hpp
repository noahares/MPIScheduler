
#pragma once

#ifdef WITH_MPI
#include <cstdio>
#include <mpi.h>
#endif

#include <fstream>
#include <chrono>
#include <thread>
#include <cstdio>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <signal.h>  
#include <iostream>
#include <string>
#include <cassert>

namespace MPIScheduler {

class MPISchedulerException: public std::exception {
public:
  explicit MPISchedulerException(const std::string &s): msg_(s) {}
  MPISchedulerException(const std::string &s1, 
      const std::string &s2): msg_(s1 + s2) {}
private:
  std::string msg_;
};

using Time = std::chrono::time_point<std::chrono::system_clock>;


class Common {
public:
  // todobenoit not portable
  // todobenoit not portable
  static void makedir(const std::string &name) {
    mkdir(name.c_str(), 0755);
  }
  
  static std::string joinPaths(const std::string &path1, const std::string &path2) {
    return path1 + "/" + path2;
  }

  static std::string joinPaths(const std::string &path1, 
      const std::string &path2,
      const std::string &path3) {
    return joinPaths(joinPaths(path1, path2), path3);
  }

  static Time getTime() {
    return std::chrono::system_clock::now();
  }

  static std::string getIncrementalLogFile(const std::string &path, 
      const std::string &name,
      const std::string &extension);

  static long getElapsedMs(Time begin, Time end) {
    return std::chrono::duration_cast<std::chrono::milliseconds>
      (end-begin).count();
  }

#ifdef WITH_MPI
  static void check(int mpiError) {
    if (mpiError != MPI_SUCCESS) {
      std::cout << "MPI ERROR !!!!" << std::endl;
      throw MPISchedulerException("MPI error !");
    }
  }
#endif

  static int getPid() {
    return getpid();
  }
  
  static std::string getHost() {
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    return std::string(hostname);
  }


};

int systemCall(const std::string &command, const std::string &outputFile, bool threadSafe = false);


class Timer {
public:
  Timer() {
    reset();
  }

  long getElapsedMs() const {
    auto end = Common::getTime();
    return Common::getElapsedMs(_start, end);
  }

  void reset() {
    _start = std::chrono::system_clock::now();
  }
private:
  Time _start;
};

class SchedulerArgumentsParser {
public:
  SchedulerArgumentsParser(int argc, char** argv):
    commandsFilename()
  {
    if (argc < 6) {
      print_help();
      throw MPISchedulerException("Error: invalid syntax");
    }
    int i = 1;
    implem = std::string(argv[i++]);
    library = std::string(argv[i++]);
    commandsFilename = std::string(argv[i++]);
    outputDir = std::string(argv[i++]);
    jobFailureFatal = atoi(argv[i++]);
    if (i < argc) {
      threadsArg = std::string(argv[i++]);
    }
    if (i < argc) {
      outputLogs = std::string(argv[i++]);
    }
  }
  
  void print_help() 
  {
    std::cout << "Syntax:" << std::endl;
    std::cout << "mpi-scheduler implem library command_file output_dir job_failure_fatal" << std::endl;
  }

  std::string implem;
  std::string library;
  std::string commandsFilename;
  std::string outputDir;
  int jobFailureFatal;
  std::string threadsArg;
  std::string outputLogs;
};

} // namespace MPIScheduler

