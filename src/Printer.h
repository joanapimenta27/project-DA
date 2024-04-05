#ifndef AED_PROJ_2_PRINTER_H
#define AED_PROJ_2_PRINTER_H

#include <iostream>
#include <vector>
#include <sys/ioctl.h>
#include <locale>
#include <codecvt>

std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

//----------------- COLOR SCHEMES --------------------//
std::wstring bg_light_red = L"\x1b[101m";
std::wstring end_bg = L"\x1b[0m";
std::wstring italic = L"\x1b[3m";
std::wstring end_italic = L"\x1b[0m";
std::wstring red = L"\033[31m";
std::wstring bold = L"\033[1m";
std::wstring underline = L"\033[4m";;
std::wstring end_effect = L"\033[0m";
//----------------------------------------------------//

std::vector<std::vector<std::wstring>> loadingDataAnimation = {
        {
                L"█   ████  ██  ███  ███ ██  █  ████    ███   ██  ███  ██              ",
                L"█   █  █ █__█ █  █  █  █ █ █  █  _    █  █ █__█  █  █__█   █         ",
                L"███ ████ █  █ ███  ███ █  ██  ████    ███  █  █  █  █  █      █  █  █",
        },
        {
                L"█   ████  ██  ███  ███ ██  █  ████    ███   ██  ███  ██              ",
                L"█   █  █ █__█ █  █  █  █ █ █  █  _    █  █ █__█  █  █__█      █      ",
                L"███ ████ █  █ ███  ███ █  ██  ████    ███  █  █  █  █  █   █     █  █",
        },
        {
                L"█   ████  ██  ███  ███ ██  █  ████    ███   ██  ███  ██              ",
                L"█   █  █ █__█ █  █  █  █ █ █  █  _    █  █ █__█  █  █__█         █   ",
                L"███ ████ █  █ ███  ███ █  ██  ████    ███  █  █  █  █  █   █  █     █",
        },
        {
                L"█   ████  ██  ███  ███ ██  █  ████    ███   ██  ███  ██              ",
                L"█   █  █ █__█ █  █  █  █ █ █  █  _    █  █ █__█  █  █__█            █",
                L"███ ████ █  █ ███  ███ █  ██  ████    ███  █  █  █  █  █   █  █  █   ",
        },
        {
                L"█   ████  ██  ███  ███ ██  █  ████    ███   ██  ███  ██              ",
                L"█   █  █ █__█ █  █  █  █ █ █  █  _    █  █ █__█  █  █__█             ",
                L"███ ████ █  █ ███  ███ █  ██  ████    ███  █  █  █  █  █   █  █  █  █",
        },
        {
                L"█   ████  ██  ███  ███ ██  █  ████    ███   ██  ███  ██              ",
                L"█   █  █ █__█ █  █  █  █ █ █  █  _    █  █ █__█  █  █__█             ",
                L"███ ████ █  █ ███  ███ █  ██  ████    ███  █  █  █  █  █   █  █  █  █",
        },
        {
                L"█   ████  ██  ███  ███ ██  █  ████    ███   ██  ███  ██              ",
                L"█   █  █ █__█ █  █  █  █ █ █  █  _    █  █ █__█  █  █__█             ",
                L"███ ████ █  █ ███  ███ █  ██  ████    ███  █  █  █  █  █   █  █  █  █",
        },
        {
                L"█   ████  ██  ███  ███ ██  █  ████    ███   ██  ███  ██              ",
                L"█   █  █ █__█ █  █  █  █ █ █  █  _    █  █ █__█  █  █__█             ",
                L"███ ████ █  █ ███  ███ █  ██  ████    ███  █  █  █  █  █   █  █  █  █",
        },
        {
                L"█   ████  ██  ███  ███ ██  █  ████    ███   ██  ███  ██              ",
                L"█   █  █ █__█ █  █  █  █ █ █  █  _    █  █ █__█  █  █__█             ",
                L"███ ████ █  █ ███  ███ █  ██  ████    ███  █  █  █  █  █   █  █  █  █",
        }
};

int frame = 0;

int getPunct(const std::string& s){
    int count = 0;
    int count2 = 0;
    for (char c : s) {
        if (!((64 < c && c < 123) || c == 32)) {
            count++;
            count2++;
        }
    }
    return count - count2/2;
}

int getTerminalWidth() {
    struct winsize w{};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}
std::wstring centerUp(const std::wstring& txtToCenter){
    if (getTerminalWidth() / 2 - txtToCenter.size() / 2 > 0){
        return std::wstring(getTerminalWidth() / 2 - txtToCenter.size() / 2, L' ') + txtToCenter;

    }
    return txtToCenter;
}

void printCenteredTitle(const std::vector<std::wstring>& title){
    for (const std::wstring& title_segment : title){
        std::wcout << bold << centerUp(title_segment) << end_effect << std::endl;
    }
}

void printTextLoadingData(){
    system("clear");
    std::wcout << L"\n\n\n" << std::endl;
    printCenteredTitle(loadingDataAnimation[frame]);
    frame ++;
    if (frame > loadingDataAnimation.size() - 1){frame = 0;}
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    system("clear");
}

void printBoldTitle(const std::vector<std::wstring>& title){
    for (const std::wstring& title_segment : title){
        std::wcout << bold << title_segment << end_effect << std::endl;
    }
}

void printOptions(const std::vector<std::wstring>& options, unsigned long selected, bool table_mode){
    std::wcout << L"\n";
    for(const std::wstring& s : options){
        if (s == options[selected] && !table_mode){
            std::wcout << "< " << underline << bold << red << s << end_effect << L" >" << std::endl << L"\n";
        }
        else{
            std::wcout << bold << L"  " << s << end_effect << std::endl << L"\n";
        }
    }
}

void printDirectory(const std::wstring& directory) {
    std::wcout << bold << L" --";
    for (wchar_t c : directory){
        std::wcout << L"-";
    }
    std::wcout << L"\n| " << directory << L" |" << std::endl;
    std::wcout << L" --";
    for (wchar_t c : directory){
        std::wcout << L"-";
    }
    std::wcout << end_effect << L"\n\n\n";
}

void printHelper(std::vector<std::wstring> helpers, const std::vector<int>& selections){
    std::wcout << L"\n" << std::endl;
    for (int selection : selections){
        std::wcout << italic << helpers[selection] << end_italic << std::endl;
    }
}

void printMonoInfo(const std::wstring& wstr){
    std::wstring wigly_underline;
    std::wcout << L"\n\n\n" << std::endl;
    std::wcout << centerUp(L"-> "  + wstr +  L" <-") << std::endl;
    for (wchar_t c : wstr){
        wigly_underline.push_back(L'~');
    }
    for (int i = 0; i < 6; i++){
        wigly_underline.push_back(L'~');
    }
    std::wcout << centerUp(red + wigly_underline + end_effect) << std::endl;
}

void printDoubleTable(const std::unordered_map<std::string, std::string>& wstring_list, int page, int elements_per_page, unsigned long selected_in_page, bool table_mode){
    std::wcout << L"\n\n " << bold << std::wstring(40, L'-') << end_effect << L"\n";
    int count = 0;
    int count_for_selected = 0;
    for (const auto& wstr : wstring_list){
        if (count >= page * elements_per_page && count < page * elements_per_page + elements_per_page){
            if (count_for_selected == selected_in_page && table_mode){
                std::wcout << L"| " << bg_light_red << italic << underline << bold
                           << converter.from_bytes(wstr.first) << std::wstring(30 - wstr.first.size() + getPunct(wstr.first), L' ')
                           << end_italic << bg_light_red << underline << L"| " << italic << converter.from_bytes(wstr.second)
                           << std::wstring(6 - wstr.second.size(), L' ')
                           << end_italic << end_bg << end_effect << L" |" << std::endl;
            }
            else{
                std::wcout << L"| " << converter.from_bytes(wstr.first) << std::wstring(30 - wstr.first.size() + getPunct(wstr.first), L' ')
                << L"| " << converter.from_bytes(wstr.second) << std::wstring(6 - wstr.second.size(), L' ')
                << L" |" << std::endl;
            }
            count_for_selected ++;
        }
        count ++;
    }
    if (wstring_list.empty()){std::wcout << L"|" << italic << L"  No results " << std::wstring(27, L' ') << end_effect << L"|" << std::endl;}
    std::wcout << L"|" << bold << std::wstring(40, L'-') << end_effect << L"|\n";
    std::wcout << L"|" << italic << L" <('p')" << std::wstring(26, L' ') << L"('n')> " << end_italic << L"|" << std::endl;
    std::wcout << L"|" << std::wstring(38 - std::to_string(page + 1).size() - std::to_string(wstring_list.size()/elements_per_page + 1).size(), ' ')
               << bold << page + 1 << L"/" << wstring_list.size()/elements_per_page + 1 << end_effect << L" |" << std::endl;
    std::wcout << L' ' << bold << std::wstring(40, L'-') << end_effect << std::endl;
    std::wcout << italic << L"\n Total Number : " << end_italic << bold<< wstring_list.size() << end_effect << std::endl;
}

#endif //AED_PROJ_2_PRINTER_H
