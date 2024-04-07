#include "FileReader.h"

/**
 * @brief Trims whitespaces at the beginning and end of a string.
 *
 * @param str String to be trimmed.
 * @return Trimmed string.
 *
 * @complexity The time complexity of this function is O(n), where n is the length of the input string.
 * This is because the function iterates over the string once from both ends to find the start and end positions
 * of characters that are not spaces. The substr operation at the end has a time complexity of O(end - start),
 * which is proportional to the length of the trimmed substring.
 */
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


/**
 * @brief Class for reading data from a file.
 */
FileReader::FileReader(const std::string &fName) {
    file_.open(fName);
    /**
     * @brief Constructor for the FileReader class.
     *
     * @param fName File name or path to be opened.
     */
}

/**
 * @brief Reads data from the file and returns it as a vector of vectors of strings.
 *
 * Each inner vector represents a line from the file, and each string represents a field separated by commas.
 * Whitespaces at the beginning and end are removed from each field.
 *
 * @return Parsed data from the file.
 *
 * @complexity The time complexity of this function is O(n*m), where n is the number of lines in the file
 * and m is the average number of fields per line. Each line is read from the file, split into fields,
 * and trimmed to remove whitespaces. Therefore, the overall time complexity is proportional to the
 * total number of characters read from the file.
 */
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
