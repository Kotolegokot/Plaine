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

#include "ConnectionManager.hpp"

void ConnectionManager::start(connection_ptr c)
{
    m_connections.insert(c);
    c->start();
}

void ConnectionManager::stop(connection_ptr c)
{
    m_connections.erase(c);
    c->stop();
}

void ConnectionManager::stop_all()
{
    std::for_each(m_connections.begin(), m_connections.end(), std::mem_fn(&Connection::stop));
    m_connections.clear();
}

bool ConnectionManager::all_ready() const
{
    return std::all_of(m_connections.cbegin(), m_connections.cend(), std::mem_fn(&Connection::ready));
}

std::size_t ConnectionManager::count() const
{
    return m_connections.size();
}
