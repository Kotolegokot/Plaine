#include "Log.h"

std::ofstream Log::out { LOG_FILE };
std::wofstream Log::wout { LOG_FILE };
std::mutex Log::mutex;

std::ostream &operator <<(std::ostream &out, severity_level level)
{
    switch (level) {
    case severity_level::error:
        out << "ERROR   :";
    case severity_level::warning:
        out << "WARNING :";
    case severity_level::notice:
        out << "NOTICE  :";
    case severity_level::info:
        out << "INFO    :";
    case severity_level::debug:
        out << "DEBUG   :";
    }

    return out;
}

std::wostream &operator <<(std::wostream &wout, severity_level level)
{
    switch (level) {
    case severity_level::error:
        wout << L"ERROR   :";
    case severity_level::warning:
        wout << L"WARNING :";
    case severity_level::notice:
        wout << L"NOTICE  :";
    case severity_level::info:
        wout << L"INFO    :";
    case severity_level::debug:
        wout << L"DEBUG   :";
    }

    return wout;
}
