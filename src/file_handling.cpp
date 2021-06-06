#include "file_handling.hpp"

void file_handling::readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void file_handling::writeFile(fs::FS &fs, const char * path, std::string message){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message.c_str())){
    Serial.println("- file written");
    } else {
    Serial.println("- write failed");
  }
  file.close();
}

void file_handling::deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\r\n", path);
  if(fs.remove(path)){
    Serial.println("- file deleted");
    } else {
    Serial.println("- delete failed");
  }
}

void file_handling::appendFile(fs::FS &fs, const char * path, std::string message){
  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("- failed to open file for appending");
    return;
  }
  if(file.print(message.c_str())){
    Serial.println("- message appended");
    } else {
    Serial.println("- append failed");
  }
  file.close();
}
