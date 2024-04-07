#include "Station.h"

/**
 * @brief Class representing a station.
 */
Station::Station(const std::string &id, const std::string &code){
    id_ = id;
    code_ = code;

    /**
     * @brief Constructor for the Station class.
     *
     * @param id ID of the station.
     * @param code Code of the station.
     */
}

/**
    * @brief Get the ID of the station.
    *
    * @return ID of the station.
    */
std::string Station::getId(){
    return id_;
}

/**
    * @brief Get the code of the station.
    *
    * @return Code of the station.
    */
std::string Station::getCode() {
    return code_;
}

/**
    * @brief Default constructor for the Station class.
    */
Station::Station() = default;
