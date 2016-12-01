#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <ostream>
#include <string>
#include <mutex>

const std::string LOG_FILE { "logfile" };

enum class severity_level { error, warning, notice, info, debug };

class Log {
    static std::ofstream out;
    static std::mutex mutex;

public:
    Log() = delete;

    static void write(severity_level level, const std::string &message);

    static void error(const std::string &message);
    static void warning(const std::string &message);
    static void notice(const std::string &message);
    static void info(const std::string &message);
    static void debug(const std::string &message);
};

#endif // LOG_H
