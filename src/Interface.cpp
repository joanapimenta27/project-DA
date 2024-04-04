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
}

void Interface::stackClear(std::stack<int> &s){
    while (!s.empty()){
        s.pop();
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
            if (page > filteredWstringVector.size()/elements_per_page) {
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
            else if (page == filteredWstringVector.size()/elements_per_page){
                selected_in_page = filteredWstringVector.size()%elements_per_page - 1;
            }
            else {
                selected_in_page = elements_per_page - 1;
            }
        }
        if (user_in == 'B') {
            selected_in_page ++;
            if (page == filteredWstringVector.size()/elements_per_page){
                if (selected_in_page > filteredWstringVector.size()%elements_per_page - 1){
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
            case 1:
                switch (selected){
                    case 0:
                        break;
                    case 1:
                        enterInputHandler(0, 0, false, true, false);
                }
            case 2:
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
            case 3:
                switch (selected){
                    case 0:
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
            case 4:
                switch (selected){
                    case 0:
                        selected_dataSet = L"big";
                        is_done = false;
                        enterInputHandler(3, 0, false, false, false);
                        break;
                    case 1:
                        break;
                    case 2:
                        enterInputHandler(0, 0, false, false, true);
                        break;
                }
                break;
            case 5:
                switch (selected){
                    case 0:
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
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); //restore old terminal

    system("clear");

};