#include "Log.h"

std::ofstream Log::out { LOG_FILE };
std::wofstream Log::wout { LOG_FILE };
std::mutex Log::mutex;

std::ostream &operator <<(std::ostream &out, severity_level level)
{
    switch (level) {
    case severity_level::error:
        out << "ERROR   :";
        break;
    case severity_level::warning:
        out << "WARNING :";
        break;
    case severity_level::notice:
        out << "NOTICE  :";
        break;
    case severity_level::info:
        out << "INFO    :";
        break;
    case severity_level::debug:
        out << "DEBUG   :";
        break;
    }

    return out;
}

std::wostream &operator <<(std::wostream &wout, severity_level level)
{
    switch (level) {
    case severity_level::error:
        wout << L"ERROR   :";
        break;
    case severity_level::warning:
        wout << L"WARNING :";
        break;
    case severity_level::notice:
        wout << L"NOTICE  :";
        break;
    case severity_level::info:
        wout << L"INFO    :";
        break;
    case severity_level::debug:
        wout << L"DEBUG   :";
        break;
    }

    return wout;
}
