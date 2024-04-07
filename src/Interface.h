#ifndef PROJECT_DA_INTERFACE_H
#define PROJECT_DA_INTERFACE_H

#include <iostream>
#include <codecvt>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <locale>
#include <thread>
#include <stack>
#include "Management.h"

class Interface {
public:
    explicit Interface();

    void run();


private:

    //----------------- COLOR SCHEMES --------------------//
    std::wstring bg_light_red = L"\x1b[101m";
    std::wstring end_bg = L"\x1b[0m";
    std::wstring italic = L"\x1b[3m";
    std::wstring end_italic = L"\x1b[0m";
    std::wstring red = L"\033[31m";
    std::wstring blue = L"\033[34m";
    std::wstring bold = L"\033[1m";
    std::wstring underline = L"\033[4m";;
    std::wstring end_effect = L"\033[0m";
    //----------------------------------------------------//

    std::shared_ptr<Management> man;

    std::thread manThread;

    int location = 0;
    int page = 0;
    int elements_per_page = 10;

    unsigned long selected = 0;
    unsigned long selected_in_page = 0;

    unsigned int user_input = 0;

    bool write_mode = false;
    bool table_mode = false;
    bool is_done = false;

    std::string city_analised;
    std::string city_analised_code;
    std::wstring reservoir_analised_code;
    std::wstring directory;
    std::vector<std::wstring> directories;
    std::wstring write;
    std::wstring write_default =  italic + L"  You can write here  " + end_italic;
    std::wstring selected_dataSet = L"none";

    std::stack<int> earlier_locations;

    std::vector<std::wstring> filteredStringVector;
    std::unordered_map<std::string, std::string> filteredWstringPairsVector;
    std::unordered_map<std::string, std::string> citiesStringMap;
    std::unordered_map<std::string, std::string> citiesWaterDeliveredMap;
    std::unordered_map<std::string, std::string> citiesWaterDeliveredMapWithChanges;
    std::vector<std::wstring> reservoirCodeVector;
    std::vector<std::wstring> stationCodeVector;
    std::vector<std::wstring> selectedReservoirCode;
    std::vector<std::wstring> selectedPumpingCode;
    std::unordered_map<int, int> locationHasTable = {
            {7, 1}, {12, 1}, {14, 1}
    };
    std::unordered_map<int, int> capOfWrite = {
            {7, 30}, {12, 4}, {14, 5}
    };
    std::unordered_map<int, int> locationOfCitySearch = {
            {7, 1}
    };
    std::unordered_map<int, int> locationOfReservoirSearch = {
            {12, 1}
    };
    std::unordered_map<int, int> locationOfStationSearch = {
            {14, 1}
    };
    std::unordered_map<int, int> locationWithTable = {
            {7, 0}, {12, 1}, {14, 2}
    };

    std::vector<std::vector<std::string>> water_needs;

    std::vector<std::vector<std::wstring>> options{
            {
                L"Choose DataSet",
                L"Credits",
                L"Quit"
            },
            {
                L"Change Name",
                L"Main Menu"
            },
            {
                L"Portugal DataSet",
                L"Madeira DataSet",
                L"Main Menu"
            },
            {
                L"Portugal DataSet " + bold + blue + L" <- ACTIVE" + end_effect,
                L"Madeira DataSet",
                L"Proceed"
            },
            {
                L"Portugal DataSet",
                L"Madeira DataSet " + bold + blue + L" <- ACTIVE" + end_effect,
                L"Proceed"
            },
            {
                L"Basic Service Metrics",
                L"Reliability and Sensitivity to Failures",
                L"Choose DataSet",
                L"Quit"
            },
            {
                L"Max Amount of Water",
                L"Cities in Deficit",
                L"Dummy2",
                L"Main Menu"
            },
            {
                L"Search for a City",
                L"Total",
                L"Back",
                L"Main Menu"
            },
            {
                L"Back",
                L"Main Menu"
            },
            {
                L"Back",
                L"Main Menu"
            },
            {
                L"Back",
                L"Main Menu"
            },
            {
                L"Reservoir Out of Commission",
                L"Pumping Stations in Maintenance",
                L"Pipeline Failures",
                L"Main Menu"
            },
            {
                L"Search for a Reservoir",
                L"Back",
                L"Main Menu"
            },
            {
                L"Add One More Reservoir",
                L"Back",
                L"Main Menu"
            },
            {
                L"Search for a Pumping Station",
                L"Back",
                L"Main Menu"
            },
            {
                L"Add One More Pumping Station",
                L"Back",
                L"Main Menu"
            },
    };

    std::vector<std::wstring> helpers{
            L"  You can use 'up arrow', 'down arrow', and 'ENTER' to select the options",
            L"  You can use 'tab' to change to the table, and 'ENTER' to select one",
            L"  You can use 'n' and 'p' to go to the next and previous page of the table respectively",
            L"  Choose your DataSet",
            L"  You selected Portugal DataSet",
            L"  You selected Madeira DataSet"
    };

    std::vector<std::wstring> waterSupplyManagement = {
            L" --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- ",
            L"| ██     ██  █████  ████████ ███████ ██████      ███████ ██    ██ ██████  ██████  ██     ██    ██     ███    ███  █████  ███    ██  █████   ██████  ███████ ███    ███ ███████ ███    ██ ████████   |",
            L"| ██     ██ ██   ██    ██    ██      ██   ██     ██      ██    ██ ██   ██ ██   ██ ██      ██  ██      ████  ████ ██   ██ ████   ██ ██   ██ ██       ██      ████  ████ ██      ████   ██    ██      |",
            L"| ██  █  ██ ███████    ██    █████   ██████      ███████ ██    ██ ██████  ██████  ██       ████       ██ ████ ██ ███████ ██ ██  ██ ███████ ██   ███ █████   ██ ████ ██ █████   ██ ██  ██    ██      |",
            L"| ██ ███ ██ ██   ██    ██    ██      ██   ██          ██ ██    ██ ██      ██      ██        ██        ██  ██  ██ ██   ██ ██  ██ ██ ██   ██ ██    ██ ██      ██  ██  ██ ██      ██  ██ ██    ██      |",
            L"|  ███ ███  ██   ██    ██    ███████ ██   ██     ███████  ██████  ██      ██      ██████    ██        ██      ██ ██   ██ ██   ████ ██   ██  ██████  ███████ ██      ██ ███████ ██   ████    ██      |",
            L" --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- ",
    };

    void inputer();

    void basicInputResponse(unsigned int user_in);

    void inputResponseInWriteMode(wchar_t user_in);

    void refreshDirectories();

    static void stackClear(std::stack<int> &s);

    void enterInputHandler(int loc, unsigned long sel, bool back, bool main_menu, bool main_menu2);

    void writeOptionDefaulterCity();

    std::unordered_map<std::string, std::string> filterCitySearch(const std::unordered_map<std::string, std::string> &um);

    static std::unordered_map<std::string, std::string>

    stringifyUMStringClass(const std::unordered_map<std::string, City> &um);

    static std::wstring smooth_string(const std::wstring &w);

    void initializeTable();


    void tableModeCleaner(std::unordered_map<std::string, std::string> um);

    static std::vector<std::wstring> getCodeVector(const std::unordered_map<std::string, Reservoir> &um);

    void writeOptionDefaulterReservoir();

    std::vector<std::wstring> filterReservoirSearch(const std::vector<std::wstring> &um);

    void tableModeCleaner2(std::vector<std::wstring> um);

    static std::vector<std::wstring> getCodeStation(const std::unordered_map<std::string, Station> &um);

    void writeOptionDefaulterStation();
};

#endif //PROJECT_DA_INTERFACE_H
