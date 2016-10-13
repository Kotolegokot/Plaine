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

Game::Game(const struct ConfigData &data)
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

void Game::initializeGUI()
{
    // load font
    gui::IGUIFont *font = gui::CGUITTFont::createTTFont(driver, fileSystem, io::path("media/fonts/font.ttf"), 13, true, true);
    if (font)
        skin->setFont(font);

    gui = new GUI(configuration, guiEnvironment);
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
    device = createDevice(video::EDT_OPENGL, configuration.resolution, 32, configuration.fullscreen, configuration.stencilBuffer, configuration.vsync);
    if (!device) {
        error("Couldn't create a device :(\n");
        return false;
    }
    device->setWindowCaption(L"PlaneTest");

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
}

void Game::initializeScene()
{
    if (FOG_ENABLED)
        driver->setFog(DEFAULT_COLOR, video::EFT_FOG_LINEAR, configuration.renderDistance - 300, configuration.renderDistance, .003f, true, false);

    initializeBullet();

    // create plane
    plane = new Plane(dynamicsWorld, device, btVector3(0, 0, 0));
    planeControl = new PlaneControl(plane, configuration.controls);

    debugDrawer = new DebugDrawer(device);
    if (DEBUG_DRAWER_ENABLED)
        debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    dynamicsWorld->setDebugDrawer(debugDrawer);

    // create camera
    {
        camera = sceneManager->addCameraSceneNode(0);
        camera->setFarValue(configuration.renderDistance);

        // set camera position and rotation
        core::vector3df upVector(0, 1, 0);
        upVector.rotateXYBy(plane->getEulerRotation().z() * core::RADTODEG64);

        camera->setPosition(plane->getNode().getPosition() + upVector * 0.3f * CAMERA_DISTANCE
                - core::vector3df(0, 0, CAMERA_DISTANCE));
        camera->setUpVector(upVector);

        camera->setTarget(camera->getPosition() + core::vector3df(0, 0, 1));
    }


    // add some light
    light = sceneManager->addLightSceneNode(camera, core::vector3df(0, 0, -100), DEFAULT_LIGHT_COLOR, 300);
    light->setLightType(video::ELT_DIRECTIONAL);
    video::SLight lightData;
    lightData = light->getLightData();
    lightData.DiffuseColor = DEFAULT_LIGHT_COLOR;
    lightData.AmbientColor = DEFAULT_LIGHT_COLOR;
    light->setLightData(lightData);

    // create obstacle generator
    obstacleGenerator = new ObstacleGenerator(device, dynamicsWorld, camera->getFarValue(), 500);
}

void Game::error(const core::stringw &str) const
{
    std::wcerr << "Error: " << str.c_str() << std::endl;
}

void Game::terminateDevice()
{
    Config conf;
    conf.saveConfig("game.conf", configuration);
    gui->terminate();
    device->closeDevice();
    device->run();
    device->drop();
}

void Game::terminateScene()
{
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
void Game::menu()
{
    // if game is not initialized send error message and exit
    if (!initialized) {
        error(ERR_NOT_INITIALIZED);
        return;
    }
    // set resolution to actual screen size
    configuration.resolution = driver->getScreenSize();
    // initialize menu
    gui->initialize(MENU);
    //sets cursor visible
    device->getCursorControl()->setVisible(true);
    while (device->run()) {
        // if start button is pressed then run the game
        if (eventReceiver->state == HUD) {
            gui->terminate();
            run();
        }
        // if quit button is pressed then exit the game
        if (eventReceiver->quit) {
            break;
        }
        // escape key reactions in different GUI states
        if (eventReceiver->IsKeyDown(KEY_ESCAPE)) {
            if (!eventReceiver->escapePressed)
            {
                eventReceiver->escapePressed = true;
                if (eventReceiver->state == MENU)
                    return;
                else if (eventReceiver->state == SETTINGS)
                    {
                        eventReceiver->state = MENU;
                        eventReceiver->toggleGUI = true;
                    }
                else if (eventReceiver->state == CONTROL_SETTINGS)
                    {
                        eventReceiver->state = SETTINGS;
                        eventReceiver->toggleGUI = true;
                    }
            }
        }
        else if (!eventReceiver->IsKeyDown(KEY_ESCAPE))
            eventReceiver->escapePressed = false;
        // if window need restart to implement new graphic settings
        if (gui->getState() == SETTINGS && eventReceiver->state == MENU && eventReceiver->needRestartInMenu)
            {
                gui->terminate();
                terminateDevice();
                if (!initializeDevice())
                    return;
                initializeGUI();
                initialized = true;
                gui->initialize(MENU);
                eventReceiver->needRestartInMenu = false;
            }
        // if need to toggle gui
        if (eventReceiver->toggleGUI)
        {
            gui->terminate();
            switch (eventReceiver->state)
            {
            case(MENU):
                gui->initialize(MENU);
                break;
            case(INGAME_MENU):
                gui->initialize(INGAME_MENU);
                break;
            case(HUD):
                gui->initialize(HUD);
                break;
            case(SETTINGS):
                gui->initialize(SETTINGS);
                break;
            case(CONTROL_SETTINGS):
                gui->initialize(CONTROL_SETTINGS);
                break;
            case(TERMINATED):
                break;
            }
            eventReceiver->toggleGUI = false;
        }
        // settings
        if (eventReceiver->state == SETTINGS) {
                // toggles fullscreen
            if (eventReceiver->toggleFullscreen)
            {
                configuration.fullscreen = !configuration.fullscreen;
                // sets default resolution (ignores if fullscreen is on)
                configuration.resolution = core::dimension2d<u32>(640, 480);
                // toggles off resizable mode
                configuration.resizable = false;
                // restart window
                gui->terminate();
                terminateDevice();
                if (!initializeDevice())
                    return;
                initializeGUI();
                initialized = true;
                eventReceiver->state = SETTINGS;
                gui->initialize(SETTINGS);
                eventReceiver->needRestartInMenu = false;
            }
             // toggles resolution
            if (eventReceiver->toggleResolution)
            {
                switch (gui->comboBoxResolution->getSelected())
                {
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
                // restart window
                gui->terminate();
                terminateDevice();
                if (!initializeDevice())
                    return;
                initializeGUI();
                initialized = true;
                eventReceiver->state = SETTINGS;
                gui->initialize(SETTINGS);
                eventReceiver->needRestartInMenu = false;
            }
            // toggle render distance, vsync and stencil buffer
            if(eventReceiver->toggleGraphicMode)
            {
                configuration.renderDistance = gui->spinBoxRenderDistance->getValue();
                configuration.vsync = gui->checkBoxVSync->isChecked();
                configuration.stencilBuffer = gui->checkBoxStencilBuffer->isChecked();
                eventReceiver->toggleGraphicMode = false;
                eventReceiver->needRestartInMenu = true;
            }
            // toggles language
            if (eventReceiver->toggleLanguage)
            {
                switch (gui->comboBoxLanguage->getSelected())
                {
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
                eventReceiver->toggleLanguage = false;
                setLanguage(configuration.language, true);
                gui->terminate();
                gui->initialize(SETTINGS);
            }
        }
        // control settings
        if (eventReceiver->state == CONTROL_SETTINGS) {
                // if "default" button is pressed
                if (eventReceiver->defaultControls)
                {
                    configuration.controls = Controls();
                    eventReceiver->changingControlUp = false;
                    eventReceiver->changingControlLeft = false;
                    eventReceiver->changingControlDown = false;
                    eventReceiver->changingControlRight = false;
                    eventReceiver->defaultControls = false;
                    eventReceiver->lastKey = KEY_KEY_CODES_COUNT;
                    gui->terminate();
                    gui->initialize(CONTROL_SETTINGS);
                }
                // if control key was chosen to replace
                if((eventReceiver->changingControlUp || eventReceiver->changingControlLeft ||
                    eventReceiver->changingControlDown || eventReceiver->changingControlRight ||
                    eventReceiver->changingControlCwRoll || eventReceiver->changingControlCcwRoll))
                {
                    //if something has been pressed after choice of key that user want to replace
                   if (eventReceiver->lastKey != KEY_KEY_CODES_COUNT)
                   {
                       // and if it's not escape or another inappropriate key
                       if ((eventReceiver->lastKey != KEY_ESCAPE) &&
                           (keyCodeName(eventReceiver->lastKey) != ""))
                        {
                            //if key is already occupied somewhere
                            if (eventReceiver->lastKey == configuration.controls.up)
                                configuration.controls.up = KEY_KEY_CODES_COUNT;
                            else if (eventReceiver->lastKey == configuration.controls.left)
                                configuration.controls.left = KEY_KEY_CODES_COUNT;
                            else if (eventReceiver->lastKey == configuration.controls.down)
                                configuration.controls.down = KEY_KEY_CODES_COUNT;
                            else if (eventReceiver->lastKey == configuration.controls.right)
                                configuration.controls.right = KEY_KEY_CODES_COUNT;
                            else if (eventReceiver->lastKey == configuration.controls.cwRoll)
                                configuration.controls.cwRoll = KEY_KEY_CODES_COUNT;
                            else if (eventReceiver->lastKey == configuration.controls.ccwRoll)
                                configuration.controls.ccwRoll = KEY_KEY_CODES_COUNT;
                            //if not
                            if (eventReceiver->changingControlUp)
                                configuration.controls.up = eventReceiver->lastKey;
                            else if (eventReceiver->changingControlLeft)
                                configuration.controls.left = eventReceiver->lastKey;
                            else if (eventReceiver->changingControlDown)
                                configuration.controls.down = eventReceiver->lastKey;
                            else if (eventReceiver->changingControlRight)
                                configuration.controls.right = eventReceiver->lastKey;
                            else if (eventReceiver->changingControlCwRoll)
                                configuration.controls.cwRoll = eventReceiver->lastKey;
                            else if (eventReceiver->changingControlCcwRoll)
                                configuration.controls.ccwRoll = eventReceiver->lastKey;
                        }
                        eventReceiver->changingControlUp = false;
                        eventReceiver->changingControlLeft = false;
                        eventReceiver->changingControlDown = false;
                        eventReceiver->changingControlRight = false;
                        eventReceiver->changingControlCwRoll = false;
                        eventReceiver->changingControlCcwRoll = false;
                        eventReceiver->lastKey = KEY_KEY_CODES_COUNT;
                        gui->terminate();
                        gui->initialize(CONTROL_SETTINGS);
                   }
                   else if (eventReceiver->changingControlUp)
                        gui->buttonControlUp->setText(_wp("Press a key"));
                    else if (eventReceiver->changingControlLeft)
                        gui->buttonControlLeft->setText(_wp("Press a key"));
                    else if (eventReceiver->changingControlDown)
                        gui->buttonControlDown->setText(_wp("Press a key"));
                    else if (eventReceiver->changingControlRight)
                        gui->buttonControlRight->setText(_wp("Press a key"));
                    else if (eventReceiver->changingControlCwRoll)
                        gui->buttonControlCwRoll->setText(_wp("Press a key"));
                    else if (eventReceiver->changingControlCcwRoll)
                        gui->buttonControlCcwRoll->setText(_wp("Press a key"));
                }
        }
        // catch a resize of window
        if (configuration.resolution != driver->getScreenSize())
        {
            configuration.resolution = driver->getScreenSize();
            gui->resizeGUI();
        }
        // screen size
        core::stringw scrs = _w("Screen size: ");
        scrs += configuration.resolution.Width;
        scrs += "x";
        scrs += configuration.resolution.Height;
        gui->textScreenSize->setText(scrs.c_str());
        if ((guiEnvironment->getFocus() != nullptr) && (eventReceiver->tabPressed))
        {
            gui->selectWithTab();
            eventReceiver->tabPressed = false;
        }
        if (eventReceiver->downPressed)
        {
            if (guiEnvironment->getFocus() == nullptr)
                gui->selectElement(0);
            else
            {
                gui->selectNextElement();
            }
            eventReceiver->downPressed = false;
        }
        if (eventReceiver->upPressed)
        {
            if (guiEnvironment->getFocus() == nullptr)
                gui->selectElement(0);
            else
            {
                gui->selectPreviousElement();
            }
            eventReceiver->upPressed = false;
        }
        if (eventReceiver->IsKeyDown(KEY_RIGHT))
        {
            if ((!eventReceiver->rightPressed) && (guiEnvironment->getFocus()->getType() == gui::EGUIET_BUTTON) && (guiEnvironment->getFocus() != nullptr))
            {
                    SEvent event;
                    event.EventType = EET_GUI_EVENT;
                    event.GUIEvent.Caller = guiEnvironment->getFocus();
                    event.GUIEvent.Element = guiEnvironment->getFocus();
                    event.GUIEvent.EventType = gui::EGET_BUTTON_CLICKED;
                    device->postEventFromUser(event);
                    eventReceiver->rightPressed = true;
            }
        } else
                eventReceiver->rightPressed = false;
        if (eventReceiver->leftPressed)
        {
            if (eventReceiver->state == SETTINGS)
            {
                eventReceiver->state = MENU;
                eventReceiver->toggleGUI = true;
            }
            else if (eventReceiver->state == CONTROL_SETTINGS)
            {
                eventReceiver->state = SETTINGS;
                eventReceiver->toggleGUI = true;
            }
            eventReceiver->leftPressed = false;
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
    gui->terminate();
}

// start the game itself
void Game::run()
{
    // initialize HUD
    gui->initialize(HUD);
    //initialize scene
    initializeScene();
    // create structure for light data
    video::SLight lightData;
    // create variable for color
    video::SColor color;
    while (device->run())
    {
        // if we exit to menu or quit from game -> stop
        if (eventReceiver->state == MENU || eventReceiver->quit) {
            break;
        }
        // set color
        color = iridescentColor(timer->getTime());
        if (pause) {
            // catch a resize of window
            if (configuration.resolution != driver->getScreenSize())
            {
                configuration.resolution = driver->getScreenSize();
                gui->resizeGUI();
            }
            // screen size
            core::stringw scrs = _w("Screen size: ");
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            gui->textScreenSize->setText(scrs.c_str());
            // set cursor visible
            device->getCursorControl()->setVisible(true);

            // if need to toggle gui
            if (eventReceiver->toggleGUI) {
                pause = !pause;
                gui->terminate();
                gui->initialize(HUD);
                eventReceiver->toggleGUI = false;
            }
            if ((guiEnvironment->getFocus() != nullptr) && (eventReceiver->tabPressed))
            {
                gui->selectWithTab();
                eventReceiver->tabPressed = false;
            }
            if (eventReceiver->downPressed)
            {
                if (guiEnvironment->getFocus() == nullptr)
                    gui->selectElement(0);
                else
                    gui->selectNextElement();
                eventReceiver->downPressed = false;
            }
            if (eventReceiver->upPressed)
            {
                if (guiEnvironment->getFocus() == nullptr)
                    gui->selectElement(0);
                else
                    gui->selectPreviousElement();
                eventReceiver->upPressed = false;
            }
            if (eventReceiver->IsKeyDown(KEY_RIGHT))
            {
                if ((!eventReceiver->rightPressed) && (guiEnvironment->getFocus() != nullptr))
                {
                    SEvent event;
                    event.EventType = EET_GUI_EVENT;
                    event.GUIEvent.Caller = guiEnvironment->getFocus();
                    event.GUIEvent.Element = guiEnvironment->getFocus();
                    event.GUIEvent.EventType = gui::EGET_BUTTON_CLICKED;
                    device->postEventFromUser(event);
                    eventReceiver->rightPressed = true;
                }
            } else
                eventReceiver->rightPressed = false;
            if (eventReceiver->leftPressed)
            {
                eventReceiver->state = MENU;
                eventReceiver->toggleGUI = true;
                eventReceiver->leftPressed = false;
                break;
            }
        } else {
            // setting fog color
            if (FOG_ENABLED && IRIDESCENT_FOG)
                driver->setFog(color, video::EFT_FOG_LINEAR, configuration.renderDistance - 300, configuration.renderDistance, 0.01f, true, true);

            // setting light color
            if (IRIDESCENT_LIGHT)
            {
                lightData = light->getLightData();
                lightData.DiffuseColor = color;
                lightData.AmbientColor = color;
                light->setLightData(lightData);
            }

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
                gui->textCameraPos->setText(cameraPosition.c_str());
            }

            // cube counter
            {
                core::stringw cubeCount = _w("Cubes: ");
                cubeCount += obstacleGenerator->getCubeCount();
                gui->textCubeCount->setText(cubeCount.c_str());
            }

            // fps counter
            {
                core::stringw fps = _w("FPS: ");
                fps += driver->getFPS();
                gui->textFPS->setText(fps.c_str());
            }

            // velocity counter
            {
                core::stringw velocity = _w("Linear velocity: ");
                velocity += (int) plane->getRigidBody().getLinearVelocity().length();
                velocity += _w(", angular velocity: ");
                velocity += plane->getRigidBody().getAngularVelocity().length();
                gui->textVelocity->setText(velocity.c_str());
            }

            // set camera position, target, and rotation
            {
                core::vector3df upVector(0, 1, 0);
                upVector.rotateXYBy(plane->getEulerRotation().z() * core::RADTODEG64);

                camera->setPosition(plane->getNode().getPosition() + upVector * 0.3f * CAMERA_DISTANCE
                    - core::vector3df(0, 0, CAMERA_DISTANCE));
                camera->setUpVector(upVector);

                camera->setTarget(camera->getPosition() + core::vector3df(0, 0, 1));
            }

            //set points counter
            {
                 core::stringw points = _w("Points: ");
                 points += (int) (plane ->getNode().getPosition().Z *0.01);
                 gui->textPoints->setText(points.c_str());
            }

            //set cursor invisible
            device->getCursorControl()->setVisible(false);

            // generate level
            obstacleGenerator->generate(plane->getNode().getPosition());

            // if toggling gui is needed
            if (eventReceiver->toggleGUI) {
                pause = !pause;
                gui->terminate();
                gui->initialize(INGAME_MENU);
                eventReceiver->toggleGUI = false;
            }
        }
        // if escape is pressed
        if (eventReceiver->IsKeyDown(KEY_ESCAPE)) {
            if (!eventReceiver->escapePressed) {
                eventReceiver->toggleGUI = true;
                eventReceiver->escapePressed = true;
            }
        } else if (!eventReceiver->IsKeyDown(KEY_ESCAPE)) {
            eventReceiver->escapePressed = false;
        }
        if (device->isWindowActive()) {
            if (IRIDESCENT_BACKGROUND)
                driver->beginScene(true, true, color);
            else
                driver->beginScene(true, true, DEFAULT_COLOR);
            if (!pause) {
                // handle plane controls
                planeControl->handle(eventReceiver);
                // physics simulation
                dynamicsWorld->stepSimulation(1 / 60.0f);
                dynamicsWorld->debugDrawWorld();
                // draw scene
                sceneManager->drawAll();
                #ifdef DEBUG
                std::cout << "=== STEP_SIMULATION ===" << std::endl;
                #endif
            }
            guiEnvironment->drawAll();
            driver->endScene();
        } else {
            if (!pause) {
                pause = true;
                gui->terminate();
                gui->initialize(INGAME_MENU);
            }
            device->yield();
        }
    }
    pause = false;

    terminateScene();
    gui->terminate();
}
