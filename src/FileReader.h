#ifndef PROJECT_DA_FILEREADER_H
#define PROJECT_DA_FILEREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

class FileReader{
private:
    std::ifstream file_;
    std::vector<std::vector<std::string>> data_;

public:
    explicit FileReader(const std::string &fName);
    std::vector<std::vector<std::string>> getData();
};

#endif //PROJECT_DA_FILEREADER_H
