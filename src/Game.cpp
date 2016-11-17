/* This file is part of PlaneRunner.
 *
 * PlaneRunner is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlaneRunner is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlaneRunner. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Game.h"

using namespace irr;

Game::Game(const ConfigData &data)
{
    // load configuration, initialize device and GUI
    configuration = data;
    if (!initializeDevice())
        return;
    initializeGUI();
    initialized = true;
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
    gui::IGUIFont *font = gui::CGUITTFont::createTTFont(driver, fileSystem, io::path("media/fonts/font.ttf"), 13, true, true);
    if (font)
        skin->setFont(font);

    gui = new GUI(configuration, *guiEnvironment);
    gui->addScreen(std::make_unique<MainMenuScreen>(configuration, *guiEnvironment), Screen::MAIN_MENU);
    gui->addScreen(std::make_unique<SettingsScreen>(configuration, *guiEnvironment), Screen::SETTINGS);
    gui->addScreen(std::make_unique<ControlSettingsScreen>(configuration, *guiEnvironment), Screen::CONTROL_SETTINGS);
    gui->addScreen(std::make_unique<PauseMenuScreen>(configuration, *guiEnvironment), Screen::PAUSE_MENU);
    gui->addScreen(std::make_unique<HUDScreen>(configuration, *guiEnvironment), Screen::HUD);
    gui->addScreen(std::make_unique<GameOverScreen>(configuration, *guiEnvironment), Screen::GAME_OVER);
}

bool Game::initializeDevice()
{
    // if fullscreen is enabled, create an empty device
    //      to get screen resolution
    if (configuration.fullscreen)
    {
        IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);
        configuration.resolution = nulldevice->getVideoModeList()->getDesktopResolution();
        nulldevice -> drop();
    }

    // create device (which is simply a window in which the
    //      whole world is rendered)
    device = createDevice(video::EDT_OPENGL, configuration.resolution, 32,
                          configuration.fullscreen, configuration.stencilBuffer, configuration.vsync);
    if (!device) {
        error("Couldn't create a device :(\n");
        return false;
    }
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

    return true;
}

// initializes bullet world
void Game::initializeBullet()
{
    // add broadphase interface
    broadphase = new btDbvtBroadphase();
    // configurate collision
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
    // add constraint solver
    solver = new btSequentialImpulseConstraintSolver;
    // create world
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, 0, 0));

    gContactProcessedCallback = [](btManifoldPoint &cp, void *obj0p, void *obj1p) -> bool
        {
            auto obj0 = static_cast<btCollisionObject *>(obj0p);
            auto obj1 = static_cast<btCollisionObject *>(obj1p);

            // if one of the objects is the place
            if (obj0->getUserIndex() == 1 || obj1->getUserIndex() == 1) {
                #if DEBUG_OUTPUT
                    std::cout << "Plane collision occured" << std::endl;
                    std::cout << "Collision impulse: " << cp.getAppliedImpulse() << std::endl;
                #endif // DEBUG_OUTPUT

                // obj0 must always be the plane
                if (obj1->getUserIndex() == 1)
                    std::swap(obj0, obj1);

                Plane &plane = *static_cast<Plane *>(obj0->getUserPointer());
                if (cp.getAppliedImpulse() > 400)
                    plane.setExploded(true);
                else if (!plane.getExploded())
                    plane.addScore(-cp.getAppliedImpulse());
            }

            return true;
        };
}

void Game::initializeScene()
{
    #if FOG_ENABLED
        driver->setFog(DEFAULT_COLOR, video::EFT_FOG_LINEAR, configuration.renderDistance - 300,
                       configuration.renderDistance, .003f, true, false);
    #endif // FOG_ENABLED

    initializeBullet();

    // create plane
    plane = new Plane(*dynamicsWorld, *device, btVector3(0, 0, 0));
    planeControl = new PlaneControl(*plane, configuration.controls);

    explosion = new Explosion(*dynamicsWorld, *device, plane->getPosition(), 1000); // create explosion

    #if DEBUG_DRAWER_ENABLED
        debugDrawer = new DebugDrawer(device);
        debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
        dynamicsWorld->setDebugDrawer(debugDrawer);
    #endif // DEBUG_DRAWER_ENABLED

    // create camera
    {
        camera = sceneManager->addCameraSceneNode(0);
        camera->setFarValue(configuration.renderDistance);
        updateCamera();
    }

    // add some light
    {
        light = sceneManager->addLightSceneNode(0, core::vector3df(0, 0, 0), DEFAULT_LIGHT_COLOR, 300);
        light->setLightType(video::ELT_DIRECTIONAL);
        video::SLight lightData;
        lightData = light->getLightData();
        lightData.DiffuseColor = DEFAULT_LIGHT_COLOR;
        lightData.AmbientColor = DEFAULT_LIGHT_COLOR;
        light->setLightData(lightData);
    }

    obstacleGenerator = new ObstacleGenerator(*device, *dynamicsWorld, camera->getFarValue(), 500);
}

void Game::error(const core::stringw &str) const
{
    std::wcerr << "Error: " << str.c_str() << std::endl;
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

void Game::terminateScene()
{
    delete explosion;
    delete plane;
    // IMPORTANT: obstacleGenerator must be deleted before dynamicsWorld and sceneManager
    delete obstacleGenerator;
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
    sceneManager->clear();
}

// show main menu
void Game::mainMenu()
{
    // if game is not initialized send error message and exit
    if (!initialized) {
        error("device has failed to initialize!");
        return;
    }

    // set resolution to actual screen size
    configuration.resolution = driver->getScreenSize();
    // initialize menu
    gui->initialize(Screen::MAIN_MENU);
    //sets cursor visible
    device->getCursorControl()->setVisible(true);

    size_t catchingControlID = CONTROLS_COUNT;

    ConfigData oldConfiguration = configuration;

    while (device->run()) {
        // handle gui events
        handleSelecting();

        switch (gui->getCurrentScreenIndex()) {
        case Screen::MAIN_MENU:
            if (eventReceiver->checkEvent(ID_BUTTON_START)) {
                if (run())
                    gui->initialize(Screen::MAIN_MENU);
                else
                    return;
            }
            if (eventReceiver->checkEvent(ID_BUTTON_SETTINGS)) {
                oldConfiguration = configuration;
                gui->initialize(Screen::SETTINGS);
            }
            if (eventReceiver->checkEvent(ID_BUTTON_QUIT) ||
                eventReceiver->checkKeyPressed(KEY_ESCAPE) ||
                eventReceiver->checkKeyPressed(KEY_LEFT))
                return;

            {
                core::stringw scrs = _w("Screen size: ");
                scrs += configuration.resolution.Width;
                scrs += "x";
                scrs += configuration.resolution.Height;
                gui->getCurrentScreenAsMainMenu().textScreenSize->setText(scrs.c_str());
            }
            break;

        case Screen::SETTINGS:
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
                if (!initializeDevice())
                    return;
                initializeGUI();
                initialized = true;
                oldConfiguration = configuration;

                gui->initialize(Screen::SETTINGS);
            }
            if (eventReceiver->checkEvent(ID_BUTTON_CONTROL_SETTINGS)) {
                gui->initialize(Screen::CONTROL_SETTINGS);
            }
            if (eventReceiver->checkEvent(ID_BUTTON_TOGGLE_FULLSCREEN)) {
                configuration.fullscreen = !configuration.fullscreen;

                terminateDevice();
                if (!initializeDevice())
                    return;
                initializeGUI();
                initialized = true;
                oldConfiguration = configuration;

                gui->initialize(Screen::SETTINGS);

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
                    if (!initializeDevice())
                        return;
                    initializeGUI();
                    initialized = true;
                }

                gui->initialize(Screen::MAIN_MENU);
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
            for (size_t i = 0; i < CONTROLS_COUNT; i++)
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
            }
            if (eventReceiver->checkEvent(ID_BUTTON_QUIT)) {
                return;
            }

            if (catchingControlID != CONTROLS_COUNT) {
                if (eventReceiver->lastKeyAvailable() &&
                    (!keyCodeName(eventReceiver->getLastKey()).empty() || eventReceiver->getLastKey() == KEY_ESCAPE)) {
                    if (!eventReceiver->checkKeyPressed(KEY_ESCAPE)) {
                        for (size_t i = 0; i < CONTROLS_COUNT; i++)
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
            if (IRIDESCENT_BACKGROUND)
                driver->beginScene(true, true, iridescentColor(timer->getTime()));
            else
                driver->beginScene(true, true, DEFAULT_COLOR);
            guiEnvironment->drawAll();
            driver->endScene();
        } else {
            device->yield();
        }
    }
}

// start the game itself
// returns false if quit is pressed
bool Game::run()
{
    // generate some chunks
    auto chunkDB = generateChunkDB();

    gui->initialize(Screen::HUD);
    initializeScene();

    video::SLight lightData;
    video::SColor color;

    constexpr unsigned int tick = 1000.0f / 60.0f;
    u32 timePrevious, timeCurrent;
    u64 accumulator, deltaTime = 0;

    timePrevious = timeCurrent = accumulator = timer->getTime();

    while (device->run())
    {
        color = iridescentColor(timer->getTime());

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
                camera->setAspectRatio((f32) configuration.resolution.Width / configuration.resolution.Height);
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

                // rendering
                #if FOG_ENABLED && IRIDESCENT_FOG
                    driver->setFog(color, video::EFT_FOG_LINEAR, configuration.renderDistance - 300,
                        configuration.renderDistance, 0.01f, true, true);
                #endif // FOG_ENABLED && IRIDESCENT_FOG
                #if IRIDESCENT_LIGHT
                    lightData = light->getLightData();
                    lightData.DiffuseColor = color;
                    lightData.AmbientColor = color;
                    light->setLightData(lightData);
                #endif // IRIDESCENT_LIGHT
                updateCamera(); // update camera position, target, and rotation
                #if DEBUG_DRAWER_ENABLED
                    dynamicsWorld->debugDrawWorld();
                #endif // DEBUG_DRAWER_ENABLED
                sceneManager->drawAll(); // draw scene

                if (eventReceiver->checkEvent(ID_BUTTON_RESUME) ||
                    eventReceiver->checkKeyPressed(KEY_ESCAPE))
                {
                    gui->initialize(Screen::HUD);
                }

                handleSelecting();

                if (eventReceiver->checkKeyPressed(KEY_LEFT) ||
                    eventReceiver->checkEvent(ID_BUTTON_MENU))
                {
                    terminateScene();
                    return true;
                }

                if (eventReceiver->checkEvent(ID_BUTTON_QUIT)) {
                    terminateScene();
                    return false;
                }

                timeCurrent = timePrevious = timer->getTime();
                accumulator = timer->getTime() - deltaTime;

                break;

            case Screen::GAME_OVER: {
                {
                    core::stringw score(_w("Your score: "));
                    score += plane->getScore();
                    gui->getCurrentScreenAsGameOver().textScore->setText(score.c_str());
                }

                // set cursor visible
                device->getCursorControl()->setVisible(true);

                // still continue simulation as we wanna see plane blowing up when we lose
                #if DEBUG_OUTPUT
                    std::cout << "=== BEGIN SIMULATION ===" << std::endl;;
                #endif // DEBUG_OUTPUT
                timeCurrent = timer->getTime();
                const float step = timeCurrent - timePrevious;
                dynamicsWorld->stepSimulation((step / 1000.0), 10, tick / 1000.0f);
                #if DEBUG_OUTPUT
                    std::cout << "Simulation step: " << step << "ms" << std::endl;
                #endif // DEBUG_OUTPUT
                timePrevious = timeCurrent;
                #if DEBUG_OUTPUT
                    std::cout << "=== END_SIMULATION ===" << std::endl << std::endl;
                #endif

                // rendering
                #if FOG_ENABLED && IRIDESCENT_FOG
                    driver->setFog(color, video::EFT_FOG_LINEAR, configuration.renderDistance - 300,
                        configuration.renderDistance, 0.01f, true, true);
                #endif // FOG_ENABLED && IRIDESCENT_FOG
                #if IRIDESCENT_LIGHT
                    lightData = light->getLightData();
                    lightData.DiffuseColor = color;
                    lightData.AmbientColor = color;
                    light->setLightData(lightData);
                #endif // IRIDESCENT_LIGHT
                #if DEBUG_DRAWER_ENABLED
                    dynamicsWorld->debugDrawWorld();
                #endif // DEBUG_DRAWER_ENABLED
                sceneManager->drawAll(); // draw scene

                handleSelecting();

                if (eventReceiver->checkEvent(ID_BUTTON_MENU)) {
                    terminateScene();
                    return true;
                }

                break;
            }
            case Screen::HUD: {
                #if DEBUG_OUTPUT
                    std::cout << "=== BEGIN SIMULATION ===" << std::endl;;
                #endif // DEBUG_OUTPUT

                // physics simulation
                timeCurrent = timer->getTime();
                const float step = timeCurrent - timePrevious;
                dynamicsWorld->stepSimulation((step / 1000.0), 10, tick / 1000.0f);
                #if DEBUG_OUTPUT
                    std::cout << "Simulation step: " << step << "ms" << std::endl;
                #endif // DEBUG_OUTPUT
                timePrevious = timeCurrent;

                if (eventReceiver->checkKeyPressed(KEY_ESCAPE)) {
                    gui->initialize(Screen::PAUSE_MENU);
                    continue;
                }
                if (eventReceiver->checkKeyPressed(KEY_F3)) {
                    gui->getCurrentScreenAsHUD().setInfoVisible(!gui->getCurrentScreenAsHUD().getInfoVisible());
                    gui->reload();
                }

                explosion->setPosition(plane->getPosition());

                if (plane->getExploded()) {
                    explosion->explode();
                    plane->disappear();

                    gui->initialize(Screen::GAME_OVER);
                    continue;
                }

                deltaTime = timer->getTime() - accumulator;
                #if DEBUG_OUTPUT
                    std::cout << "Control handling delta: " << deltaTime << "ms" << std::endl;
                #endif // DEBUG_OUTPUT
                while (deltaTime >= tick) {
                    deltaTime -= tick;
                    accumulator += tick;

                    obstacleGenerator->generate(plane->getNode().getPosition(), *chunkDB);
                    planeControl->handle(*eventReceiver); // handle plane controls
                    plane->addScore(2);
                }

                #if DEBUG_OUTPUT
                    std::cout << "=== END_SIMULATION ===" << std::endl << std::endl;
                #endif

                // rendering
                #if FOG_ENABLED && IRIDESCENT_FOG
                    driver->setFog(color, video::EFT_FOG_LINEAR, configuration.renderDistance - 300,
                        configuration.renderDistance, 0.01f, true, true);
                #endif // FOG_ENABLED && IRIDESCENT_FOG

                #if IRIDESCENT_LIGHT
                    lightData = light->getLightData();
                    lightData.DiffuseColor = color;
                    lightData.AmbientColor = color;
                    light->setLightData(lightData);
                #endif // IRIDESCENT_LIGHT

                updateHUD();
                updateCamera(); // update camera position, target, and rotation

                device->getCursorControl()->setVisible(false); //set cursor invisible

                #if DEBUG_DRAWER_ENABLED
                    dynamicsWorld->debugDrawWorld();
                #endif // DEBUG_DRAWER_ENABLED

                sceneManager->drawAll(); // draw scene

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

    terminateScene();
    return false;
}

void Game::updateHUD()
{
    // camera position
    {
        core::stringw cameraPosition = _w("Plane position: (");
        core::vector3df position = plane->getNode().getPosition();
        cameraPosition += position.X;
        cameraPosition += ", ";
        cameraPosition += position.Y;
        cameraPosition += ", ";
        cameraPosition += position.Z;
        cameraPosition += ")";
        gui->getCurrentScreenAsHUD().textCameraPosition->setText(cameraPosition.c_str());

        #if DEBUG_OUTPUT
            std::cout << "Plane position: (" << position.X << ", " << position.Y
                      << ", " << position.Z << ")" << std::endl;
        #endif // DEBUG_OUTPUT
    }

    // cube counter
    {
        core::stringw obstacles = _w("Obstacles: ");
        obstacles += obstacleGenerator->getCubeCount();
        gui->getCurrentScreenAsHUD().textObstaclesCount->setText(obstacles.c_str());

        #if DEBUG_OUTPUT
            std::cout << "Obstacles: " << obstacleGenerator->getCubeCount() << std::endl;
        #endif // DEBUG_OUTPUT
    }

    // fps counter
    {
        core::stringw fps = _w("FPS: ");
        fps += driver->getFPS();
        gui->getCurrentScreenAsHUD().textFPS->setText(fps.c_str());

        #if DEBUG_OUTPUT
            std::cout << "FPS: " << driver->getFPS() << std::endl;
        #endif // DEBUG_OUTPUT
    }

    // velocity counter
    {
        core::stringw velocity = _w("Linear velocity: ");
        velocity += (int) plane->getRigidBody().getLinearVelocity().length();
        velocity += _w("; Angular velocity: ");
        velocity += plane->getRigidBody().getAngularVelocity().length();
        gui->getCurrentScreenAsHUD().textVelocity->setText(velocity.c_str());

        #if DEBUG_OUTPUT
            std::cout << "Linear velocity: " << plane->getRigidBody().getLinearVelocity().length() << std::endl;
            std::cout << "Angular velocity: " << plane->getRigidBody().getAngularVelocity().length() << std::endl;
        #endif // DEBUG_OUTPUT
    }

    // rotation counter
    {
        btVector3 rotation = plane->getEulerRotation();

        core::stringw rotation_str = _w("Pitch: ");
        rotation_str += rotation.x();
        rotation_str += _w("; Yaw: ");
        rotation_str += rotation.y();
        rotation_str += _w("; Roll: ");
        rotation_str += rotation.z();
        gui->getCurrentScreenAsHUD().textAngle->setText(rotation_str.c_str());

        #if DEBUG_OUTPUT
            std::cout << "Pitch: " << rotation.x() << std::endl;
            std::cout << "Yaw: " << rotation.y() << std::endl;
            std::cout << "Roll: " << rotation.z() << std::endl;
        #endif // DEBUG_OUTPUT
    }

    // score counter
    {
        core::stringw score = _w("Score: ");
        score += plane->getScore();
        gui->getCurrentScreenAsHUD().textScore->setText(score.c_str());

        #if DEBUG_OUTPUT
            std::cout << "Score: " << plane->getScore() << std::endl;
        #endif // DEBUG_OUTPUT
    }
}

void Game::updateCamera()
{
    core::vector3df upVector(0, 1, 0);
    upVector.rotateXYBy(plane->getEulerRotation().z() * core::RADTODEG64);

    camera->setPosition(plane->getNode().getPosition() + upVector * 0.3f * CAMERA_DISTANCE
        - core::vector3df(0, 0, CAMERA_DISTANCE));
    camera->setUpVector(upVector);

    camera->setTarget(camera->getPosition() + core::vector3df(0, 0, 1));
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

    constexpr std::size_t THREADS = 8;
    constexpr std::size_t CHUNKS_PER_THREAD = CHUNK_DB_SIZE / THREADS;

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
    threads.emplace_back(generateRange, (THREADS - 1) * CHUNKS_PER_THREAD, chunkDB->size());

    // waiting for all the threads to get done
    for (auto &thread : threads)
        thread.join();

    return chunkDB;
}
