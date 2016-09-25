#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string>
#include <cstdlib>
#include <irrlicht.h>
#include <libintl.h>

#ifndef _WIN32
#include <iconv.h>
#else
#include <Windows.h>
#endif

using namespace irr;

#define _(string) gettext(string)
#define _w(string) utf8_to_irrwide(gettext(string))
#define _wp(string) utf8_to_wide(gettext(string)).c_str()

size_t convert(const char *to, const char *from, char *outbuf, size_t outbuf_size, char *inbuf, size_t inbuf_size);
std::wstring utf8_to_wide(const std::string &input);
core::stringw utf8_to_irrwide(const std::string &input);
std::string wide_to_utf8(const std::wstring &input);
void setLanguage(std::string language, bool replace);
f32 getRandomf(f32 min = 0, f32 max = 1);

// GUI IDs
enum { ID_BUTTON_QUIT,
    ID_BUTTON_START,
    ID_BUTTON_SETTINGS,
    ID_BUTTON_MENU,
    ID_BUTTON_TOGGLE_FULLSCREEN,
    ID_BUTTON_RESUME,
    ID_COMBOBOX_RESOLUTION,
    ID_COMBOBOX_COLORDEPTH,
    ID_COMBOBOX_LANGUAGE,
    ID_CHECKBOX_VSYNC,
    ID_CHECKBOX_STENCILBUFFER};

#endif // UTIL_H_INCLUDED
