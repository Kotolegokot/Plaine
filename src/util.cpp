#include "util.h"

#ifndef _WIN32
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
