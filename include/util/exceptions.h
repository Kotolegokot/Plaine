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

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class initialization_error : public std::exception {
public:
    const char *what() const noexcept override
    {
        return "failed to initialize device";
    }
};

class repeated_initialization : public std::exception {
public:
    const char *what() const noexcept override
    {
        return "repeated device initialization";
    }
};

class invalid_audiosource : public std::exception {
    unsigned int m_source;
    std::string m_what;
public:
    invalid_audiosource(unsigned int source) :
        m_source(source)
    {
        m_what = "invalid audio source (";
        m_what += m_source;
        m_what += ")";
    }

    const char *what() const noexcept override
    {
        return m_what.c_str();
    }
};

#endif // EXCEPTIONS_H
