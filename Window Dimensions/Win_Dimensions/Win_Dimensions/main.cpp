#define _WIN32_DCOM
#include <iostream>
#include <WbemIdl.h>
#pragma comment( lib, "wbemuuid.lib" )

int main()
{
    HRESULT hr;
    hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    HWND console = GetConsoleWindow();
    RECT ConsoleRect;
    GetWindowRect(console, &ConsoleRect);
    SetWindowPos(
        console,
        HWND_TOP,
        1920/2,
        0,
        1920/2,
        1080,
        SWP_SHOWWINDOW
    );

    std::cout << "            Hello World " << std::endl;
    std::cout << " -------------------------------" << std::endl;
    std::cout << " This is an example program that" << std::endl;
    std::cout << " demonstrates how to setup the " << std::endl;
    std::cout << " window position and size using" << std::endl;
    std::cout << " Win32 API" << std::endl;
    std::cin.get();
}