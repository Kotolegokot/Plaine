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
            btCollisionObject *obj0 = static_cast<btCollisionObject *>(obj0p);
            btCollisionObject *obj1 = static_cast<btCollisionObject *>(obj1p);

            if (obj0->getUserIndex() == 1 || obj1->getUserIndex() == 1) {
                #if DEBUG_OUTPUT
                    std::cout << "Plane collision occured" << std::endl;
                    std::cout << "Collision impulse: " << cp.getAppliedImpulse() << std::endl;
                #endif // DEBUG_OUTPUT

                // obj0 is always the plane
                if (obj1->getUserIndex() == 1)
                    std::swap(obj0, obj1);

                Plane &plane = *static_cast<Plane *>(obj0->getUserPointer());
                plane.addScore(-cp.getAppliedImpulse());

                if (cp.getAppliedImpulse() > 400)
                    plane.setExploded(true);
            }

            return true;
        };
}

void Game::initializeScene()
{
    if (FOG_ENABLED)
        driver->setFog(DEFAULT_COLOR, video::EFT_FOG_LINEAR, configuration.renderDistance - 300, configuration.renderDistance, .003f, true, false);

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
    gui->initialize(Screen::HUD);
    initializeScene();

    video::SLight lightData;
    video::SColor color;

    constexpr unsigned int TickMs = 32;
    u32 time_physics_prev, time_physics_curr;
    u64 time_gameclock;

    time_physics_prev = time_physics_curr = time_gameclock = timer->getTime();

    while (device->run())
    {
        color = iridescentColor(timer->getTime());

        if (device->isWindowActive()) {
            #if IRIDESCENT_BACKGROUND
                driver->beginScene(true, true, color);
            #else
                driver->beginScene(true, true, DEFAULT_COLOR);
            #endif // IRIDESCENT_BACKGROUND


            switch (gui->getCurrentScreenIndex()) {
            case Screen::PAUSE_MENU:
                // catch window resize
                if (configuration.resolution != driver->getScreenSize()) {
                    configuration.resolution = driver->getScreenSize();
                    gui->resize();
                }

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

                time_physics_curr = time_physics_prev = timer->getTime();
                break;
            case Screen::HUD: {
                #if DEBUG_OUTPUT
                    std::cout << "=== BEGIN SIMULATION ===" << std::endl;;
                #endif // DEBUG_OUTPUT

                // set fog color
                #if FOG_ENABLED && IRIDESCENT_FOG
                    driver->setFog(color, video::EFT_FOG_LINEAR, configuration.renderDistance - 300,
                        configuration.renderDistance, 0.01f, true, true);
                #endif // FOG_ENABLED && IRIDESCENT_FOG

                // set light color
                #if IRIDESCENT_LIGHT
                    lightData = light->getLightData();
                    lightData.DiffuseColor = color;
                    lightData.AmbientColor = color;
                    light->setLightData(lightData);
                #endif // IRIDESCENT_LIGHT

                updateHUD();
                updateCamera(); // update camera position, target, and rotation

                //set cursor invisible
                device->getCursorControl()->setVisible(false);

                // generate obstacles
                obstacleGenerator->generate(plane->getNode().getPosition());

                if (eventReceiver->checkKeyPressed(KEY_ESCAPE))
                    gui->initialize(Screen::PAUSE_MENU);
                if (eventReceiver->checkKeyPressed(KEY_F3))
                    gui->getCurrentScreenAsHUD().setInfoVisible(!gui->getCurrentScreenAsHUD().getInfoVisible());

                explosion->setPosition(plane->getPosition());
                sceneManager->drawAll(); // draw scene

                if (plane->getExploded()) {
                    explosion->explode();
                    plane->setExploded(false);
                }

                time_physics_curr = timer->getTime();
                // physics simulation
                const float step = time_physics_curr - time_physics_prev;
                dynamicsWorld->stepSimulation((step / 1000.0), 10);
                #if DEBUG_OUTPUT
                    std::cout << "Simulation step: " << step << "ms" << std::endl;
                #endif // DEBUG_OUTPUT
                time_physics_prev = time_physics_curr;
                #if DEBUG_DRAWER_ENABLED
                    dynamicsWorld->debugDrawWorld();
                #endif // DEBUG_DRAWER_ENABLED
                u64 dt = timer->getTime() - time_gameclock;

                #if DEBUG_OUTPUT
                    std::cout << "Control handling delta: " << dt << "ms" << std::endl;
                #endif // DEBUG_OUTPUT
                while (dt >= TickMs) {
                    dt -= TickMs;
                    time_gameclock += TickMs;

                    planeControl->handle(*eventReceiver); // handle plane controls
                    plane->addScore(2);
                }

                #if DEBUG_OUTPUT
                    std::cout << "=== END_SIMULATION ===" << std::endl << std::endl;
                #endif
                break;
            }
            default:
                break;
            }

            guiEnvironment->drawAll();
            driver->endScene();
        } else {
            if (gui->getCurrentScreenIndex() == Screen::PAUSE_MENU) {
                time_physics_curr = time_physics_prev = timer->getTime();
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
        velocity += _w(", angular velocity: ");
        velocity += plane->getRigidBody().getAngularVelocity().length();
        gui->getCurrentScreenAsHUD().textVelocity->setText(velocity.c_str());

        #if DEBUG_OUTPUT
            std::cout << "Linear velocity: " << plane->getRigidBody().getLinearVelocity().length() << std::endl;
            std::cout << "Angular velocity: " << plane->getRigidBody().getAngularVelocity().length() << std::endl;
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
