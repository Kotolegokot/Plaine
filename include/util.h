#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string>
#include <cstdlib>
#include <irrlicht/irrlicht.h>

#ifndef _WIN32
#include <iconv.h>
#else
#include <Windows.h>
#endif

using namespace irr;

size_t convert(const char *to, const char *from, char *outbuf, size_t outbuf_size, char *inbuf, size_t inbuf_size);
std::wstring utf8_to_wide(const std::string &input);
std::string wide_to_utf8(const std::wstring &input);

// GUI IDs
enum { ID_BUTTON_QUIT, ID_BUTTON_START, ID_BUTTON_SETTINGS, ID_BUTTON_MENU, ID_BUTTON_TOGGLE_FULLSCREEN, ID_RESOLUTION_COMBOBOX};

#endif // UTIL_H_INCLUDED
