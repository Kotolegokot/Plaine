#include "Log.h"

std::ofstream Log::out { LOG_FILE };
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

void Log::write(severity_level level, const std::string &message)
{
    std::lock_guard<std::mutex> lock(mutex);
    out << level << " " << message << std::endl;
}

void Log::error(const std::string &message)
{
    write(severity_level::error, message);
}

void Log::warning(const std::string &message)
{
    write(severity_level::warning, message);
}

void Log::notice(const std::string &message)
{
    write(severity_level::notice, message);
}

void Log::info(const std::string &message)
{
    write(severity_level::info, message);
}

void Log::debug(const std::string &message)
{
    write(severity_level::debug, message);
}
