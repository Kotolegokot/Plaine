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

#include <memory>
#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <atomic>
#include <asio.hpp>

using asio::ip::tcp;

const std::string PROTOCOL_VERSION = "0.0.1";

class ConnectionManager;

class Connection : public std::enable_shared_from_this<Connection>
{
    // socket for the connection
    tcp::socket m_socket;

    // the manager for this connection
    ConnectionManager &m_manager;

    // buffer for incoming data
    std::array<char, 8192> m_buffer;

    // handle completion of a write operation
    void handle_write(const asio::error_code &e);

    // handle completion of a read operation
    void handle_read(const asio::error_code &error,
                     std::size_t bytes_transferred);

    enum Status { NONE,
                  READ_VERSION,
                  REQUEST_VERSION,
                  READ_NICK,
                  REQUEST_NICK,
                  READ_READY,
                  REQUEST_READY,
                  ABORT,
                  PLAYING };

    std::atomic<Status> m_status { NONE };

    // check if the nick is not allowed on this server
    bool invalid_nick(const std::string &nick) const;

public:
    explicit Connection(asio::io_service &io_service, ConnectionManager &manager);

    // get the socket associated with the connection
    tcp::socket &socket();

    // start the first asynchronous operation for the connection
    void start();

    // stop all asynchronous operations associated with the connection
    void stop();

    // checks if the player is ready to play
    bool ready() const;
};

using connection_ptr = std::shared_ptr<Connection>;
