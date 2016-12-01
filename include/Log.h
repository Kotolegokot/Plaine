#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <ostream>
#include <string>
#include <mutex>
#include <utility>

const std::string LOG_FILE { "logfile" };

enum class severity_level { error, warning, notice, info, debug };

std::ostream &operator <<(std::ostream &out, severity_level level);
std::wostream &operator <<(std::wostream &wout, severity_level level);

class Log {
    static std::ofstream out;
    static std::wofstream wout;
    static std::mutex mutex;

public:
    Log() = delete;

    template <typename... Args>
    static void write(severity_level level, Args &&... args)
    {
        std::lock_guard<std::mutex> lock(mutex);
        out << level << " ";
        (void)(int []){ 0, (out << std::forward<Args>(args), 0)... };
        out << std::endl;
    }

    template <typename... Args>
    static void wwrite(severity_level level, Args &&... args)
    {
        std::lock_guard<std::mutex> lock(mutex);
        wout << level << L" ";
        (void)(int []){ 0, (out << std::forward<Args>(args), 0)... };
        wout << std::endl;
    }

    template <typename... Args>
    static void error(Args &&... args)
    {
        write(severity_level::error, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void werror(Args &&... args)
    {
        wwrite(severity_level::error, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void warning(Args &&... args)
    {
        write(severity_level::warning, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void wwarning(Args &&... args)
    {
        wwrite(severity_level::warning, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void notice(Args &&... args)
    {
        write(severity_level::notice, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void wnotice(Args &&... args)
    {
        wwrite(severity_level::notice, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void info(Args &&... args)
    {
        write(severity_level::info, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void winfo(Args &&... args)
    {
        wwrite(severity_level::info, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void debug(Args &&... args)
    {
        write(severity_level::debug, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void wdebug(Args &&... args)
    {
        wwrite(severity_level::debug, std::forward<Args>(args)...);
    }
};

#endif // LOG_H
