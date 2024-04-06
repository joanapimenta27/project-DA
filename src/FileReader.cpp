#include "FileReader.h"

std::string trim(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();

    while (start < end && std::isspace(str[start])) {
        start++;
    }

    while (end > start && std::isspace(str[end - 1])) {
        end--;
    }

    return str.substr(start, end - start);
}



FileReader::FileReader(const std::string &fName) {
    file_.open(fName);
}

std::vector<std::vector<std::string>> FileReader::getData() {
    std::string line;
    std::getline(file_, line);
    while (std::getline(file_, line)) {
        std::istringstream iss(line);
        std::string value;
        std::vector<std::string> v;
        while (std::getline(iss, value, ',')) {
            value = trim(value);
            v.push_back(value);
        }
        data_.push_back(v);
    }
    return data_;
}
