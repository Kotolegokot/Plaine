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

#include <string>
#include <functional>
#include <iostream>
#include <array>
#include <asio.hpp>

using asio::ip::tcp;

const std::string PROTOCOL_VERSION = "0.0.1";

class Client
{
    asio::io_service &m_io_service;
    tcp::resolver m_resolver;

    // socket for the connection
    tcp::socket m_socket;

    // buffer for incoming data
    std::array<char, 8192> m_buffer;

    // player's nick
    std::string m_nick;

    void handle_resolve(const asio::error_code &e,
                        tcp::resolver::iterator endpoint_it);

    void handle_connect(const asio::error_code &e);

    // handle completion for a write operation
    void handle_write(const asio::error_code &e);

    // handle completion for a read operation
    void handle_read(const asio::error_code &e,
                     std::size_t bytes_transferred);

    enum Status { NONE,
                  VERSION,
                  NICK,
                  READY,
                  PLAYING } m_status = NONE;

public:
    explicit Client(asio::io_service &io_service);

    // start the connection and operations for it
    void start(const std::string &nick, const std::string &host, unsigned short port);

    // stop all asynchronous operations
    // and close the connection
    void stop();

    Status status() const;
};
