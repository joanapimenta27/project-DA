#include "City.h"

/**
 * @brief Class that represents a city.
 */
City::City(const std::string &name, const std::string &id, const std::string &code, float demand, int population) {
    name_ = name;
    id_ = id;
    code_ = code;
    demand_ = demand;
    population_ = population;
    /**
    * @brief Constructor for the City class.
    *
    * @param name Name of the city.
    * @param id ID of the city.
    * @param code Code of the city.
    * @param demand Water demand of the city.
    * @param population Population of the city.
    */
}

/**
    * @brief Get the name of the city.
    *
    * @return Name of the city.
    */
std::string City::getName() const {
    return name_;
}

/**
 * @brief Get ID of the city.
 *
 * @return ID of the city.
 */
std::string City::getId() {
    return id_;
}

/**
    * @brief Get code of the city.
    *
    * @return Code of the city.
    */
std::string City::getCode(){
    return code_;
}

/**
    * @brief Get the water demand of the city.
    *
    * @return Water demand of the city.
    */
float City::getDemand() const{
    return demand_;
}

/**
    * @brief Get the population of the city.
    *
    * @return Population of the city.
    */
int City::getPopulation() const{
    return population_;
}

/**
     * @brief Default constructor for the City class.
     */
City::City() = default;