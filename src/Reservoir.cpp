#include "Reservoir.h"

/**
 * @brief Class representing a reservoir.
 */
Reservoir::Reservoir(const std::string &name, const std::string &municipality, const std::string &id, const std::string &code, int maxDelivery) {
    name_ = name;
    id_ = id;
    code_ = code;
    municipality_ = municipality;
    maxDelivery_ = maxDelivery;

    /**
    * @brief Constructor for the Reservoir class.
    *
    * @param name Name of the reservoir.
    * @param municipality Municipality where the reservoir is located.
    * @param id ID of the reservoir.
    * @param code Code of the reservoir.
    * @param maxDelivery Maximum delivery capacity of the reservoir.
    */
}

/**
    * @brief Get the name of the reservoir.
    *
    * @return Name of the reservoir.
    */
std::string Reservoir::getName(){
    return name_;
}

/**
     * @brief Get the ID of the reservoir.
     *
     * @return ID of the reservoir.
     */
std::string Reservoir::getId(){
    return id_;
}

/**
    * @brief Get  code of the reservoir.
    *
    * @return Code of the reservoir.
    */
std::string Reservoir::getCode(){
    return code_;
}

/**
    * @brief Get the municipality where the reservoir is located.
    *
    * @return Municipality where the reservoir is located.
    */
std::string Reservoir::getMunicipality(){
    return municipality_;
}

/**
    * @brief Get the maximum delivery capacity of the reservoir.
    *
    * @return Maximum delivery capacity of the reservoir.
    */
int Reservoir::getMaxDelivery() const {
    return maxDelivery_;
}

/**
    * @brief Default constructor for the Reservoir class.
    */
Reservoir::Reservoir() = default;


