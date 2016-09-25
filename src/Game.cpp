#include "Game.h"

using namespace irr;

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
    terminate();
}

void Game::initializeGUI()
{
    gui::IGUISkin *skin = guiEnvironment->getSkin();
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
    device = createDevice(video::EDT_OPENGL, configuration.resolution, configuration.colordepth, configuration.fullscreen, true);
    if (!device) {
        error("Couldn't create a device :(\n");
        return false;
    }
    device->setWindowCaption(L"PlaneTest");
    driver = device->getVideoDriver();
    sceneManager = device->getSceneManager();
    guiEnvironment = device->getGUIEnvironment();
    fileSystem = device->getFileSystem();
    eventReceiver = new EventReceiver();
    device->setEventReceiver(eventReceiver);
    device->setResizable(configuration.resizable);

    return true;
}

void Game::initializeScene()
{
    camera = sceneManager->addCameraSceneNode();
    scene::ISceneNodeAnimator *cameraAnimator = new SceneNodeAnimatorCameraPlayer(35.f, 15.f, 15.f);
    camera->setFarValue(1500);
    camera->addAnimator(cameraAnimator);
    cameraAnimator->drop();

    light = sceneManager->addLightSceneNode(camera, core::vector3df(0, 0, -100), video::SColorf(1, 1, 1), 300);
    light->setVisible(true);

    obstacleGenerator = new ObstacleGenerator(device, camera->getFarValue(), 500);
}

void Game::error(const core::stringw &str) const
{
    std::wcerr << "Error: " << str.c_str() << std::endl;
}

void Game::terminate()
{
    Config conf;
    conf.saveConfig("game.conf", configuration);
    gui->terminateGUI();
    device->closeDevice();
    device->run();
    device->drop();
}

void Game::menu()
{
    if (!initialized) {
        error(ERR_NOT_INITIALIZED);
        return;
    }
    configuration.resolution = driver->getScreenSize();
    gui->initializeMenuGUI();

    while (device->run()) {
        if (eventReceiver->start){
            gui->terminateGUI();
            run();
            gui->initializeMenuGUI();
        }
        if (eventReceiver->quit || eventReceiver->IsKeyDown(KEY_ESCAPE)){
            break;
        }
        if (eventReceiver->settings){
            if (gui->buttonStart != nullptr)
            {
                gui->terminateGUI();
                gui->buttonStart = nullptr;
                gui->initializeSettingsGUI();
            }
            if (eventReceiver->toggleFullscreen)
            {
                configuration.fullscreen = !configuration.fullscreen;
                terminate();
                configuration.resolution = core::dimension2d<u32>(1240, 720);
                configuration.resizable = false;
                if (!initializeDevice())
                        return;
                initialized = true;
                initializeGUI();
                gui->initializeMenuGUI();
            }
            if(eventReceiver->toggleGraphicMode)
            {
                switch (gui->resolutionComboBox->getSelected())
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
                switch (gui->colorDepthComboBox->getSelected())
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
                terminate();
                if (!initializeDevice())
                    return;
                initialized = true;
                initializeGUI();
                gui->initializeMenuGUI();
            }
            if (eventReceiver->toggleLanguage)
            {
                switch (gui->languageComboBox->getSelected())
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
                gui->terminateGUI();
                eventReceiver->settings = false;
                gui->initializeMenuGUI();
            }
        }
        else if (gui->buttonStart == nullptr)
            {
                gui->terminateGUI();
                gui->initializeMenuGUI();
            }
        core::stringw scrs = _w("Screen size: ");
        scrs += configuration.resolution.Width;
        scrs += "x";
        scrs += configuration.resolution.Height;
        gui->screenSizeText->setText(scrs.c_str());
        if(configuration.resolution != driver->getScreenSize())
        {
            configuration.resolution = driver->getScreenSize();
            gui->resizeGUI();
        }
        device->getCursorControl()->setVisible(true);
        if (device->isWindowActive()) {
            driver->beginScene(true, true, video::SColor(0, 135, 206, 235));;
            guiEnvironment->drawAll();
            driver->endScene();
        } else {
            device->yield();
        }
    }
}

void Game::run()
{
    gui->initializeInGameGUI();
    initializeScene();
    configuration.resolution = driver->getScreenSize();

    while (device->run())
    {
        if (eventReceiver->quit)
            break;
        if (!eventReceiver->start){
            gui->terminateGUI();
            break;
        }
        if (pause) {
            core::stringw scrs = _w("Screen size: ");
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            gui->screenSizeText->setText(scrs.c_str());

            if (configuration.resolution != driver->getScreenSize())
            {
                configuration.resolution = driver->getScreenSize();
                gui->resizeGUI();
            }

            camera->setInputReceiverEnabled(false);

            gui->setVisible(true);
            gui->cameraPosText->setVisible(false);
            gui->cubeCountText->setVisible(false);
            device->getCursorControl()->setVisible(true);
        } else {
            {
                core::stringw cameraPosition = _w("Camera position: (");
                core::vector3df position = camera->getPosition();
                cameraPosition += position.X;
                cameraPosition += ", ";
                cameraPosition += position.Y;
                cameraPosition += ", ";
                cameraPosition += position.Z;
                cameraPosition += ")";
                gui->cameraPosText->setText(cameraPosition.c_str());
            }

            {
                core::stringw cubeCount = _w("Cubes: ");
                cubeCount += obstacleGenerator->getCubeCount();
                gui->cubeCountText->setText(cubeCount.c_str());
            }

            camera->setInputReceiverEnabled(true);

            gui->setVisible(false);
            gui->cameraPosText->setVisible(true);
            gui->cubeCountText->setVisible(true);
            device->getCursorControl()->setVisible(false);

            obstacleGenerator->generate(camera->getPosition());
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
            driver->beginScene(true, true, video::SColor(0, 135, 206, 235));
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
    sceneManager->clear();
}
