//
// Created by jiang.wenqiang on 2018/8/14.
//

#include <windows.h>
#include <iostream>
using std::wcout;
using std::endl;

int main () {
    wchar_t str[20];
    DWORD num = 20;
    GetUserNameW(str, &num);
    wcout << str << ":" << num << endl;
    num = 20;
    GetComputerNameW(str, &num);
    wcout << str << ":" << num << endl;
}