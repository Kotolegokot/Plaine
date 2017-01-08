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

#include "Connection.hpp"
#include "ConnectionManager.hpp"

const std::string S_REQUEST_VERSION = "REQUEST VERSION";
const std::string S_INVALID_VERSION = "INVALID VERSION";
const std::string S_NICE_VERSION    = "NICE VERSION";

const std::string S_REQUEST_NICK    = "REQUEST NICK";
const std::string S_INVALID_NICK    = "INVALID NICK";
const std::string S_NICE_NICK       = "NICE NICK";

const std::string S_REQUEST_READY   = "REQUEST READY";

Connection::Connection(asio::io_service &io_service, ConnectionManager &manager) :
    m_socket(io_service),
    m_manager(manager)
{}

tcp::socket &Connection::socket()
{
    return m_socket;
}

void Connection::start()
{
    m_status = READ_VERSION;
    asio::async_write(m_socket, asio::buffer(S_REQUEST_VERSION),
                      std::bind(&Connection::handle_write,
                                shared_from_this(),
                                std::placeholders::_1));
}

void Connection::handle_write(const asio::error_code &e)
{
    if (!e) {
        switch (m_status) {
        case NONE:
        case READ_VERSION:
        case READ_NICK:
        case READ_READY:
            m_socket.async_read_some(asio::buffer(m_buffer),
                                     std::bind(&Connection::handle_read,
                                               shared_from_this(),
                                               std::placeholders::_1,
                                               std::placeholders::_2));
            break;
        case REQUEST_NICK:
            m_status = READ_NICK;
            m_socket.async_write_some(asio::buffer(S_REQUEST_NICK),
                                      std::bind(&Connection::handle_write,
                                                shared_from_this(),
                                                std::placeholders::_1));
            break;
        case REQUEST_READY:
            m_status = READ_READY;
            m_socket.async_write_some(asio::buffer(S_REQUEST_READY),
                                      std::bind(&Connection::handle_write,
                                                shared_from_this(),
                                                std::placeholders::_1));
            break;
        case ABORT:
            m_manager.stop(shared_from_this());
            break;
        case PLAYING:
            // playing
            m_manager.stop(shared_from_this());
            break;
        }

    } else {
        std::cerr << "Error: " << e.message() << std::endl;
        m_manager.stop(shared_from_this());
    }
}

void Connection::handle_read(const asio::error_code &e, std::size_t bytes_transferred)
{
    if (!e) {
        auto begin = m_buffer.cbegin();
        auto end = begin;
        std::advance(end, bytes_transferred);

        std::string msg(begin, end);
        std::cout << "FROM CLIENT: " << msg << std::endl;
        switch (m_status) {
        case READ_VERSION:
            if (msg == PROTOCOL_VERSION) {
                m_status = REQUEST_NICK;
                std::cout << "TO CLIENT: " << S_NICE_VERSION << std::endl;
                m_socket.async_write_some(asio::buffer(S_NICE_VERSION),
                                          std::bind(&Connection::handle_write,
                                                    shared_from_this(),
                                                    std::placeholders::_1));
            } else {
                m_status = ABORT;
                std::cout << "TO CLIENT: " << S_INVALID_VERSION << std::endl;
                m_socket.async_write_some(asio::buffer(S_INVALID_VERSION),
                                          std::bind(&Connection::handle_write,
                                                    shared_from_this(),
                                                    std::placeholders::_1));
            }
            break;
        case READ_NICK:
            if (invalid_nick(msg)) {
                m_status = ABORT;
                std::cout << "TO CLIENT: " << S_INVALID_NICK << std::endl;
                m_socket.async_write_some(asio::buffer(S_INVALID_NICK),
                                          std::bind(&Connection::handle_write,
                                                    shared_from_this(),
                                                    std::placeholders::_1));
            } else {
                m_status = REQUEST_READY;
                std::cout << "TO CLIENT: " << S_NICE_NICK << std::endl;
                m_socket.async_write_some(asio::buffer(S_NICE_NICK),
                                          std::bind(&Connection::handle_write,
                                                    shared_from_this(),
                                                    std::placeholders::_1));
            }
            break;
        case READ_READY:
            if (msg == "READY") {
                // the client is ready to play
                m_status = PLAYING;
            } else {
                m_manager.stop(shared_from_this());
            }
            break;
        case ABORT:
            m_manager.stop(shared_from_this());
            break;
        case PLAYING:
            // playing
            m_manager.stop(shared_from_this());
            break;
        }
    } else {
        std::cerr << "Error: " << e.message() << std::endl;
        m_manager.stop(shared_from_this());
    }
}

void Connection::stop()
{
    m_status = NONE;
    m_socket.close();
}

bool Connection::invalid_nick(const std::string &nick) const
{
    return nick.empty();
}

bool Connection::ready() const
{
    return m_status == PLAYING;
}
