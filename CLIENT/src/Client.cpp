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

#include "Client.hpp"

// server's messages
const std::string REQUEST_VERSION = "REQUEST VERSION";
const std::string INVALID_VERSION = "INVALID VERSION";
const std::string NICE_VERSION    = "NICE VERSION";

const std::string REQUEST_NICK    = "REQUEST NICK";
const std::string INVALID_NICK    = "INVALID NICK";
const std::string NICE_NICK       = "NICE NICK";

const std::string REQUEST_READY   = "REQUEST READY";

Client::Client(asio::io_service &io_service) :
    m_io_service(io_service),
    m_resolver(io_service),
    m_socket(io_service)
{}

void Client::start(const std::string &nick, const std::string &host, unsigned short port)
{
    m_nick = nick;

    tcp::resolver::query query(host, std::to_string(port));
    m_resolver.async_resolve(query, std::bind(&Client::handle_resolve, this,
                                              std::placeholders::_1,
                                              std::placeholders::_2));
}

void Client::handle_resolve(const asio::error_code &e, tcp::resolver::iterator endpoint_it)
{
    if (!e) {
        // attempt a connection to every endpoint in the list
        // until we successfully establish a connection
        asio::async_connect(m_socket, endpoint_it,
                            std::bind(&Client::handle_connect, this,
                                      std::placeholders::_1));
    } else {
        std::cerr << "Error: " << e.message() << std::endl;
    }
}

void Client::handle_connect(const asio::error_code &e)
{
    if (!e) {
        // the connection was successful
        // wait for requests
        m_socket.async_read_some(asio::buffer(m_buffer),
                                 std::bind(&Client::handle_read, this,
                                           std::placeholders::_1,
                                           std::placeholders::_2));
    } else {
        std::cerr << "Error: " << e.message() << std::endl;
    }
}

void Client::handle_read(const asio::error_code &e, std::size_t bytes_transferred)
{
    if (!e) {
        auto begin = m_buffer.cbegin();
        auto end = begin;
        std::advance(end, bytes_transferred);

        std::string msg(begin, end);
        std::cout << "FROM SERVER: " << msg << std::endl;
        if (msg == REQUEST_VERSION) {
            // send protocol version
            m_status = VERSION;
            std::cout << "TO SERVER: " << PROTOCOL_VERSION << std::endl;
            asio::async_write(m_socket, asio::buffer(PROTOCOL_VERSION),
                                        std::bind(&Client::handle_write, this,
                                                  std::placeholders::_1));
        } else if (msg == INVALID_VERSION) {
            // protocol versions are incompatible, abort
            std::cerr << "Incompatible protocol versions" << std::endl;
            stop();
        } else if (msg == NICE_VERSION) {
            // versions are compatible, read on
            m_status = NONE;
            m_socket.async_read_some(asio::buffer(m_buffer),
                                     std::bind(&Client::handle_read, this,
                                               std::placeholders::_1,
                                               std::placeholders::_2));
        } else if (msg == REQUEST_NICK) {
            // send nick
            m_status = NICK;
            std::cout << "TO SERVER: " << m_nick << std::endl;
            asio::async_write(m_socket, asio::buffer(m_nick),
                                        std::bind(&Client::handle_write,
                                                  this,
                                                  std::placeholders::_1));
        } else if (msg == INVALID_NICK) {
            // invalid nick, abort
            std::cerr << "Invalid nick: '" << m_nick << "'" << std::endl;
            stop();
        } else if (msg == NICE_NICK) {
            // nick is alright, read on
            m_status = NONE;
            m_socket.async_read_some(asio::buffer(m_buffer),
                                     std::bind(&Client::handle_read, this,
                                               std::placeholders::_1,
                                               std::placeholders::_2));
        } else if (msg == REQUEST_READY) {
            // send ready
            m_status = READY;
            std::cout << "TO SERVER: READY" << std::endl;
            asio::async_write(m_socket, asio::buffer("READY"),
                                        std::bind(&Client::handle_write, this,
                                                  std::placeholders::_1));
        } else {
            // invalid request, abort
            std::cerr << "Invalid request: " << msg << std::endl;
            stop();
        }
    } else {
        std::cerr << "Error: " << e.message() << std::endl;
    }
}

void Client::handle_write(const asio::error_code &e)
{
    if (!e) {
        switch (m_status) {
        case NONE:
        case VERSION:
        case NICK:
        case READY:
            m_socket.async_read_some(asio::buffer(m_buffer),
                                     std::bind(&Client::handle_read,
                                               this,
                                               std::placeholders::_1,
                                               std::placeholders::_2));
            break;
        case PLAYING:
            break;
        }
    } else {
        std::cerr << "Error: " << e.message() << std::endl;
    }
}

void Client::stop()
{
    m_status = NONE;
    m_socket.close();
}

Client::Status Client::status() const
{
    return m_status;
}
