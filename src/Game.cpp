#include "Game.h"

using namespace irr;

#define FAR_VALUE 2000

Game::Game(const struct ConfigData &data)
{
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
    gui::IGUIFont *font = gui::CGUITTFont::createTTFont(driver, fileSystem, io::path("media/fonts/font.ttf"), 13, true, true);
    if (font)
        skin->setFont(font);

    gui = new GUI(configuration, guiEnvironment);
}

bool Game::initializeDevice()
{
    if (configuration.fullscreen)
    {
        IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);
        configuration.resolution = nulldevice->getVideoModeList()->getDesktopResolution();
        nulldevice -> drop();
    }
    device = createDevice(video::EDT_OPENGL, configuration.resolution, configuration.colordepth, configuration.fullscreen, configuration.stencilBuffer, configuration.vsync);
    if (!device) {
        error("Couldn't create a device :(\n");
        return false;
    }
    device->setWindowCaption(L"PlaneTest");
    timer=device->getTimer();
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

void Game::initializeScene()
{

    driver->setFog(iridescentColor(timer->getTime()), video::EFT_FOG_LINEAR, 1300, 1600, .003f, true, false);

    // Bullet
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

    light = sceneManager->addLightSceneNode(0, core::vector3df(0, 0, 1), video::SColor(0, getRandomf(0, 255), getRandomf(0, 255), getRandomf(0, 255)), 300);
    light->setLightType(video::ELT_DIRECTIONAL);

    plane = new Plane(dynamicsWorld, device, btVector3(0, 0, 0));
    plane->getRigidBody()->applyForce(btVector3(0, 0, 100000), btVector3(0, 0, 0));

    camera = device->getSceneManager()->addCameraSceneNode(0);// (plane->getNode());
    camera->setPosition(core::vector3df(0, 0, -SPHERE_RADIUS - CAMERA_DISTANCE));
    camera->setFarValue(FAR_VALUE);

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
    sceneManager->clear();
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

void Game::menu()
{
    if (!initialized) {
        error(ERR_NOT_INITIALIZED);
        return;
    }
    configuration.resolution = driver->getScreenSize();
    gui->initialize(MENU);
    while (device->run()) {
        if (eventReceiver->stage == INGAME_MENU){
            gui->terminate();
            run();
        }
        if (eventReceiver->quit) {
            break;
        }
        if (eventReceiver->IsKeyDown(KEY_ESCAPE)) {
            if (!eventReceiver->escapePressed)
            {
                eventReceiver->escapePressed = true;
                if (eventReceiver->stage == MENU)
                    return;
                else if (eventReceiver->stage == SETTINGS)
                    {
                        eventReceiver->stage = MENU;
                        eventReceiver->toggleGUI = true;
                    }
                else if (eventReceiver->stage == CONTROL_SETTINGS)
                    {
                        eventReceiver->stage = SETTINGS;
                        eventReceiver->toggleGUI = true;
                    }
            }
        }
        else if (!eventReceiver->IsKeyDown(KEY_ESCAPE))
            eventReceiver->escapePressed = false;
        if (gui->getStage() == SETTINGS && eventReceiver->stage == MENU && eventReceiver->needRestartInMenu)
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
        if (eventReceiver->toggleGUI)
        {
            gui->terminate();
            switch (eventReceiver->stage)
            {
            case(MENU):
                gui->initialize(MENU);
                break;
            case(SETTINGS):
                gui->initialize(SETTINGS);
                break;
            case(CONTROL_SETTINGS):
                gui->initialize(CONTROL_SETTINGS);
                break;
            case(INGAME_MENU):
                gui->initialize(INGAME_MENU);
                break;
            case(TERMINATED):
                break;
            }
            eventReceiver->toggleGUI = false;
        }
        if (eventReceiver->stage == SETTINGS){
            if (eventReceiver->toggleFullscreen)
            {
                configuration.fullscreen = !configuration.fullscreen;
                configuration.resolution = core::dimension2d<u32>(640, 480);
                configuration.resizable = false;
                gui->terminate();
                terminateDevice();
                if (!initializeDevice())
                    return;
                initializeGUI();
                initialized = true;
                eventReceiver->stage = SETTINGS;
                gui->initialize(SETTINGS);
                eventReceiver->needRestartInMenu = false;
            }
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
                gui->terminate();
                terminateDevice();
                if (!initializeDevice())
                    return;
                initializeGUI();
                initialized = true;
                eventReceiver->stage = SETTINGS;
                gui->initialize(SETTINGS);
                eventReceiver->needRestartInMenu = false;
            }
            if(eventReceiver->toggleColorDepth)
            {
                switch (gui->comboBoxColorDepth->getSelected())
                {
                case 0:
                    configuration.colordepth = 8;
                    break;
                case 1:
                    configuration.colordepth = 16;
                    break;
                case 2:
                    configuration.colordepth = 32;
                    break;
                }
                configuration.vsync = gui->checkBoxVSync->isChecked();
                configuration.stencilBuffer = gui->checkBoxStencilBuffer->isChecked();
                eventReceiver->toggleColorDepth = false;
                eventReceiver->needRestartInMenu = true;
            }
            if (eventReceiver->toggleLanguage)
            {
                switch (gui->comboBoxLanguage->getSelected())
                {
                case 0:
                    configuration.language = "en";
                    break;
                case 1:
                    configuration.language = "ru";
                    break;
                case 2:
                    configuration.language = "crh";
                    break;
                case 3:
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
        if (eventReceiver->stage == CONTROL_SETTINGS){
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
                if((eventReceiver->changingControlUp || eventReceiver->changingControlLeft ||
                    eventReceiver->changingControlDown || eventReceiver->changingControlRight))
                {
                    //if something pressed after choice of key that user want to replace
                   if (eventReceiver->lastKey != KEY_KEY_CODES_COUNT)
                   {
                       if ((eventReceiver->lastKey != KEY_ESCAPE) &&
                           keyCodeName(eventReceiver->lastKey) != "")
                        {
                            if (eventReceiver->lastKey == configuration.controls.up)
                                configuration.controls.up = KEY_KEY_CODES_COUNT;
                            else if (eventReceiver->lastKey == configuration.controls.left)
                                configuration.controls.left = KEY_KEY_CODES_COUNT;
                            else if (eventReceiver->lastKey == configuration.controls.down)
                                configuration.controls.down = KEY_KEY_CODES_COUNT;
                            else if (eventReceiver->lastKey == configuration.controls.right)
                                configuration.controls.right = KEY_KEY_CODES_COUNT;
                            if (eventReceiver->changingControlUp)
                                configuration.controls.up = eventReceiver->lastKey;
                            else if (eventReceiver->changingControlLeft)
                                configuration.controls.left = eventReceiver->lastKey;
                            else if (eventReceiver->changingControlDown)
                                configuration.controls.down = eventReceiver->lastKey;
                            else if (eventReceiver->changingControlRight)
                                configuration.controls.right = eventReceiver->lastKey;
                        }
                        eventReceiver->changingControlUp = false;
                        eventReceiver->changingControlLeft = false;
                        eventReceiver->changingControlDown = false;
                        eventReceiver->changingControlRight = false;
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
                }
        }
        if (configuration.resolution != driver->getScreenSize())
        {
            configuration.resolution = driver->getScreenSize();
            gui->resizeGUI();
        }
        core::stringw scrs = _w("Screen size: ");
        scrs += configuration.resolution.Width;
        scrs += "x";
        scrs += configuration.resolution.Height;
        gui->textScreenSize->setText(scrs.c_str());
        device->getCursorControl()->setVisible(true);
        if (device->isWindowActive()) {
            driver->beginScene(true, true, iridescentColor(timer->getTime()));
            guiEnvironment->drawAll();
            driver->endScene();
        } else {
            device->yield();
        }
    }

    gui->terminate();
}

void Game::run()
{
    gui->initialize(INGAME_MENU);
    initializeScene();
    configuration.resolution = driver->getScreenSize();
    video::SLight lightData;
    video::SColor color;
    while (device->run())
    {
        if (eventReceiver->stage == MENU || eventReceiver->quit){
            break;
        }
        color = iridescentColor(timer->getTime());
        driver->setFog(color, video::EFT_FOG_LINEAR, 800.0f, 1500.0f, 0.01f, true, true);
        lightData = light->getLightData();
        lightData.DiffuseColor = color;
        lightData.AmbientColor = color;
        light->setLightData(lightData);
        if (pause) {
            core::stringw scrs = _w("Screen size: ");
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            gui->textScreenSize->setText(scrs.c_str());

            if (configuration.resolution != driver->getScreenSize())
            {
                configuration.resolution = driver->getScreenSize();
                gui->resizeGUI();
            }

            gui->setVisible(true);
            gui->textCameraPos->setVisible(false);
            gui->textCubeCount->setVisible(false);
            gui->textFPS->setVisible(false);
            device->getCursorControl()->setVisible(true);
        } else {
            {
                core::stringw cameraPosition = _w("Plane position: (");
                core::vector3df position = plane->getNode()->getPosition();
                cameraPosition += position.X;
                cameraPosition += ", ";
                cameraPosition += position.Y;
                cameraPosition += ", ";
                cameraPosition += position.Z;
                cameraPosition += ")";
                gui->textCameraPos->setText(cameraPosition.c_str());
            }

            {
                core::stringw cubeCount = _w("Cubes: ");
                cubeCount += obstacleGenerator->getCubeCount();
                gui->textCubeCount->setText(cubeCount.c_str());
            }

            {
                core::stringw fps = _w("FPS: ");
                fps += driver->getFPS();
                gui->textFPS->setText(fps.c_str());
            }

            camera->setPosition(plane->getNode()->getPosition() - core::vector3df(0, 0, SPHERE_RADIUS + CAMERA_DISTANCE));
            camera->setTarget(plane->getNode()->getPosition());
            gui->setVisible(false);
            gui->textCameraPos->setVisible(true);
            gui->textCubeCount->setVisible(true);
            gui->textFPS->setVisible(true);
            device->getCursorControl()->setVisible(false);

            obstacleGenerator->generate(plane->getNode()->getPosition());
        }

        if (eventReceiver->IsKeyDown(KEY_ESCAPE)) {
            if (!eventReceiver->escapePressed) {
                pause = !pause;
                eventReceiver->escapePressed = true;
            }
        } else if (!eventReceiver->IsKeyDown(KEY_ESCAPE)) {
            eventReceiver->escapePressed = false;
        }
        if (eventReceiver->resume)
        {
            eventReceiver->resume = false;
            pause = false;
        }

        if (device->isWindowActive()) {
            dynamicsWorld->stepSimulation(1 / 60.f);
            driver->beginScene(true, true, color);
            if (!pause)
                sceneManager->drawAll();
            guiEnvironment->drawAll();
            driver->endScene();
        } else {
            pause = true;
            device->yield();
        }
    }
    pause = false;

    terminateScene();
    gui->terminate();
}
