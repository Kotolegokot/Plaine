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

#pragma once

#include <set>
#include <algorithm>
#include "Connection.hpp"

class ConnectionManager {
    std::set<connection_ptr> m_connections;

public:
    // add the specified connection to the manager and start it
    void start(connection_ptr c);

    // stop the specified connection
    void stop(connection_ptr c);

    // stop all connections
    void stop_all();

    // checks if all players are ready
    bool all_ready() const;

    // number of active connections
    std::size_t count() const;
};
