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
#include <asio.hpp>

#include "Error.hpp"

using asio::ip::tcp;
using asio::ip::udp;

class Server
{
    // when INVALID, server must not be started
    // again, it should be recreated instead
    enum Status { STOPPED, LISTENING, PLAYING, INVALID };
    std::atomic<Status> m_status { STOPPED };

    asio::io_service m_io_service;
    tcp::acceptor m_acceptor;
    std::vector<tcp::socket> m_players;

    const unsigned int m_players_count;
    const unsigned short m_port;

    void accept_players();
    void play();
public:
    Server(unsigned int players_count = 0, unsigned short port = 0);

    void wait();
    bool running() const;
    bool invalid() const;
    Status status() const;

    unsigned int players_count() const;
    unsigned short port() const;
};
