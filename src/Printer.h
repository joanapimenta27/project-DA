#ifndef AED_PROJ_2_PRINTER_H
#define AED_PROJ_2_PRINTER_H

#include <iostream>
#include <vector>
#include <sys/ioctl.h>

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

#endif //AED_PROJ_2_PRINTER_H
