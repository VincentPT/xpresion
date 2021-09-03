#pragma once

#ifdef _WIN32
#ifdef EXPORT_EXPRESSIONCPP
#define EXPRESSIONCPP_API __declspec(dllexport)
#else
#define EXPRESSIONCPP_API __declspec(dllimport)
#endif // EXPORT_EXPRESSIONCPP
#else
#define EXPRESSIONCPP_API
#endif // _WIN32