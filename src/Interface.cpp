#include <cstring>
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
    }
}

std::unordered_map<std::string, std::string> Interface::stringifyUMStringClass(const std::unordered_map<std::string, City>& um){
    std::unordered_map<std::string, std::string> res;
    for (auto p : um){
        res[p.second.getName()] = p.first;
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

void Interface::enterInputHandler(int loc, unsigned long sel, bool back, bool main_menu, bool main_menu2){
    refreshDirectories();
    if (back){
        location = earlier_locations.top();
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
}

void Interface::tableModeCleaner(std::unordered_map<std::string, std::string> um){
    write = write_default;
    selected_in_page = 0;
    selected = 0;
    page = 0;
    table_mode = !table_mode;
    filteredWstringPairsVector = um;
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
            if (page > filteredWstringPairsVector.size()/elements_per_page) {
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
            else if (page == filteredWstringPairsVector.size()/elements_per_page){
                selected_in_page = filteredWstringPairsVector.size()%elements_per_page - 1;
            }
            else {
                selected_in_page = elements_per_page - 1;
            }
        }
        if (user_in == 'B') {
            selected_in_page ++;
            if (page == filteredWstringPairsVector.size()/elements_per_page){
                if (selected_in_page > filteredWstringPairsVector.size()%elements_per_page - 1){
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
                    city_analised = it->first;
                    city_analised_code = it->second;
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
                printMonoInfo(bold + converter.from_bytes(std::to_string(man->maxFlow(*man->getWaterNetwork(),"source") ))+ end_effect);
                printHelper(helpers, {0});
                inputer();
                break;

            case 9:
                printDirectory(directory);
                printOptions(options[location], selected, table_mode);
                printMonoInfo(bold + L"Waiting for Function" + end_effect);
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
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); //restore old terminal

    system("clear");

};