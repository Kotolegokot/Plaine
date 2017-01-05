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

Server::Server(unsigned int players_count, unsigned short port) :
     m_acceptor(m_io_service, tcp::endpoint(tcp::v4(), port)),
     m_players_count(players_count),
     m_port(port)
{
    m_io_service.run();

    std::cout << "Listening..." << std::endl;
    m_status = LISTENING;
    accept_players();
}

void Server::accept_players()
{
    if (m_players_count == 0) {
        play();
        return;
    }

    m_players.emplace_back(m_io_service);
    m_acceptor.async_accept(m_players.back(), [this](const asio::error_code &error)
    {
        if (error) {
            m_players.pop_back();
        } else {
            std::cout << "player accepted, "
                      << m_players_count - m_players.size()
                      << " left" << std::endl;
        }

        if (m_players_count - m_players.size() == 0) {
            play();
        } else {
            accept_players();
        }
    });
}

void Server::play()
{
    std::thread([this] {
        m_status = GENERATING_CHUNKS;
        auto chunk_db = ChunkGenerator::generate();

        m_status = PLAYING;
        m_world = std::make_unique<World>(*chunk_db);

        std::cout << "Playing..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "Game Over" << std::endl;

        m_status = INVALID;
        for (tcp::socket &player : m_players)
            player.close();
        m_acceptor.close();
    }).detach();
}

void Server::wait()
{
    while (m_status != STOPPED ||
           m_status != INVALID);
}

bool Server::running() const
{
    return m_status == LISTENING ||
           m_status == PLAYING;
}

bool Server::invalid() const
{
    return m_status == INVALID;
}

Server::Status Server::status() const
{
    return m_status;
}

unsigned int Server::players_count() const
{
    return m_players_count;
}

unsigned short Server::port() const
{
    return m_port;
}
