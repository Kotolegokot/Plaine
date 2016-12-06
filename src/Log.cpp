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
