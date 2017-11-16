#ifndef LOGGER
#define LOGGER

#include <sstream>
#include <fstream>

class Logger
{
public:
  static void clear()
  {
    std::ofstream myfile;
    myfile.open("log.txt", std::ofstream::out | std::ofstream::trunc);
    myfile.close();
  }

  static void log(std::string txt)
  {
    std::ofstream myfile;
    myfile.open("log.txt", std::ios::app);
    myfile << txt;
    myfile.close();
  }

  static void logInt(int x)
  {
    std::ostringstream s;
    s << x << "\n";
    log(s.str());
  }
};

#endif