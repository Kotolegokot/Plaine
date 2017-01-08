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
#include <algorithm>
#include <memory>
#include <functional>
#include <exception>
#include <asio.hpp>
#include <irrlicht.h>
#include <ITimer.h>
#include "World.hpp"
#include "gui/GUI.hpp"
#include "gui/GUIID.hpp"
#include "gui/screens/ControlSettingsScreen.hpp"
#include "gui/screens/HUDScreen.hpp"
#include "gui/screens/MainMenuScreen.hpp"
#include "gui/screens/ConnectScreen.hpp"
#include "gui/screens/PauseMenuScreen.hpp"
#include "gui/screens/SettingsScreen.hpp"
#include "gui/screens/GameOverScreen.hpp"
#include "gui/screens/ScoreboardScreen.hpp"
#include "EventReceiver.hpp"
#include "Config.hpp"
#include "Scoreboard.hpp"
#include "util/i18n.hpp"
#include "util/CGUITTFont.hpp"
#include "util/options.hpp"
#include "util/exceptions.hpp"
#include "Client.hpp"

using asio::ip::tcp;
using asio::ip::udp;
using namespace irr;

class Game
{
    bool run();
    void mainMenu();

    asio::io_service m_io_service;

    ConfigData configuration;
    std::unique_ptr<GUI> gui;
    IrrlichtDevice *device;
    video::IVideoDriver *driver;
    scene::ISceneManager *sceneManager;
    gui::IGUIEnvironment *guiEnvironment;
    io::IFileSystem *fileSystem;
    EventReceiver *eventReceiver;
    gui::IGUISkin *skin;
    ITimer *timer;

    std::unique_ptr<World> world;

    void initializeDevice();
    void initializeGUI();
    void terminateDevice();
    void setSpriteBank(bool);

    void updateHUD();
    void handleSelecting();

public:
    Game(const struct ConfigData &data = ConfigData());
    ~Game();

    void start();
};
