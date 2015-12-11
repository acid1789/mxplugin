#pragma once

//#include "PluginSimpleFnLib.h"

#ifdef PLUGINSIMPLEFNLIB_EXPORTS
#define PLUGINSIMPLEFNLIB_API __declspec(dllexport)
#else
#define PLUGINSIMPLEFNLIB_API __declspec(dllimport)
#endif

#include <string>

extern "C" PLUGINSIMPLEFNLIB_API const char* SimpleFunction();