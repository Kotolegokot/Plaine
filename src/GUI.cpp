#include "GUI.h"

using namespace irr;

GUI::GUI (struct ConfigData &data, gui::IGUIEnvironment *guiEnv) : configuration(data), guiEnvironment(guiEnv) {}

GUIState GUI::getStage()
{
    return stage;
}

void GUI::recalculateButtonProportions()
{
    //width for buttons calculates from width of window
    buttonWidth = configuration.resolution.Width / 6;
    //minimal width for buttons
    if (buttonWidth < 220)
        buttonWidth = 220;
    //height calculated from width
    buttonHeight = buttonWidth * 1/8;
}

void GUI::initialize(GUIState stage)
{
    //choose specific method for initializing GUI
    switch (stage)
    {
    case(MENU):
        initializeMenuGUI();
        break;
    case(INGAME_MENU):
        initializeInGameGUI();
        break;
    case(HUD):
        initializeHUD();
        break;
    case(SETTINGS):
        initializeSettingsGUI();
        break;
     case(CONTROL_SETTINGS):
        initializeControlSettingsGUI();
        break;
    case(TERMINATED):
        break;
    }
    this->stage = stage;
}

void GUI::initializeMenuGUI()
{
    recalculateButtonProportions();
    textScreenSize = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonStart = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE),
        0, ID_BUTTON_START, _wp("Start"), _wp("Start game"));
    buttonSettings = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE),
        0, ID_BUTTON_SETTINGS, _wp("Settings"), _wp("Game settings"));
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE),
        0, ID_BUTTON_QUIT, _wp("Quit"), _wp("Exit game"));
}

void GUI::terminateMenuGUI()
{
    buttonStart->remove();
    buttonSettings->remove();
    buttonQuit->remove();
    textScreenSize->remove();
}

void GUI::initializeInGameGUI()
{
    textScreenSize = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonResume = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE),
        0, ID_BUTTON_RESUME, _wp("Resume"), _wp("Resume Game"));
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE),
        0, ID_BUTTON_MENU, _wp("Menu"), _wp("Exit to Main menu"));
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE),
        0, ID_BUTTON_QUIT, _wp("Quit"), _wp("Exit game"));
}

void GUI::terminateInGameGUI()
{
    textScreenSize->remove();
    buttonResume->remove();
    buttonMenu->remove();
    buttonQuit->remove();
}

void GUI::initializeHUD()
{
    recalculateButtonProportions();
    textCameraPos = guiEnvironment->addStaticText(L"CAMERA_POS", core::rect<s32>(10, 10, 400, 30), false);
    textCameraPos->setBackgroundColor(video::SColor(120, 255, 255, 255));
    textCubeCount = guiEnvironment->addStaticText(L"CUBE_COUNT", core::rect<s32>(10, 10 + 24, 400, 30 + 24), false);
    textCubeCount->setBackgroundColor(video::SColor(120, 255, 255, 255));
    textFPS = guiEnvironment->addStaticText(L"FPS", core::rect<s32>(10, 10 + 24 + 24, 400, 30 + 24 + 24), false);
    textFPS->setBackgroundColor(video::SColor(120, 255, 255, 255));
    textVelocity = guiEnvironment->addStaticText(L"Velocity", core::rect<s32>(10, 10 + 24 + 24 + 24, 400, 30 + 24 + 24 + 24), false);
    textVelocity->setBackgroundColor(video::SColor(120, 255, 255, 255));
}

void GUI::terminateHUD()
{
    textCameraPos->remove();
    textCubeCount->remove();
    textFPS->remove();
    textVelocity->remove();
}

void GUI::initializeSettingsGUI()
{
    recalculateButtonProportions();
    textScreenSize = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    textLanguage = guiEnvironment->addStaticText(_wp("Language:"), core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 9*buttonHeight - 9*SPACE, configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 9*SPACE), false);
    comboBoxLanguage = guiEnvironment->addComboBox(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 9*buttonHeight - 9*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 9*SPACE), 0, ID_COMBOBOX_LANGUAGE);
    comboBoxLanguage->addItem(L"English", 0);
    comboBoxLanguage->addItem(L"Русский", 1);
    comboBoxLanguage->addItem(L"Qırımtatarca", 2);
    comboBoxLanguage->addItem(L"Къырымтатарджа", 3);
    if (configuration.language == "en")
        comboBoxLanguage->setSelected(0);
    else if (configuration.language == "ru")
        comboBoxLanguage->setSelected(1);
    else if (configuration.language == "crh")
        comboBoxLanguage->setSelected(2);
    else if (configuration.language == "crh@cyrillic")
        comboBoxLanguage->setSelected(3);
    textResolution = guiEnvironment->addStaticText(_wp("Resolution:"), core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 8*SPACE, configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 8*SPACE), false);
    comboBoxResolution = guiEnvironment->addComboBox(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 8*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 8*SPACE), 0, ID_COMBOBOX_RESOLUTION);
    if (configuration.fullscreen)
    {
        core::stringw scrs;
        scrs += configuration.resolution.Width;
        scrs += "x";
        scrs += configuration.resolution.Height;
        scrs += _w(" (Fullscreen)");
        comboBoxResolution->addItem(scrs.c_str(), 0);
        comboBoxResolution->setSelected(0);
        comboBoxResolution->setEnabled(false);

    }
    else {
        comboBoxResolution->addItem(L"640x480", 0);
        comboBoxResolution->addItem(L"1240x720", 1);
        if (configuration.resizable)
        {
            core::stringw scrs;
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            scrs += _w(" (Custom)");
            comboBoxResolution->addItem(scrs.c_str(), 2);
            comboBoxResolution->setSelected(2);
        }
        else if (configuration.resolution == core::dimension2d<u32>(640, 480))
        {
            comboBoxResolution->setSelected(0);
            comboBoxResolution->addItem(_wp("Custom Resolution"), 2);
        }
        else if (configuration.resolution == core::dimension2d<u32>(1240, 720))
        {
            comboBoxResolution->setSelected(1);
            comboBoxResolution->addItem(_wp("Custom Resolution"), 2);
        }
        else
        {
            core::stringw scrs;
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            scrs += _w(" (Custom)");
            comboBoxResolution->addItem(scrs.c_str(), 2);
            comboBoxResolution->setSelected(2);
        }
    }
    textColorDepth = guiEnvironment->addStaticText(_wp("Color Depth:"), core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 7*SPACE, configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 7*SPACE), false);
    comboBoxColorDepth = guiEnvironment->addComboBox(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 7*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 7*SPACE), 0, ID_COMBOBOX_COLORDEPTH);
    comboBoxColorDepth->addItem(L"8", 0);
    comboBoxColorDepth->addItem(L"16", 1);
    comboBoxColorDepth->addItem(L"32", 2);
    if (configuration.colordepth == 8)
        comboBoxColorDepth->setSelected(0);
    else if (configuration.colordepth == 16)
        comboBoxColorDepth->setSelected(1);
    else if (configuration.colordepth == 32)
        comboBoxColorDepth->setSelected(2);
    checkBoxVSync = guiEnvironment->addCheckBox(configuration.vsync, core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 6*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 6*SPACE),
        0, ID_CHECKBOX_VSYNC, _wp("VSync"));
    checkBoxStencilBuffer = guiEnvironment->addCheckBox(configuration.stencilBuffer, core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 5*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 5*SPACE),
        0, ID_CHECKBOX_STENCILBUFFER, _wp("Stencil Buffer"));

    if (configuration.fullscreen)
        buttonToggleFullscreen = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE),
            0, ID_BUTTON_TOGGLE_FULLSCREEN, _wp("Windowed"), _wp("To Windowed Mode"));
    else
        buttonToggleFullscreen = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE),
            0, ID_BUTTON_TOGGLE_FULLSCREEN, _wp("Fullscreen"), _wp("To Fullscreen Mode"));
    buttonControlSettings = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE),
        0, ID_BUTTON_CONTROL_SETTINGS, _wp("Controls"), _wp("Edit control settings"));
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE),
        0, ID_BUTTON_MENU, _wp("Back"), _wp("Exit to Main menu"));
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE),
        0, ID_BUTTON_QUIT, _wp("Quit"), _wp("Exit game"));
}

void GUI::terminateSettingsGUI()
{
    textScreenSize->remove();
    textLanguage->remove();
    comboBoxLanguage->remove();
    textResolution->remove();
    comboBoxResolution->remove();
    textColorDepth->remove();
    comboBoxColorDepth->remove();
    checkBoxVSync->remove();
    checkBoxStencilBuffer->remove();
    buttonToggleFullscreen->remove();
    buttonControlSettings->remove();
    buttonMenu->remove();
    buttonQuit->remove();
}

void GUI::initializeControlSettingsGUI()
{
    textScreenSize = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    textControlUp = guiEnvironment->addStaticText(_wp("Up: "), core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 7*SPACE, configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 7*SPACE), false);
    buttonControlUp = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 7*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 7*SPACE),
        0, ID_BUTTON_CONTROL_UP, keyCodeName(configuration.controls.up).c_str(), _wp("Click to change"));
    textControlLeft = guiEnvironment->addStaticText(_wp("Left: "), core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 6*SPACE, configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 6*SPACE), false);
    buttonControlLeft = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 6*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 6*SPACE),
        0, ID_BUTTON_CONTROL_LEFT, keyCodeName(configuration.controls.left).c_str(), _wp("Click to change"));
    textControlDown = guiEnvironment->addStaticText(_wp("Down: "), core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 5*SPACE, configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 5*SPACE), false);
    buttonControlDown = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 5*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 5*SPACE),
        0, ID_BUTTON_CONTROL_DOWN, keyCodeName(configuration.controls.down).c_str(), _wp("Click to change"));
    textControlRight = guiEnvironment->addStaticText(_wp("Right: "), core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE), false);
    buttonControlRight = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE),
        0, ID_BUTTON_CONTROL_RIGHT, keyCodeName(configuration.controls.right).c_str(), _wp("Click to change"));
    buttonDefultControls = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE),
        0, ID_BUTTON_DEFUALT_CONTROLS, _wp("Default"), _wp("Default control settings"));
    buttonSettings = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE),
        0, ID_BUTTON_SETTINGS, _wp("Back"), _wp("Back to game settings"));
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE),
        0, ID_BUTTON_QUIT, _wp("Quit"), _wp("Exit game"));
    stage = CONTROL_SETTINGS;
}

void GUI::terminateControlSettingsGUI()
{
    textControlUp->remove();
    buttonControlUp->remove();
    textControlLeft->remove();
    buttonControlLeft->remove();
    textControlDown->remove();
    buttonControlDown->remove();
    textControlRight->remove();
    buttonControlRight->remove();
    buttonDefultControls->remove();
    buttonSettings->remove();
    buttonQuit->remove();
    textScreenSize->remove();
}

void GUI::terminate()
{
    //choose specific method for terminating GUI
    switch (stage)
    {
    case(MENU):
            terminateMenuGUI();
            break;
    case(INGAME_MENU):
            terminateInGameGUI();
            break;
        case(HUD):
            terminateHUD();
            break;
    case(SETTINGS):
            terminateSettingsGUI();
            break;
     case(CONTROL_SETTINGS):
            terminateControlSettingsGUI();
            break;
    case(TERMINATED):
        break;
    }
    this->stage = TERMINATED;
}

void GUI::setVisible(bool state)
{
    switch (stage)
    {
    case (MENU):
        {
            textScreenSize->setVisible(state);
            buttonStart->setVisible(state);
            buttonSettings->setVisible(state);
            buttonQuit->setVisible(state);
            break;
        }
    case (INGAME_MENU):
        {
            buttonResume->setVisible(state);
            buttonMenu->setVisible(state);
            buttonQuit->setVisible(state);
            textScreenSize->setVisible(state);
            break;
        }
    case (HUD):
        {
            textCameraPos->setVisible(state);
            textCubeCount->setVisible(state);
            textFPS->setVisible(state);
            textVelocity->setVisible(state);
            break;
        }
    case (SETTINGS):
        {
            textScreenSize->setVisible(state);
            textLanguage->setVisible(state);
            comboBoxLanguage->setVisible(state);
            textResolution->setVisible(state);
            comboBoxResolution->setVisible(state);
            textColorDepth->setVisible(state);
            comboBoxColorDepth->setVisible(state);
            checkBoxVSync->setVisible(state);
            checkBoxStencilBuffer->setVisible(state);
            buttonToggleFullscreen->setVisible(state);
            buttonControlSettings->setVisible(state);
            buttonMenu->setVisible(state);
            buttonQuit->setVisible(state);
            break;
        }
    case (CONTROL_SETTINGS):
        {
            textScreenSize->setVisible(state);
            textControlUp->setVisible(state);
            buttonControlUp->setVisible(state);
            textControlLeft->setVisible(state);
            buttonControlLeft->setVisible(state);
            textControlDown->setVisible(state);
            buttonControlDown->setVisible(state);
            textControlRight->setVisible(state);
            buttonControlRight->setVisible(state);
            buttonDefultControls->setVisible(state);
            buttonSettings->setVisible(state);
            buttonQuit->setVisible(state);
            break;
        }
    case (TERMINATED):
        break;
    }
}

void GUI::resizeGUI()
{
    //changing gui position and size when window is resizing
    recalculateButtonProportions();
    switch (stage)
    {
    case (MENU):
        {
            buttonStart->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE));
            buttonSettings->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE));
            break;
        }
    case (INGAME_MENU):
        {
            buttonResume->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE));
            buttonMenu->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE));
            break;
        }
    case (SETTINGS):
        {
            textLanguage->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 9*buttonHeight - 9*SPACE, configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 9*SPACE));
            comboBoxLanguage->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 9*buttonHeight - 9*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 9*SPACE));
            textResolution->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 8*SPACE, configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 8*SPACE));
            comboBoxResolution->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 8*buttonHeight - 8*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 8*SPACE));
            textColorDepth->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 7*SPACE, configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 7*SPACE));
            comboBoxColorDepth->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 7*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 7*SPACE));
            checkBoxVSync->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 6*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 6*SPACE));
            checkBoxStencilBuffer->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 5*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 5*SPACE));
            buttonToggleFullscreen->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE));
            buttonControlSettings->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE));
            buttonMenu->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE));
            //if window size has changed, its resolution is not standard
            comboBoxResolution->removeItem(2);
            core::stringw scrs;
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            scrs += _wp(" (Custom)");
            comboBoxResolution->addItem(scrs.c_str(), 2);
            comboBoxResolution->setSelected(2);
            break;
        }
    case (CONTROL_SETTINGS):
        {
            textControlUp->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 7*SPACE, configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 7*SPACE));
            buttonControlUp->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 7*buttonHeight - 7*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 7*SPACE));;
            textControlLeft->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 6*SPACE, configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 6*SPACE));
            buttonControlLeft->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 6*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 6*SPACE));
            textControlDown->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 5*SPACE, configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 5*SPACE));
            buttonControlDown->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 5*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 5*SPACE));
            textControlRight->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE));
            buttonControlRight->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth/2 - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE));
            buttonDefultControls->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE));
            buttonSettings->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE));
            break;
        }
    case (HUD):
        break;
    case (TERMINATED):
        break;
    }
}
