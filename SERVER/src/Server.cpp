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

Server::Server(asio::io_service &io_service,
               unsigned int players_count, unsigned short port) :
    m_io_service(io_service),
    m_acceptor(io_service, tcp::endpoint(tcp::v4(), port)),
    m_players_count(players_count),
    m_port(port)
{
    std::cout << "Listening..." << std::endl;
    m_status = LISTENING;
    //m_acceptor.listen();
    start_accept();
}

void Server::start_accept()
{
//    if (m_players_count - m_manager.count() == 0) {
//        while (!m_manager.all_ready());
//        play();
//        return;
//    }

    new_connection.reset(new Connection(m_io_service, m_manager));
    m_acceptor.async_accept(new_connection->socket(), std::bind(&Server::handle_accept,
                                                                this,
                                                                std::placeholders::_1));
}

void Server::handle_accept(const asio::error_code &e)
{
    if (!e) {
        m_manager.start(new_connection);
        std::cout << "client accepted, "
                  << m_players_count - m_manager.count()
                  << " left" << std::endl;
    } else {
        std::cerr << "Error: " << e.message() << std::endl;
    }

    start_accept();
}

void Server::play()
{
    std::thread([this] {
        m_status = GENERATING_CHUNKS;
        std::cout << "Generating chunks..." << std::endl;
        auto chunk_db = ChunkGenerator::generate();

        m_status = PLAYING;
        std::cout << "Playing..." << std::endl;
        m_world = std::make_unique<World>(*chunk_db);

        std::this_thread::sleep_for(std::chrono::seconds(5));

        m_status = INVALID;
        std::cout << "Game Over" << std::endl;

        // close connections
        m_manager.stop_all();
        m_acceptor.close();
    }).detach();
}

void Server::wait() const
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
