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

#include <Server.hpp>

void Server::start()
{
    m_running = true;

    std::thread([this] {
        std::cout << "Start server..." << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(5));

        m_running = false;
        std::cout << "Stop server" << std::endl;
    }).detach();
}

void Server::wait()
{
    while (m_running);
}

bool Server::running() const
{
    return m_running;
}
