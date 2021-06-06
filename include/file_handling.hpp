#ifndef FILE_HANDLING_HPP
#define FILE_HANDLING_HPP
#include "FS.h"
#include "SPIFFS.h"

class file_handling{
public:
  static void readFile(fs::FS &fs, const char * path);

  static void writeFile(fs::FS &fs, const char * path, std::string message);

  static void deleteFile(fs::FS &fs, const char * path);

  static void appendFile(fs::FS &fs, const char * path, std::string message);
};

#endif // FILE_HANDLING_HPP