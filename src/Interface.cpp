#include <cstring>
#include <utility>
#include "Interface.h"
#include "Printer.h"

Interface::Interface()= default;

void Interface::refreshDirectories() {
    directories.clear();
    directories.emplace_back(L"");
    directories.emplace_back(L"Credits");
    directories.emplace_back(L"Choose DataSet");
    directories.emplace_back(L"Choose DataSet > Portugal DataSet");
    directories.emplace_back(L"Choose DataSet > Madeira DataSet");
    directories.emplace_back(L"");
    directories.emplace_back(L"Basic Service Metrics");
    directories.emplace_back(L"Basic Service Metrics > Max Amount of Water");
    directories.emplace_back(L"Basic Service Metrics > Max Amount of Water > Total");
    directories.emplace_back(L"Basic Service Metrics > Max Amount of Water > " + converter.from_bytes(city_analised));
    directories.emplace_back(L"Basic Service Metrics > Cities in Deficit");
    directories.emplace_back(L"Reliability and Sensitivity to Failures");
    directories.emplace_back(L"Reliability and Sensitivity to Failures > Reservoir Out of Commission");
    std::wstring res_prep;
    for (int i = 0; i < selectedReservoirCode.size(); i ++){
        res_prep.clear();
        if (i == 0){
            res_prep.append(selectedReservoirCode[i]);
        }
        else if (i != selectedReservoirCode.size() - 1){
            res_prep.append(L", " + selectedReservoirCode[i]);
        }
        else{
            res_prep.append(L" and " + selectedReservoirCode[i]);
        }
    }
    directories.emplace_back(L"Reliability and Sensitivity to Failures > Reservoir " + res_prep + L" Out of Commission");
    directories.emplace_back(L"Reliability and Sensitivity to Failures > Pumping Stations in Maintenance");
    res_prep.clear();
    for (int i = 0; i < selectedPumpingCode.size(); i ++){
        if (i == 0){
            res_prep.append(selectedPumpingCode[i]);
        }
        else if (i != selectedPumpingCode.size() - 1){
            res_prep.append(L", " + selectedPumpingCode[i]);
        }
        else{
            res_prep.append(L" and " + selectedPumpingCode[i]);
        }
    }
    directories.emplace_back(L"Reliability and Sensitivity to Failures > Pumping Stations" + res_prep + L" in Maintenance");
    directories.emplace_back(L"Reliability and Sensitivity to Failures > Pipeline Failures");
    res_prep.clear();
    for (int i = 0; i < selectedPipeCode.size(); i ++){
        if (i == 0){
            res_prep.append(selectedPipeCode[i]);
        }
        else if (i != selectedPipeCode.size() - 1){
            res_prep.append(L", " + selectedPipeCode[i]);
        }
        else{
            res_prep.append(L" and " + selectedPipeCode[i]);
        }
    }
    directories.emplace_back(L"Reliability and Sensitivity to Failures > Pipe " + res_prep + L" Failures");
}

void Interface::stackClear(std::stack<int> &s){
    while (!s.empty()){
        s.pop();
    }
}

std::wstring Interface::smooth_string(const std::wstring& w){
    std::wstring sw;
    for (wchar_t c : w) {
        if (!iswspace(c)) {
            c = std::tolower(c);
            switch (c) {
                case L'á':
                case L'à':
                case L'â':
                case L'ä':
                case L'ã':
                case L'Á':
                case L'À':
                case L'Ã':
                case L'Â':
                case L'Ä':
                    sw += L'a';
                    break;
                case L'é':
                case L'è':
                case L'ê':
                case L'ë':
                case L'ẽ':
                case L'Ë':
                case L'É':
                case L'È':
                case L'Ê':
                case L'Ẽ':
                    sw += L'e';
                    break;
                case L'í':
                case L'ì':
                case L'î':
                case L'ï':
                case L'ĩ':
                case L'Í':
                case L'Ì':
                case L'Î':
                case L'Ĩ':
                case L'Ï':
                    sw += L'i';
                    break;
                case L'ó':
                case L'ò':
                case L'ô':
                case L'ö':
                case L'õ':
                case L'Ó':
                case L'Ò':
                case L'Õ':
                case L'Ô':
                case L'Ö':
                    sw += L'o';
                    break;
                case L'ú':
                case L'ù':
                case L'û':
                case L'ü':
                case L'ũ':
                case L'Ú':
                case L'Ù':
                case L'Ũ':
                case L'Û':
                case L'Ü':
                    sw += L'u';
                    break;
                case L'ç':
                    sw += L'c';
                default:
                    if (!ispunct(c)) {
                        sw += c;
                    }
            }
        }
    }
    return sw;
}

void Interface::initializeTable(){
    switch(locationWithTable[location]){
        case 0:
            filteredWstringPairsVector = citiesStringMap;
            break;
        case 1:
            filteredStringVector = reservoirCodeVector;
            break;
        case 2:
            filteredStringVector = stationCodeVector;
            break;
        case 3:
            filteredStringVector = pipeCodeVector;
            break;
    }
}

std::unordered_map<std::string, std::string> Interface::stringifyUMStringClass(const std::unordered_map<std::string, City>& um){
    std::unordered_map<std::string, std::string> res;
    for (const auto& p : um){
        res[p.first] = p.second.getName();
    }
    return res;
}

std::vector<std::wstring> Interface::getCodeVector(const std::unordered_map<std::string, Reservoir>& um){
    std::vector<std::wstring> res;
for (const auto& p : um){
        res.push_back(converter.from_bytes(p.first));
    }
    return res;
}

std::vector<std::wstring> Interface::getCodeStation(const std::unordered_map<std::string, Station>& um){
    std::vector<std::wstring> res;
    for (const auto& p : um){
        res.push_back(converter.from_bytes(p.first));
    }
    return res;
}

std::vector<std::wstring> Interface::getCodePipes(const std::unordered_map<std::string, std::string>& um){
    std::vector<std::wstring> res;
    for (const auto& p : um){
        std::string o = stringDivider(converter.from_bytes(p.first), 0, '|');
        std::string d = stringDivider(converter.from_bytes(p.first), 1, '|');
        if (d != "source" && o !="sink"){
            res.push_back(converter.from_bytes(p.first));
        }
    }
    return res;
}

std::unordered_map<std::string, std::string> Interface::filterCitySearch(const std::unordered_map<std::string, std::string>& um){
    std::unordered_map<std::string, std::string> res;
    if (write.empty() || write == write_default){
        res = citiesStringMap;
    }
    for (const auto& p : um){
        if (smooth_string(converter.from_bytes(p.first)).substr(0, smooth_string(write).size()) == smooth_string(write)){
            res[p.first] = p.second;
        }
        if (smooth_string(converter.from_bytes(p.second)).substr(0, smooth_string(write).size()) == smooth_string(write)){
            res[p.first] = p.second;
        }
    }
    return res;
}

std::vector<std::wstring> Interface::filterReservoirSearch(const std::vector<std::wstring>& um){
    std::vector<std::wstring> res;
    if (write.empty() || write == write_default){
        res = reservoirCodeVector;
    }
    for (const auto& p : um){
        if (smooth_string(p).substr(0, smooth_string(write).size()) == smooth_string(write)){
            res.push_back(p);
        }
    }
    return res;
}

std::vector<std::wstring> Interface::filterStationSearch(const std::vector<std::wstring>& um){
    std::vector<std::wstring> res;
    if (write.empty() || write == write_default){
        res = stationCodeVector;
    }
    for (const auto& p : um){
        if (smooth_string(p).substr(0, smooth_string(write).size()) == smooth_string(write)){
            res.push_back(p);
        }
    }
    return res;
}

std::vector<std::wstring> Interface::filterPipeSearch(const std::vector<std::wstring>& um){
    std::vector<std::wstring> res;
    if (write.empty() || write == write_default){
        res = pipeCodeVector;
    }
    for (const auto& p : um){
        if (smooth_string(p).substr(0, smooth_string(write).size()) == smooth_string(write)){
            res.push_back(p);
        }
    }
    return res;
}

void Interface::writeOptionDefaulterCity(){
    if (!write.empty() && write != write_default) {
        options[location][0] = L"Searching for: " + bold + write + end_effect;
    } else {
        options[location][0] = L"Search for a City";
    }
    if (selected == 0 && write.empty() && !table_mode){
        options[location][0] = underline + bold + red + L"Search for a City -> " + end_effect + L"  " + italic + write_default + end_italic;
        write_mode = true;
    }
    else if (selected == 0 && !table_mode){
        options[location][0] = underline + bold + red + L"Search for a City -> " + end_effect + L"  " + write;
        write_mode = true;
    }
}

void Interface::writeOptionDefaulterReservoir(){
    if (!write.empty() && write != write_default) {
        options[location][0] = L"Searching for: " + bold + write + end_effect;
    } else {
        options[location][0] = L"Search for a Reservoir";
    }
    if (selected == 0 && write.empty() && !table_mode){
        options[location][0] = underline + bold + red + L"Search for a Reservoir -> " + end_effect + L"  " + italic + write_default + end_italic;
        write_mode = true;
    }
    else if (selected == 0 && !table_mode){
        options[location][0] = underline + bold + red + L"Search for a Reservoir -> " + end_effect + L"  " + write;
        write_mode = true;
    }
}

void Interface::writeOptionDefaulterStation(){
    if (!write.empty() && write != write_default) {
        options[location][0] = L"Searching for: " + bold + write + end_effect;
    } else {
        options[location][0] = L"Search for a Pumping Station";
    }
    if (selected == 0 && write.empty() && !table_mode){
        options[location][0] = underline + bold + red + L"Search for a Pumping Station -> " + end_effect + L"  " + italic + write_default + end_italic;
        write_mode = true;
    }
    else if (selected == 0 && !table_mode){
        options[location][0] = underline + bold + red + L"Search for a Pumping Station -> " + end_effect + L"  " + write;
        write_mode = true;
    }
}

void Interface::writeOptionDefaulterPipe(){
    if (!write.empty() && write != write_default) {
        options[location][0] = L"Searching for: " + bold + write + end_effect;
    } else {
        options[location][0] = L"Search for a Pipe";
    }
    if (selected == 0 && write.empty() && !table_mode){
        options[location][0] = underline + bold + red + L"Search for a Pipe -> " + end_effect + L"  " + italic + write_default + end_italic;
        write_mode = true;
    }
    else if (selected == 0 && !table_mode){
        options[location][0] = underline + bold + red + L"Search for a Pipe -> " + end_effect + L"  " + write;
        write_mode = true;
    }
}

void Interface::enterInputHandler(int loc, unsigned long sel, bool back, bool main_menu, bool main_menu2){
    refreshDirectories();
    if (back){
        if(loc == 0){
            location = earlier_locations.top();
        }
        else{
            location = loc;
        }
        earlier_locations.pop();
    }
    else if (main_menu){
        location = 0;
        stackClear(earlier_locations);
    }
    else if (main_menu2){
        location = 5;
        stackClear(earlier_locations);
    }
    else{
        earlier_locations.push(location);
        location = loc;
    }
    directory = directories[location];
    selected = sel;

}

void Interface::inputResponseInWriteMode(wchar_t user_in){
    if ((isalpha(user_in) || isalnum(user_in) || (user_in >= 128 && user_in <= 255) || (ispunct(user_in)))) {
        if (write == write_default) {
            write = L"";
        }
        write += user_in;
    }
    if (user_in == 32 && write != write_default) {
        write += L" ";
    }
    if ((user_in == 8 || user_in == 127) && write != write_default) {
        if (!write.empty()) {
            write.pop_back();
        }
    }
    if (write.empty()) {
        write = write_default;
    }
    if (write.size() > capOfWrite[location] && write != write_default) {
        write.pop_back();
    }
    if (locationOfCitySearch[location]){
        filteredWstringPairsVector = filterCitySearch(citiesStringMap);
        page = 0;
    }
    if (locationOfReservoirSearch[location]){
        filteredStringVector = filterReservoirSearch(reservoirCodeVector);
        page = 0;
    }
    if (locationOfStationSearch[location]){
        filteredStringVector = filterStationSearch(stationCodeVector);
        page = 0;
    }
    if (locationOfPipeSearch[location]){
        filteredStringVector = filterPipeSearch(pipeCodeVector);
        page = 0;
    }
}

void Interface::tableModeCleaner(std::unordered_map<std::string, std::string> um){
    write = write_default;
    selected_in_page = 0;
    selected = 0;
    page = 0;
    table_mode = !table_mode;
    filteredWstringPairsVector = std::move(um);
}

void Interface::tableModeCleaner2(std::vector<std::wstring> ws){
    write = write_default;
    selected_in_page = 0;
    selected = 0;
    page = 0;
    table_mode = !table_mode;
    filteredStringVector = std::move(ws);
}

void Interface::inputer(){
    user_input = getwchar();
    if (!write_mode) {
        if (user_input == 27) {
            user_input = getwchar();
            user_input = getwchar();
        }
        basicInputResponse(user_input);
    }
    else{
        if (user_input == 27){
            user_input = getwchar();
            user_input = getwchar();
            write_mode = false;
            basicInputResponse(user_input);
        }
        else if ((user_input == '\n' || user_input == '\t') && locationHasTable[location]){
            write_mode = false;
            table_mode = true;
        }
        else{
            inputResponseInWriteMode(user_input);
        }
    }
}

void Interface::basicInputResponse(unsigned int user_in) {
    if (locationHasTable[location]){
        if (user_in == '\t'){
            table_mode = !table_mode;
            selected = 0;
            selected_in_page = 0;
        }
        if (user_in == 'n'){
            page ++;
            selected_in_page = 0;
            if (page > filteredWstringPairsVector.size()/elements_per_page && locationWithTable[location] == 0) {
                page--;
            }
            else if (page > filteredStringVector.size()/elements_per_page && locationWithTable[location] != 0){
                page--;
            }
        }
        if (user_in == 'p'){
            page --;
            selected_in_page = 0;
            if (page < 0){
                page ++;
            }
        }
    }
    if (!table_mode){
        if (user_in == 'A') {
            if (selected > 0) { selected -= 1; }
            else { selected = options[location].size() - 1; }
        }
        if (user_in == 'B') {
            selected ++;
            if (selected >= options[location].size()) {selected = 0;}
        }
    }
    else{
        if (user_in == 'A') {
            if (selected_in_page > 0) { selected_in_page -= 1; }
            else if (page == filteredWstringPairsVector.size()/elements_per_page && locationWithTable[location] == 0){
                selected_in_page = filteredWstringPairsVector.size()%elements_per_page - 1;
            }
            else if (page == filteredStringVector.size()/elements_per_page && locationWithTable[location] != 0){
                selected_in_page = filteredStringVector.size()%elements_per_page - 1;
            }
            else {
                selected_in_page = elements_per_page - 1;
            }
        }
        if (user_in == 'B') {
            selected_in_page ++;
            if (page == filteredWstringPairsVector.size()/elements_per_page && locationWithTable[location] == 0){
                if (selected_in_page > filteredWstringPairsVector.size()%elements_per_page - 1){
                    selected_in_page = 0;
                }
            }
            else if (page == filteredStringVector.size()/elements_per_page && locationWithTable[location] != 0){
                if (selected_in_page > filteredStringVector.size()%elements_per_page - 1){
                    selected_in_page = 0;
                }
            }
            else{
                if (selected_in_page > elements_per_page - 1){
                    selected_in_page = 0;
                }
            }
        }
    }
    if (user_in == 'q') {
        location = -1;
    }
    //========================= ENTER INPUTS ===========================//
    if (user_in == '\n') {
        switch (location) {
            case 0:       //============================= MAIN MENU ===============================//
                switch (selected) {
                    case 0:
                        enterInputHandler(2, 0, false, false, false);
                        break;
                    case 1:
                        enterInputHandler(1, 0, false, false, false);
                        break;
                    case 2:
                        location = -1;
                        break;
                }
                break;
            case 1:    //Credits
                switch (selected){
                    case 0:
                        break;
                    case 1:
                        enterInputHandler(0, 0, false, true, false);
                }
            case 2:  //Choose DataSet none
                switch (selected){
                    case 0:
                        selected_dataSet = L"big";
                        enterInputHandler(3, 0, false, false, false);
                        break;
                    case 1:
                        selected_dataSet = L"small";
                        enterInputHandler(4, 1, false, false, false);
                        break;
                    case 2:
                        enterInputHandler(0, 0, false, true, false);
                        break;
                }
                break;
            case 3: // Choose DataSet big
                switch (selected){
                    case 0:
                        selected_dataSet = L"none";
                        is_done = false;
                        enterInputHandler(2, 0, false, false, false);
                        break;
                    case 1:
                        selected_dataSet = L"small";
                        is_done = false;
                        enterInputHandler(4, 1, false, false, false);
                        break;
                    case 2:
                        enterInputHandler(0, 0, false, false, true);
                        break;
                }
                break;
            case 4:    //Choose dataSet small
                switch (selected){
                    case 0:
                        selected_dataSet = L"big";
                        is_done = false;
                        enterInputHandler(3, 0, false, false, false);
                        break;
                    case 1:
                        selected_dataSet = L"none";
                        is_done = false;
                        enterInputHandler(2, 1, false, false, false);
                        break;
                    case 2:
                        enterInputHandler(0, 0, false, false, true);
                        break;
                }
                break;
            case 5:   //M Main Menu2
                switch (selected){
                    case 0:
                        enterInputHandler(6, 0, false, false, false);
                        break;
                    case 1:
                        enterInputHandler(11, 0, false, false, false);
                        break;
                    case 2:
                        if (selected_dataSet == L"big"){
                            enterInputHandler(3, 0, false, false, false);
                        }
                        else{
                            enterInputHandler(4, 1, false, false, false);
                        }
                        break;
                    case 3:
                        location = -1;
                }
                break;
            case 6:    // Basic Service Metrics
                switch (selected){
                    case 0:
                        enterInputHandler(7, 0, false, false, false);
                        initializeTable();
                        break;
                    case 1:
                        enterInputHandler(10, 0, false, false, false);
                        break;
                    case 2:
                        enterInputHandler(0, 0, true, false, false);
                        break;
                    case 3:
                        enterInputHandler(0, 0, false, false, true);
                        break;
                }
                break;
            case 7:
                if (!table_mode) {
                    switch (selected) {
                        case 0:
                            break;
                        case 1:
                            enterInputHandler(8, 0, false, false, false);
                            break;
                        case 2:
                            enterInputHandler(0, 0, true, false, false);
                            break;
                        case 3:
                            enterInputHandler(0, 0, false, false, true);
                            break;
                    }
                    break;
                }
                else{
                    auto it = filteredWstringPairsVector.begin();
                    std::advance(it, page * elements_per_page + selected_in_page);
                    city_analised = it->second;
                    city_analised_code = it->first;
                    enterInputHandler(9, 0, false, false, false);
                    tableModeCleaner(citiesStringMap);
                }
                break;
            case 8:
                switch (selected){
                    case 0:
                        enterInputHandler(0, 1, true, false, false);
                        break;
                    case 1:
                        enterInputHandler(0, 0, false, false, true);
                        break;
                }
                break;
            case 9:
                switch (selected){
                    case 0:
                        enterInputHandler(0, 0, true, false, false);
                        break;
                    case 1:
                        enterInputHandler(0, 0, false, false, true);
                        break;
                }
                break;
            case 10:
                switch (selected){
                    case 0:
                        enterInputHandler(0, 1, true, false, false);
                        break;
                    case 1:
                        enterInputHandler(0, 0, false, false, true);
                        break;
                }
                break;
            case 11:
                switch (selected){
                    case 0:
                        enterInputHandler(12, 0, false, false, false);
                        initializeTable();
                        break;
                    case 1:
                        enterInputHandler(14, 0, false, false, false);
                        initializeTable();
                        break;
                    case 2:
                        enterInputHandler(16, 0, false, false, false);
                        initializeTable();
                        break;
                    case 3:
                        enterInputHandler(0, 0, false, false, true);
                        break;
                }
                break;
            case 12:
                if (!table_mode) {
                    switch (selected) {
                        case 0:
                            break;
                        case 1:
                            enterInputHandler(11, 0, true, false, false);
                            break;
                        case 2:
                            enterInputHandler(0, 0, false, false, true);
                            break;
                    }
                    break;
                }
                else{
                    selectedReservoirCode.push_back(filteredStringVector[page * elements_per_page + selected_in_page]);
                    enterInputHandler(13, 0, false, false, false);
                    tableModeCleaner2(reservoirCodeVector);
                    citiesWaterDeliveredMapWithChanges = man->checkWaterNeedsReservoir(selectedReservoirCode);
                }
                break;
            case 13:
                switch (selected){
                    case 0:
                        enterInputHandler(12, 0, false, false, false);
                        initializeTable();
                        break;
                    case 1:
                        enterInputHandler(12, 0, true, false, false);
                        selectedReservoirCode.clear();
                        break;
                    case 2:
                        enterInputHandler(0, 0, false, false, true);
                        selectedReservoirCode.clear();
                        break;
                }
                break;
            case 14:
                if (!table_mode) {
                    switch (selected) {
                        case 0:
                            break;
                        case 1:
                            enterInputHandler(11, 0, true, false, false);
                            break;
                        case 2:
                            enterInputHandler(0, 0, false, false, true);
                            break;
                    }
                    break;
                }
                else{
                    selectedPumpingCode.push_back(filteredStringVector[page * elements_per_page + selected_in_page]);
                    enterInputHandler(15, 0, false, false, false);
                    tableModeCleaner2(stationCodeVector);
                    citiesWaterDeliveredMapWithChanges = man->checkWaterNeedsPumps(selectedPumpingCode);
                }
                break;
            case 15:
                switch (selected){
                    case 0:
                        enterInputHandler(14, 0, false, false, false);
                        initializeTable();
                        break;
                    case 1:
                        enterInputHandler(14, 0, true, false, false);
                        selectedPumpingCode.clear();
                        break;
                    case 2:
                        enterInputHandler(0, 0, false, false, true);
                        selectedPumpingCode.clear();
                        break;
                }
                break;
            case 16:
                if (!table_mode) {
                    switch (selected) {
                        case 0:
                            break;
                        case 1:
                            enterInputHandler(11, 0, true, false, false);
                            break;
                        case 2:
                            enterInputHandler(0, 0, false, false, true);
                            break;
                    }
                    break;
                }
                else{
                    selectedPipeCode.push_back(filteredStringVector[page * elements_per_page + selected_in_page]);
                    enterInputHandler(17, 0, false, false, false);
                    tableModeCleaner2(pipeCodeVector);
                    citiesWaterDeliveredMapWithChanges = man->checkWaterNeedsPipes(selectedPipeCode);
                }
                break;
            case 17:
                switch (selected){
                    case 0:
                        enterInputHandler(16, 0, false, false, false);
                        initializeTable();
                        break;
                    case 1:
                        enterInputHandler(16, 0, true, false, false);
                        selectedPipeCode.clear();
                        break;
                    case 2:
                        enterInputHandler(0, 0, false, false, true);
                        selectedPipeCode.clear();
                        break;
                }
                break;
        }
    }
}

void Interface::run(){
    setlocale(LC_CTYPE, "en_US.UTF-8"); // enconding to UTF-8 for extended characters such as "ç"

    struct termios oldt{}, newt{};
    tcgetattr(STDIN_FILENO, &oldt); // Get the current terminal settings

    newt = oldt; // Copy the current settings to the new settings

    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode (line buffering) and echoing

    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Set the new settings


    while(location != -1){

        system("clear");

        switch (location) {
            case 0:
                printBoldTitle(waterSupplyManagement);
                printOptions(options[location], selected, table_mode);
                printHelper(helpers, {0});
                inputer();
                break;

            case 1:
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printHelper(helpers, {0});
                inputer();
                break;

            case 2:
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printHelper(helpers, {3});
                inputer();
                break;

            case 3:
                if(!is_done) {
                    man = std::make_shared<Management>(1);
                    citiesStringMap = stringifyUMStringClass(man->getCities());
                    for (const auto& city : man->getCities()){
                        citiesWaterDeliveredMap.insert({city.first, std::to_string(man->maxFlow(*man->getWaterNetwork(),city.first))});                    }
                    reservoirCodeVector = getCodeVector(*man->getReservoirs());
                    stationCodeVector = getCodeStation(*man->getStations());
                    pipeCodeVector = getCodePipes(man->getEdgesFlow());
                    is_done = true;
                }

                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printHelper(helpers, {4});
                inputer();
                break;

            case 4:
                if(!is_done) {
                    man = std::make_shared<Management>(0);
                    citiesStringMap = stringifyUMStringClass(man->getCities());
                    for (const auto& city : man->getCities()){
                        citiesWaterDeliveredMap.insert({city.first, std::to_string(man->maxFlow(*man->getWaterNetwork(),city.first))});
                    }
                    reservoirCodeVector = getCodeVector(*man->getReservoirs());
                    stationCodeVector = getCodeStation(*man->getStations());
                    pipeCodeVector = getCodePipes(man->getEdgesFlow());
                    is_done = true;
                }

                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printHelper(helpers, {5});
                inputer();
                break;

            case 5:
                printBoldTitle(waterSupplyManagement);
                printOptions(options[location], selected, table_mode);
                printHelper(helpers, {0});
                inputer();
                break;

            case 6:
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printHelper(helpers, {0});
                inputer();
                break;

            case 7:
                writeOptionDefaulterCity();
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printDoubleTable(filteredWstringPairsVector, page, elements_per_page, selected_in_page, table_mode);
                printHelper(helpers, {1, 2});
                inputer();
                break;

            case 8:
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printMonoInfo(bold + std::to_wstring(man->maxFlow(*man->getWaterNetwork(),"sink")) + end_effect);
                printHelper(helpers, {0});
                inputer();
                break;

            case 9:
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printMonoInfo(bold + std::to_wstring(man->maxFlow(*man->getWaterNetwork(),city_analised_code)) + end_effect);
                printHelper(helpers, {0});
                inputer();
                break;

            case 10:
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printListCodeValue(man->checkWaterNeeds());
                printHelper(helpers, {0});
                inputer();
                break;

            case 11:
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printHelper(helpers, {0});
                inputer();
                break;

            case 12:
                writeOptionDefaulterReservoir();
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printTable(filteredStringVector, page, elements_per_page, selected_in_page, table_mode);
                printHelper(helpers, {1, 2});
                inputer();
                break;

            case 13:
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printListCompareValues(citiesWaterDeliveredMapWithChanges, citiesWaterDeliveredMap, citiesStringMap);
                printHelper(helpers, {0});
                inputer();
                break;

            case 14:
                writeOptionDefaulterStation();
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printTable(filteredStringVector, page, elements_per_page, selected_in_page, table_mode);
                printHelper(helpers, {1, 2});
                inputer();
                break;

            case 15:
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printListCompareValues(citiesWaterDeliveredMapWithChanges, citiesWaterDeliveredMap, citiesStringMap);
                printHelper(helpers, {0});
                inputer();
                break;

            case 16:
                writeOptionDefaulterPipe();
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printTable(filteredStringVector, page, elements_per_page, selected_in_page, table_mode);
                printHelper(helpers, {1, 2});
                inputer();
                break;

            case 17:
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printListCompareValues(citiesWaterDeliveredMapWithChanges, citiesWaterDeliveredMap, citiesStringMap);
                printHelper(helpers, {0});
                inputer();
                break;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); //restore old terminal

    system("clear");

};