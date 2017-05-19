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

#include "Game.h"

using namespace irr;

Game::Game(const ConfigData &data)
{
    // load configuration, initialize device and GUI
    configuration = data;
    initializeDevice();
    initializeGUI();
}

Game::~Game()
{
    terminateDevice();
}

void Game::start()
{
    mainMenu();
}

void Game::initializeGUI()
{
    // load font
    auto *font = gui::CGUITTFont::createTTFont(driver, fileSystem,
                                               io::path("media/fonts/font.ttf"), 13, true, true);
    if (font)
        skin->setFont(font);
    font = nullptr;

    Game::setSpriteBank(false);
    Game::setSpriteBank(true);
    gui = std::make_unique<GUI>(configuration, *guiEnvironment);
    gui->addScreen(std::make_unique<MainMenuScreen>(configuration, *guiEnvironment), Screen::MAIN_MENU);
    gui->addScreen(std::make_unique<SettingsScreen>(configuration, *guiEnvironment), Screen::SETTINGS);
    gui->addScreen(std::make_unique<ControlSettingsScreen>(configuration, *guiEnvironment), Screen::CONTROL_SETTINGS);
    gui->addScreen(std::make_unique<PauseMenuScreen>(configuration, *guiEnvironment), Screen::PAUSE_MENU);
    gui->addScreen(std::make_unique<HUDScreen>(configuration, *guiEnvironment), Screen::HUD);
    gui->addScreen(std::make_unique<GameOverScreen>(configuration, *guiEnvironment), Screen::GAME_OVER);
    gui->addScreen(std::make_unique<ScoreboardScreen>(configuration, *guiEnvironment), Screen::SCOREBOARD);
    device->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_BUTTON_TEXT,video::SColor(255,200,200,200));
}

void Game::initializeDevice()
{
    if (initialized)
        throw repeated_initialization();

    // if fullscreen is enabled, create an empty device
    //      to get screen resolution
    if (configuration.fullscreen)
    {
        IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);
        if (!nulldevice)
            throw initialization_error();
        configuration.resolution = nulldevice->getVideoModeList()->getDesktopResolution();
        nulldevice -> drop();
    }

    // create device (which is simply a window in which the
    //      whole world is rendered)
    device = createDevice(video::EDT_OPENGL, configuration.resolution, 32,
                                   configuration.fullscreen, configuration.stencilBuffer,
                                   configuration.vsync);
    if (!device)
        throw initialization_error();
    device->setWindowCaption(L"PlaneRunner");

    // get a lot of useful pointers from device
    timer = device->getTimer();
    driver = device->getVideoDriver();
    sceneManager = device->getSceneManager();
    guiEnvironment = device->getGUIEnvironment();
    skin = guiEnvironment->getSkin();
    fileSystem = device->getFileSystem();
    eventReceiver = new EventReceiver();
    device->setEventReceiver(eventReceiver);
    device->setResizable(configuration.resizable);

    timer->setTime(0);
    timer->start();

    initialized = true;
}

void Game::terminateDevice()
{
    Config::saveConfig("game.conf", configuration);
    gui->terminate();
    device->closeDevice();
    device->run();
    device->drop();

    initialized = false;
}

void Game::setSpriteBank(bool isControlButton)
{
    // add textures into sprite bank
    gui::IGUISpriteBank *spriteBank;
    if (!isControlButton) {
        spriteBank = guiEnvironment->addEmptySpriteBank("SpritesForRegularButtons");
        spriteBank->addTexture(driver->getTexture("media/textures/button_up.png")); // 0
        spriteBank->addTexture(driver->getTexture("media/textures/button_over.png")); // 1
        spriteBank->addTexture(driver->getTexture("media/textures/button_down.png")); // 2
        spriteBank->addTexture(driver->getTexture("media/textures/button_focused.png")); // 3
        // add sizes
        spriteBank->getPositions().push_back(core::rect<s32>(core::position2di(0,0),
                (driver->getTexture("media/textures/button_up.png"))->getOriginalSize()));
        spriteBank->getPositions().push_back(core::rect<s32>(core::position2di(0,0),
                (driver->getTexture("media/textures/button_over.png"))->getOriginalSize()));
        spriteBank->getPositions().push_back(core::rect<s32>(core::position2di(0,0),
                (driver->getTexture("media/textures/button_down.png"))->getOriginalSize()));
        spriteBank->getPositions().push_back(core::rect<s32>(core::position2di(0,0),
                (driver->getTexture("media/textures/button_focused.png"))->getOriginalSize()));
    } else {
        spriteBank = guiEnvironment->addEmptySpriteBank("SpritesForControlButtons");
        spriteBank->addTexture(driver->getTexture("media/textures/cbutton_up.png")); // 0
        spriteBank->addTexture(driver->getTexture("media/textures/cbutton_over.png")); // 1
        spriteBank->addTexture(driver->getTexture("media/textures/cbutton_down.png")); // 2
        spriteBank->addTexture(driver->getTexture("media/textures/cbutton_focused.png")); // 3
        // add sizes
        spriteBank->getPositions().push_back(core::rect<s32>(core::position2di(0,0),
                (driver->getTexture("media/textures/cbutton_up.png"))->getOriginalSize()));
        spriteBank->getPositions().push_back(core::rect<s32>(core::position2di(0,0),
                (driver->getTexture("media/textures/cbutton_over.png"))->getOriginalSize()));
        spriteBank->getPositions().push_back(core::rect<s32>(core::position2di(0,0),
                (driver->getTexture("media/textures/cbutton_down.png"))->getOriginalSize()));
        spriteBank->getPositions().push_back(core::rect<s32>(core::position2di(0,0),
                (driver->getTexture("media/textures/cbutton_focused.png"))->getOriginalSize()));
    }
    // add sprites
    gui::SGUISprite sprite;
    gui::SGUISpriteFrame sframe;
    sprite.Frames.push_back(sframe);
    for (int i = 0; i <= 3; i++) {
        sprite.Frames[0].rectNumber = i;
        sprite.Frames[0].textureNumber = i;
        spriteBank->getSprites().push_back(sprite);
    }
}

// show main menu
void Game::mainMenu()
{
    sf::Sound menu = Audio::getInstance().menu();
    menu.setLoop(true);
    menu.setVolume(configuration.volume);
    menu.play();

    // set resolution to actual screen size
    configuration.resolution = driver->getScreenSize();
    // initialize menu
    gui->initialize(Screen::MAIN_MENU);
    //sets cursor visible
    device->getCursorControl()->setVisible(true);

    std::size_t catchingControlID = CONTROLS_COUNT;

    ConfigData oldConfiguration = configuration;

    while (device->run()) {
        // handle gui events
        handleSelecting();

        switch (gui->getCurrentScreenIndex()) {
        case Screen::MAIN_MENU:
            if (eventReceiver->checkEvent(ID_BUTTON_START)) {
                auto chunkDB = generateChunkDB();
                menu.pause();
                if (run(std::move(chunkDB)))
                {
                    menu.play();
                    gui->initialize(Screen::MAIN_MENU);
                    continue;
                }
                else
                    return;
            }
            else if (eventReceiver->checkEvent(ID_BUTTON_SCOREBOARD)) {
                gui->initialize(Screen::SCOREBOARD);
                continue;
            }
            else if (eventReceiver->checkEvent(ID_BUTTON_SETTINGS)) {
                oldConfiguration = configuration;
                gui->initialize(Screen::SETTINGS);
                continue;
            }
            else if (eventReceiver->checkEvent(ID_BUTTON_QUIT) ||
                eventReceiver->checkKeyPressed(KEY_ESCAPE) ||
                eventReceiver->checkKeyPressed(KEY_LEFT))
                return;
            break;
        case Screen::SCOREBOARD:
            if (eventReceiver->checkEvent(ID_BUTTON_MENU) ||
                eventReceiver->checkKeyPressed(KEY_ESCAPE) ||
                eventReceiver->checkKeyPressed(KEY_LEFT)) {
                gui->initialize(Screen::MAIN_MENU);
                continue;
            } else if (eventReceiver->checkEvent(ID_BUTTON_QUIT))
                return;
            break;
        case Screen::SETTINGS:
  	    if (eventReceiver->checkEvent(ID_SCROLLBAR_VOLUME)) {
  	        configuration.volume = gui->getCurrentScreenAsSettings().scrollBarVolume->getPos();
		menu.setVolume(configuration.volume);
	    }
            if (eventReceiver->checkEvent(ID_COMBOBOX_LANGUAGE)) {
                switch (gui->getCurrentScreenAsSettings().comboBoxLanguage->getSelected()) {
                case 0:
                    configuration.language = "";
                    break;
                case 1:
                    configuration.language = "en";
                    break;
                case 2:
                    configuration.language = "ru";
                    break;
                case 3:
                    configuration.language = "crh";
                    break;
                case 4:
                    configuration.language = "crh@cyrillic";
                    break;
                default:
                    configuration.language = "";
                }
                setLanguage(configuration.language, true);
                gui->reload();
            }
            if (eventReceiver->checkEvent(ID_COMBOBOX_RESOLUTION)) {
                switch (gui->getCurrentScreenAsSettings().comboBoxResolution->getSelected()) {
                case 0:
                        configuration.resolution = core::dimension2d<u32>(640, 480);
                        configuration.resizable = false;
                        break;
                case 1:
                        configuration.resolution = core::dimension2d<u32>(1240, 720);
                        configuration.resizable = false;
                        break;
                case 2:
                        configuration.resizable = true;
                        break;
                }

                terminateDevice();
                initializeDevice();
                initializeGUI();
                oldConfiguration = configuration;

                gui->initialize(Screen::SETTINGS);
                continue;
            }
            if (eventReceiver->checkEvent(ID_BUTTON_CONTROL_SETTINGS)) {
                gui->initialize(Screen::CONTROL_SETTINGS);
            }
            if (eventReceiver->checkEvent(ID_BUTTON_TOGGLE_FULLSCREEN)) {
                configuration.fullscreen = !configuration.fullscreen;

                terminateDevice();
                initializeDevice();
                initializeGUI();
                oldConfiguration = configuration;

                gui->initialize(Screen::SETTINGS);
                continue;
            }
            if (eventReceiver->checkEvent(ID_SPINBOX_RENDER_DISTANCE)) {
                configuration.renderDistance = gui->getCurrentScreenAsSettings().spinBoxRenderDistance->getValue();
            }
            if (eventReceiver->checkEvent(ID_CHECKBOX_VSYNC)) {
                configuration.vsync = gui->getCurrentScreenAsSettings().checkBoxVSync->isChecked();
            }
            if (eventReceiver->checkEvent(ID_CHECKBOX_STENCILBUFFER)) {
                configuration.stencilBuffer = gui->getCurrentScreenAsSettings().checkBoxStencilBuffer->isChecked();
            }
            if (eventReceiver->checkEvent(ID_BUTTON_MENU) ||
                eventReceiver->checkKeyPressed(KEY_ESCAPE) ||
                eventReceiver->checkKeyPressed(KEY_LEFT))
            {
                bool needRestart = configuration.needRestart(oldConfiguration);
                if (needRestart) {
                    terminateDevice();
                    initializeDevice();
                    initializeGUI();
                }

                gui->initialize(Screen::MAIN_MENU);
                continue;
            }
            if (eventReceiver->checkEvent(ID_BUTTON_QUIT)) {
                return;
            }

            {
                core::stringw scrs = _w("Screen size: ");
                scrs += configuration.resolution.Width;
                scrs += "x";
                scrs += configuration.resolution.Height;
                gui->getCurrentScreenAsSettings().textScreenSize->setText(scrs.c_str());
            }

            break;

        case Screen::CONTROL_SETTINGS:
            for (std::size_t i = 0; i < CONTROLS_COUNT; i++)
                if (eventReceiver->checkEvent(static_cast<GUI_ID>(gui->getCurrentScreenAsControlSettings().buttonsControl[i]->getID()))) {
                    eventReceiver->startCatchingKey();
                    catchingControlID = i;
                    gui->reload();
                    gui->getCurrentScreenAsControlSettings().buttonsControl[i]->setText(_wp("Press a key"));
                }
            if (eventReceiver->checkEvent(ID_BUTTON_DEFAULT_CONTROLS)) {
                eventReceiver->stopCatchingKey();
                catchingControlID = CONTROLS_COUNT;
                configuration.controls = Controls();
                gui->reload();
            }
            if (eventReceiver->checkEvent(ID_BUTTON_SETTINGS)) {
                eventReceiver->stopCatchingKey();
                catchingControlID = CONTROLS_COUNT;
                gui->initialize(Screen::SETTINGS);
                continue;
            }
            if (eventReceiver->checkEvent(ID_BUTTON_QUIT)) {
                return;
            }

            if (catchingControlID != CONTROLS_COUNT) {
                if (eventReceiver->lastKeyAvailable() &&
                    (!keyCodeName(eventReceiver->getLastKey()).empty() || eventReceiver->getLastKey() == KEY_ESCAPE)) {
                    if (!eventReceiver->checkKeyPressed(KEY_ESCAPE)) {
                        for (std::size_t i = 0; i < CONTROLS_COUNT; i++)
                            if (configuration.controls[i] == eventReceiver->getLastKey())
                                configuration.controls[i] = KEY_KEY_CODES_COUNT;
                        configuration.controls[catchingControlID] = eventReceiver->getLastKey();
                    }

                    eventReceiver->stopCatchingKey();
                    catchingControlID = CONTROLS_COUNT;
                    gui->reload();
                }
            } else {
                if (eventReceiver->checkKeyPressed(KEY_ESCAPE) ||
                    eventReceiver->checkKeyPressed(KEY_LEFT)) {
                    gui->initialize(Screen::SETTINGS);
                    continue;
                }
            }

            {
                core::stringw scrs = _w("Screen size: ");
                scrs += configuration.resolution.Width;
                scrs += "x";
                scrs += configuration.resolution.Height;
                gui->getCurrentScreenAsControlSettings().textScreenSize->setText(scrs.c_str());
            }
            break;
        default:
            break;
        }

        // catch window resize
        if (configuration.resolution != driver->getScreenSize()) {
            oldConfiguration.resolution = configuration.resolution = driver->getScreenSize();
            gui->resize();
        }

        if (device->isWindowActive()) {
            //if (IRIDESCENT_BACKGROUND)
                driver->beginScene(true, true, iridescentColor(timer->getTime()));
            //else
            //    driver->beginScene(true, true, DEFAULT_COLOR);
            guiEnvironment->drawAll();
            driver->endScene();
        } else {
            device->yield();
        }
    }
}

// start the game itself
// returns false if quit is pressed
bool Game::run(std::unique_ptr<ChunkDB> chunkDB)
{
    // start background music
    sf::Sound background = Audio::getInstance().background();
    background.setLoop(true);
    background.setVolume(configuration.volume);
    background.play();

    gui->initialize(Screen::HUD);
    world = std::make_unique<World>(*device, configuration, *chunkDB);
    planeControl = std::make_unique<PlaneControl>(world->plane(), configuration.controls);

    constexpr unsigned int tick = 1000.0f / 60.0f;
    u32 timePrevious, timeCurrent;
    u64 accumulator, deltaTime = 0;

    timePrevious = timeCurrent = accumulator = timer->getTime();

    while (device->run())
    {
        video::SColor color = iridescentColor(timer->getTime());

        if (device->isWindowActive()) {
            #if IRIDESCENT_BACKGROUND
                driver->beginScene(true, true, color);
            #else
                driver->beginScene(true, true, DEFAULT_COLOR);
            #endif // IRIDESCENT_BACKGROUND

            // catch window resize
            if (configuration.resolution != driver->getScreenSize()) {
                configuration.resolution = driver->getScreenSize();
                gui->resize();
                world->updateAspectRatio();
            }

            switch (gui->getCurrentScreenIndex()) {
            case Screen::PAUSE_MENU:
                // screen size
                {
                    core::stringw scrs = _w("Screen size: ");
                    scrs += configuration.resolution.Width;
                    scrs += "x";
                    scrs += configuration.resolution.Height;
                    gui->getCurrentScreenAsPauseMenu().textScreenSize->setText(scrs.c_str());
                }

                // set cursor visible
                device->getCursorControl()->setVisible(true);
                world->render(color);

                if (eventReceiver->checkEvent(ID_BUTTON_RESUME) ||
                    eventReceiver->checkKeyPressed(KEY_ESCAPE))
                {
                    gui->initialize(Screen::HUD);
                }

                handleSelecting();

                if (eventReceiver->checkKeyPressed(KEY_LEFT) ||
                    eventReceiver->checkEvent(ID_BUTTON_MENU))
                {
                    world.reset();
                    return true;
                }

                if (eventReceiver->checkEvent(ID_BUTTON_QUIT)) {
                    world.reset();
                    return false;
                }

                timeCurrent = timePrevious = timer->getTime();
                accumulator = timer->getTime() - deltaTime;

                break;

            case Screen::GAME_OVER: {
                {
                    core::stringw score(_w("Your score: "));
                    score += world->plane().score();
                    gui->getCurrentScreenAsGameOver().textScore->setText(score.c_str());
                }

                // set cursor visible
                device->getCursorControl()->setVisible(true);

                // still continue simulation as we wanna see plane blowing up when we lose
                Log::getInstance().debug("=== BEGIN SIMULATION STEP ===");

                timeCurrent = timer->getTime();
                const float step = timeCurrent - timePrevious;
                world->stepSimulation((step / 1000.0), 10, tick / 1000.0f);
                timePrevious = timeCurrent;
                Log::getInstance().debug("=== END SIMULATION STEP ===");

                world->render(color);
                handleSelecting();

                if (eventReceiver->checkEvent(ID_BUTTON_MENU)) {
                    world.reset();
                    return true;
                }

                break;
            }
            case Screen::HUD: {
                Log::getInstance().debug("=== BEGIN SIMULATION STEP ===");

                // physics simulation
                timeCurrent = timer->getTime();
                const float step = timeCurrent - timePrevious;
                world->stepSimulation((step / 1000.0), 10, tick / 1000.0f);
                timePrevious = timeCurrent;

                if (eventReceiver->checkKeyPressed(KEY_ESCAPE)) {
                    gui->initialize(Screen::PAUSE_MENU);
                    continue;
                }
                if (eventReceiver->checkKeyPressed(KEY_F3)) {
                    gui->getCurrentScreenAsHUD().setInfoVisible(!gui->getCurrentScreenAsHUD().getInfoVisible());
                    gui->reload();
                }

                if (world->gameOver()) {
                    background.stop();

                    gui->initialize(Screen::GAME_OVER);
                    std::vector<s32> score = Scoreboard::loadScore("score.txt");
                    score.push_back((world->plane().score()));
                    std::sort(score.begin(), score.end(), std::greater<int>());
                    Scoreboard::saveScore("score.txt", score);
                    continue;
                }

                deltaTime = timer->getTime() - accumulator;
                Log::getInstance().debug("generation and control handling delta = ", deltaTime, "ms");
                while (deltaTime >= tick) {
                    deltaTime -= tick;
                    accumulator += tick;

                    world->generate();
                    planeControl->handle(*eventReceiver); // handle plane controls
                    world->plane().addScore(2);
                }

                Log::getInstance().debug("=== END SIMULATION STEP ===");

                world->render(color);
                device->getCursorControl()->setVisible(false); //set cursor invisible
                updateHUD();

                break;
            }
            default:
                break;
            }

            guiEnvironment->drawAll();
            driver->endScene();
        } else {
            if (gui->getCurrentScreenIndex() == Screen::PAUSE_MENU) {
                timeCurrent = timePrevious = timer->getTime();
                accumulator = timer->getTime() - deltaTime;
                device->yield();
            } else
                gui->initialize(Screen::PAUSE_MENU);
        }
    }

//    Audio::getInstance().background.stop();
    world.reset();
    return false;
}

void Game::updateHUD()
{
    // camera position
    {
        core::stringw cameraPosition = _w("Plane position: (");
        core::vector3df position = world->plane().node().getPosition();
        cameraPosition += position.X;
        cameraPosition += ", ";
        cameraPosition += position.Y;
        cameraPosition += ", ";
        cameraPosition += position.Z;
        cameraPosition += ")";
        gui->getCurrentScreenAsHUD().textCameraPosition->setText(cameraPosition.c_str());

        Log::getInstance().debug("plane position = (", position.X, ", ", position.Y, ", ", position.Z, ")");
    }

    // cube counter
    {
        core::stringw obstacles = _w("Obstacles: ");
        obstacles += world->obstacles();
        gui->getCurrentScreenAsHUD().textObstaclesCount->setText(obstacles.c_str());

        Log::getInstance().debug("obstacles = ", world->obstacles());
    }

    // fps counter
    {
        core::stringw fps = _w("FPS: ");
        fps += driver->getFPS();
        gui->getCurrentScreenAsHUD().textFPS->setText(fps.c_str());

        Log::getInstance().debug("FPS = ", driver->getFPS());
    }

    // velocity counter
    {
        core::stringw velocity = _w("Linear velocity: ");
        velocity += (int) world->plane().rigidBody().getLinearVelocity().length();
        velocity += _w("; Angular velocity: ");
        velocity += world->plane().rigidBody().getAngularVelocity().length();
        gui->getCurrentScreenAsHUD().textVelocity->setText(velocity.c_str());

        Log::getInstance().debug("linear velocity = ", world->plane().getLinearVelocity().length());
        Log::getInstance().debug("angular velocity = ", world->plane().getLinearVelocity().length());
    }

    // rotation counter
    {
        btVector3 rotation = world->plane().getEulerRotationDeg();

        core::stringw rotation_str = _w("Pitch: ");
        rotation_str += rotation.x();
        rotation_str += _w("°; Yaw: ");
        rotation_str += rotation.y();
        rotation_str += _w("°; Roll: ");
        rotation_str += rotation.z();
        rotation_str += _w("°");
        gui->getCurrentScreenAsHUD().textAngle->setText(rotation_str.c_str());

        Log::getInstance().debug("rotation = (", rotation.x(), ", ", rotation.y(), ", ", rotation.z(), ")");
    }

    // score counter
    {
        core::stringw score = _w("Score: ");
        score += world->plane().score();
        gui->getCurrentScreenAsHUD().textScore->setText(score.c_str());

        Log::getInstance().debug("score = ", world->plane().score());
    }
}

void Game::handleSelecting()
{
    if (guiEnvironment->getFocus() && eventReceiver->isKeyDown(KEY_TAB))
        gui->selectWithTab();

    if (eventReceiver->checkKeyPressed(KEY_DOWN)) {
        if (!guiEnvironment->getFocus())
            gui->selectElement(0);
        else
            gui->selectNextElement();
    }

    if (eventReceiver->checkKeyPressed(KEY_UP)) {
        if (!guiEnvironment->getFocus())
            gui->selectElement(0);
        else
            gui->selectPreviousElement();
    }

    if (eventReceiver->checkKeyPressed(KEY_RIGHT))
        if (guiEnvironment->getFocus() && guiEnvironment->getFocus()->getType() == gui::EGUIET_BUTTON) {
            SEvent event;
            event.EventType = EET_GUI_EVENT;
            event.GUIEvent.Caller = guiEnvironment->getFocus();
            event.GUIEvent.Element = guiEnvironment->getFocus();
            event.GUIEvent.EventType = gui::EGET_BUTTON_CLICKED;
            device->postEventFromUser(event);
        }
}


std::unique_ptr<ChunkDB> Game::generateChunkDB()
{
    auto chunkDB = std::make_unique<ChunkDB>();

    static const std::size_t THREADS = std::max(1u, std::thread::hardware_concurrency());

    static const std::size_t CHUNKS_PER_THREAD = CHUNK_DB_SIZE / THREADS;

    auto generateRange =
        [&chunkDB](std::size_t begin, std::size_t end) mutable
        {
            for (std::size_t i = begin; i < end; i++)
                chunkDB->at(i).generate();
        };

    std::vector<std::thread> threads;

    // first THREADS - 1 pieces
    for (std::size_t i = 0; i < THREADS - 1; i++)
        threads.emplace_back(generateRange, i * CHUNKS_PER_THREAD, (i + 1) * CHUNKS_PER_THREAD);

    // last piece
    generateRange((THREADS - 1) * CHUNKS_PER_THREAD, chunkDB->size());

    // waiting for all the threads to get done
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    return chunkDB;
}
