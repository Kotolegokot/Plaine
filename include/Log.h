/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <ostream>
#include <string>
#include <mutex>
#include <utility>

const std::string LOG_FILE { "logfile" };

enum class severity_level { error, warning, notice, info, debug };

static inline std::ostream &operator <<(std::ostream &out, severity_level level)
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

static inline std::wostream &operator <<(std::wostream &wout, severity_level level)
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

class Log {
    Log() = default;

    std::ofstream out { LOG_FILE };
    std::wofstream wout { LOG_FILE };
    std::mutex mutex;

    static Log instance;
public:
    static Log &getInstance()
    {
        return instance;
    }

    template <typename... Args>
    void write(severity_level level, Args &&... args)
    {
        std::lock_guard<std::mutex> lock(mutex);
        out << level << " ";
        (void)(int []){ 0, (out << std::forward<Args>(args), 0)... };
        out << std::endl;
    }

    template <typename... Args>
    void wwrite(severity_level level, Args &&... args)
    {
        std::lock_guard<std::mutex> lock(mutex);
        wout << level << L" ";
        (void)(int []){ 0, (out << std::forward<Args>(args), 0)... };
        wout << std::endl;
    }

    template <typename... Args>
    void error(Args &&... args)
    {
        write(severity_level::error, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void werror(Args &&... args)
    {
        wwrite(severity_level::error, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warning(Args &&... args)
    {
        write(severity_level::warning, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void wwarning(Args &&... args)
    {
        wwrite(severity_level::warning, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void notice(Args &&... args)
    {
        write(severity_level::notice, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void wnotice(Args &&... args)
    {
        wwrite(severity_level::notice, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(Args &&... args)
    {
        write(severity_level::info, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void winfo(Args &&... args)
    {
        wwrite(severity_level::info, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void debug(Args &&... args)
    {
        write(severity_level::debug, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void wdebug(Args &&... args)
    {
        wwrite(severity_level::debug, std::forward<Args>(args)...);
    }
};

#endif // LOG_H
