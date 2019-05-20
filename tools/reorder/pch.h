#ifndef PCH_H
#define PCH_H

#include <stdio.h>
#include <tchar.h>
#include <map>
#include <string>
#include <list>
#include <istream>
#include <ostream>
#include <iostream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <iomanip>
#include <vector>
#include <sstream>
#include <filesystem>
#include <windows.h>

using namespace std;

typedef std::basic_string<TCHAR> tstring;

#if defined(UNICODE) || defined(_UNICODE)
#define tcout std::wcout
#else
#define tcout std::cout
#endif

#endif //PCH_H
