#include "util.h"
#include <random>

using namespace irr;

#ifndef _WIN32

void setLanguage(std::string language, bool replace)
{
    setenv("LANGUAGE", language.c_str(), replace);

    // stinky hack (from https://www.gnu.org/software/gettext/manual/html_node/gettext-grok.html)
    {
        extern int  _nl_msg_cat_cntr;
        ++_nl_msg_cat_cntr;
    }
}

size_t convert(const char *to, const char *from, char *outbuf, size_t outbuf_size, char *inbuf, size_t inbuf_size) {
	iconv_t cd = iconv_open(to, from);

	char *inbuf_ptr = inbuf;
	char *outbuf_ptr = outbuf;

	size_t *inbuf_left_ptr = &inbuf_size;
	size_t *outbuf_left_ptr = &outbuf_size;

	while (inbuf_size > 0)
		iconv(cd, &inbuf_ptr, inbuf_left_ptr, &outbuf_ptr, outbuf_left_ptr);

	iconv_close(cd);
	return 0;
}

std::wstring utf8_to_wide(const std::string &input) {
	size_t inbuf_size = input.length() + 1;
	// maximum possible size, every character is sizeof(wchar_t) bytes
	size_t outbuf_size = (input.length() + 1) * sizeof(wchar_t);

	char *inbuf = new char[inbuf_size];
	memcpy(inbuf, input.c_str(), inbuf_size);
	char *outbuf = new char[outbuf_size];
	memset(outbuf, 0, outbuf_size);

	convert("WCHAR_T", "UTF-8", outbuf, outbuf_size, inbuf, inbuf_size);
	std::wstring out((wchar_t*)outbuf);

	delete[] inbuf;
	delete[] outbuf;

	return out;
}

core::stringw utf8_to_irrwide(const std::string &input) {
	size_t inbuf_size = input.length() + 1;
	// maximum possible size, every character is sizeof(wchar_t) bytes
	size_t outbuf_size = (input.length() + 1) * sizeof(wchar_t);

	char *inbuf = new char[inbuf_size];
	memcpy(inbuf, input.c_str(), inbuf_size);
	char *outbuf = new char[outbuf_size];
	memset(outbuf, 0, outbuf_size);

	convert("WCHAR_T", "UTF-8", outbuf, outbuf_size, inbuf, inbuf_size);
	core::stringw out((wchar_t *)outbuf);

	delete[] inbuf;
	delete[] outbuf;

	return out;
}

std::string wide_to_utf8(const std::wstring &input) {
	size_t inbuf_size = (input.length() + 1) * sizeof(wchar_t);
	// maximum possible size: utf-8 encodes codepoints using 1 up to 6 bytes
	size_t outbuf_size = (input.length() + 1) * 6;

	char *inbuf = new char[inbuf_size];
	memcpy(inbuf, input.c_str(), inbuf_size);
	char *outbuf = new char[outbuf_size];
	memset(outbuf, 0, outbuf_size);

	convert("UTF-8", "WCHAR_T", outbuf, outbuf_size, inbuf, inbuf_size);
	std::string out(outbuf);

	delete[] inbuf;
	delete[] outbuf;

	return out;
}
#else

void setLanguage(std::string language, bool)
{
        std::string str = "LANGUAGE=";
        str += language;
        _wputenv(utf8_to_wide(str).c_str());
}

std::wstring utf8_to_wide(const std::string &input) {
	size_t outbuf_size = input.size() + 1;
	wchar_t *outbuf = new wchar_t[outbuf_size];
	memset(outbuf, 0, outbuf_size * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, input.c_str(), input.size(), outbuf, outbuf_size);
	std::wstring out(outbuf);
	delete[] outbuf;
	return out;
}

core::stringw utf8_to_irrwide(const std::string &input) {
	size_t outbuf_size = input.size() + 1;
	wchar_t *outbuf = new wchar_t[outbuf_size];
	memset(outbuf, 0, outbuf_size * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, input.c_str(), input.size(), outbuf, outbuf_size);
    core::stringw out(outbuf);
	delete[] outbuf;

	return out;
}

std::string wide_to_utf8(const std::wstring &input) {
	size_t outbuf_size = (input.size() + 1) * 6;
	char *outbuf = new char[outbuf_size];
	memset(outbuf, 0, outbuf_size);
	WideCharToMultiByte(CP_UTF8, 0, input.c_str(), input.size(), outbuf, outbuf_size, NULL, NULL);
	std::string out(outbuf);
	delete[] outbuf;
	return out;
}
#endif // _WIN32

f32 getRandomf(f32 min, f32 max)
{
    static std::default_random_engine engine;
    static std::uniform_real_distribution<> distribution(min, max);

    return distribution(engine);
}

core::stringw keyCodeName(const EKEY_CODE &keyCode)
{
    if ((keyCode >= 48 && keyCode<= 57) || (keyCode >= 65 && keyCode <= 90)) {
        core::stringw str;
        str.append(keyCode);
        return str;
    }
    switch(keyCode)
    {
    case(KEY_UP):
        return _w("Up Arrow");
        break;
    case(KEY_LEFT):
        return _w("Left Arrow");
        break;
    case(KEY_DOWN):
        return _w("Down Arrow");
        break;
    case(KEY_RIGHT):
        return _w("Right Arrow");
        break;
    case(KEY_NUMPAD0):
        return L"NUM 0";
        break;
    case(KEY_NUMPAD1):
        return L"NUM 1";
        break;
    case(KEY_NUMPAD2):
        return L"NUM 2";
        break;
    case(KEY_NUMPAD3):
        return L"NUM 3";
        break;
    case(KEY_NUMPAD4):
        return L"NUM 4";
        break;
    case(KEY_NUMPAD5):
        return L"NUM 5";
        break;
    case(KEY_NUMPAD6):
        return L"NUM 6";
        break;
    case(KEY_NUMPAD7):
        return L"NUM 7";
        break;
    case(KEY_NUMPAD8):
        return L"NUM 8";
        break;
    case(KEY_NUMPAD9):
        return L"NUM 9";
        break;
    case(KEY_TAB):
        return L"Tab";
        break;
    case(KEY_BACK):
        return L"Back";
        break;
    case(KEY_SPACE):
        return _w("Space");
        break;
    case(KEY_LSHIFT):
        return L"Left Shift";
        break;
    case(KEY_LCONTROL):
        return L"Left Ctrl";
        break;
    case(KEY_RSHIFT):
        return L"Right Shift";
        break;
    case(KEY_RCONTROL):
        return L"Right Ctrl";
        break;
    case(KEY_LMENU):
        return L"Alt";
        break;
    case(KEY_RMENU):
        return L"Right Alt";
        break;
    default:
        return L"";
    }
}

video::SColor iridescentColor(const u32 &currentTime)
{
    static video::SColor color = video::SColor(0, getRandomf(0, 255), getRandomf(0, 255), getRandomf(0, 255));
    static video::SColor oldColor = color;
    static f32 diff = 0;
    static u32 time = 0;
    if (int(time - currentTime) > COLOR_CHANGE_INTERVAL)
        time = currentTime + COLOR_CHANGE_INTERVAL;
    if (currentTime >= time)
        {
            oldColor = color;
            color = video::SColor(0, getRandomf(0, 255), getRandomf(0, 255), getRandomf(0, 255));
            time = currentTime + COLOR_CHANGE_INTERVAL;
        }
    diff = (time - currentTime)/COLOR_CHANGE_INTERVAL;

    return video::SColor(0, color.getRed() + int((oldColor.getRed()) - (color.getRed()))*diff,
                                    color.getGreen() + int(oldColor.getGreen() - color.getGreen())*diff,
                                    color.getBlue() + int(oldColor.getBlue() - color.getBlue())*diff);
}
