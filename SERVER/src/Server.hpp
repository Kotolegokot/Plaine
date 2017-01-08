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

#include <iostream>
#include <thread>
#include <atomic>
#include <memory>
#include <functional>
#include <asio.hpp>

#include "Error.hpp"
#include "ChunkGenerator.hpp"
#include "ConnectionManager.hpp"
#include "Connection.hpp"
#include "World.hpp"

using asio::ip::tcp;
using asio::ip::udp;

class Server
{
    // when INVALID, server must not be started
    // again, it should be recreated instead
    enum Status { STOPPED, LISTENING, GENERATING_CHUNKS, PLAYING, INVALID };
    std::atomic<Status> m_status { STOPPED };

    std::unique_ptr<World> m_world;

    // io_service used to perform asnchronous operations
    asio::io_service &m_io_service;
    // acceptor used to listen for incoming connections
    tcp::acceptor m_acceptor;

    // the next connection to be accepted
    connection_ptr new_connection;

    // the connection manager that owns all live connections
    ConnectionManager m_manager;

    const unsigned int m_players_count;
    const unsigned short m_port;

    // initiate as asynchronous accept operation
    void start_accept();

    void handle_accept(const asio::error_code &e);

    void play();
public:
    Server(asio::io_service &io_service,
           unsigned int players_count, unsigned short port);
    // delete copy constructor and assignment operator
    Server(const Server &) = delete;
    Server &operator =(const Server &) = delete;
    // preserve move constructor and assignment operator
    Server(Server &&) = default;
    Server &operator =(Server &&) = default;

    void wait() const;
    bool running() const;
    bool invalid() const;
    Status status() const;

    unsigned int players_count() const;
    unsigned short port() const;
};
