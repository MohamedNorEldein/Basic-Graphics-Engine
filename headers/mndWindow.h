#pragma once
#pragma warning(disable : 4996)

#include<windows.h>
#include <iostream>
#include <string>



LRESULT CALLBACK EvetProtocol1(HWND handel, UINT msg, WPARAM w, LPARAM l);
//static FILE * out;

#define out(X)			MessageBox(nullptr, (X), L"debug out", MB_OK)
#define cout(X)			std::cout << (X) << std::endl
const long double MATH_PI = 3.1415926535897932384626433832795;

