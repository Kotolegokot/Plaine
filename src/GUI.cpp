#include "GUI.h"

using namespace irr;

void GUI::recalculationButtonProportions()
{
    buttonWidth = configuration.resolution.Width / 6;
    if (buttonWidth < 180)
        buttonWidth = 180;
    buttonHeight = buttonWidth * 1/8;
}

void GUI::initializeMenuGUI()
{
    recalculationButtonProportions();
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonStart = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 3*buttonHeight - 30, configuration.resolution.Width - 20, configuration.resolution.Height - 2*buttonHeight - 30),
        0, ID_BUTTON_START, _wp("Start"), _wp("Start game"));
    buttonSettings = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 2*buttonHeight - 20, configuration.resolution.Width - 20, configuration.resolution.Height - buttonHeight - 20),
        0, ID_BUTTON_SETTINGS, _wp("Settings"), _wp("Game settings"));
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - buttonHeight - 10, configuration.resolution.Width - 20, configuration.resolution.Height - 10),
        0, ID_BUTTON_QUIT, _wp("Quit"), _wp("Exit game"));
    stage = MENU;
}

void GUI::terminateMenuGUI()
{
    buttonStart->remove();
    buttonSettings->remove();
    buttonQuit->remove();
    screenSizeText->remove();
}

void GUI::initializeInGameGUI()
{
    recalculationButtonProportions();
    cameraPosText = guiEnvironment->addStaticText(L"CAMERA_POS", core::rect<s32>(10, 10, 400, 30), false);
    cameraPosText->setBackgroundColor(video::SColor(120, 255, 255, 255));
    cubeCountText = guiEnvironment->addStaticText(L"CUBE_COUNT", core::rect<s32>(10, 10 + 24, 400, 30 + 24), false);
    cubeCountText->setBackgroundColor(video::SColor(120, 255, 255, 255));
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonResume = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 3*buttonHeight - 30, configuration.resolution.Width - 20, configuration.resolution.Height - 2*buttonHeight - 30), 0, ID_BUTTON_RESUME, _wp("Resume"), _wp("Resume Game"));
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 2*buttonHeight - 20, configuration.resolution.Width - 20, configuration.resolution.Height - buttonHeight - 20),
        0, ID_BUTTON_MENU, _wp("Menu"), _wp("Exit to Main menu"));
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - buttonHeight - 10, configuration.resolution.Width - 20, configuration.resolution.Height - 10),
        0, ID_BUTTON_QUIT, _wp("Quit"), _wp("Exit game"));
    stage = INGAME_MENU;
}

void GUI::terminateInGameGUI()
{
    buttonResume->remove();
    buttonMenu->remove();
    buttonQuit->remove();
    cameraPosText->remove();
    cubeCountText->remove();
    screenSizeText->remove();
}

void GUI::initializeSettingsGUI()
{
    recalculationButtonProportions();
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    resolutionText = guiEnvironment->addStaticText(_wp("Resolution:"), core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 30, configuration.resolution.Height - 6*buttonHeight - 60, configuration.resolution.Width - buttonWidth - 30, configuration.resolution.Height - 5*buttonHeight - 60), false);
    resolutionComboBox = guiEnvironment->addComboBox(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 6*buttonHeight - 60, configuration.resolution.Width - 20, configuration.resolution.Height - 5*buttonHeight - 60), 0, ID_COMBOBOX_RESOLUTION);
    if (configuration.fullscreen)
    {
        core::stringw scrs;
        scrs += configuration.resolution.Width;
        scrs += "x";
        scrs += configuration.resolution.Height;
        scrs += _w(" (Fullscreen)");
        resolutionComboBox->addItem(scrs.c_str(), 0);
        resolutionComboBox->setSelected(0);
        resolutionComboBox->setEnabled(false);
        buttonToggleFullscreen = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 3*buttonHeight - 30, configuration.resolution.Width - 20, configuration.resolution.Height - 2*buttonHeight - 30), 0, ID_BUTTON_TOGGLE_FULLSCREEN, _wp("Windowed"), _wp("To Windowed Mode"));

    }
    else {
        resolutionComboBox->addItem(L"640x480", 0);
        resolutionComboBox->addItem(L"1240x720", 1);
        if (configuration.resizable)
        {
            core::stringw scrs;
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            scrs += _w(" (Custom)");
            resolutionComboBox->addItem(scrs.c_str(), 2);
            resolutionComboBox->setSelected(2);
        }
        else if (configuration.resolution == core::dimension2d<u32>(640, 480))
        {
            resolutionComboBox->setSelected(0);
            resolutionComboBox->addItem(_wp("Custom Resolution"), 2);
        }
        else if (configuration.resolution == core::dimension2d<u32>(1240, 720))
        {
            resolutionComboBox->setSelected(1);
            resolutionComboBox->addItem(_wp("Custom Resolution"), 2);
        }
        else
        {
            core::stringw scrs;
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            scrs += _w(" (Custom)");
            resolutionComboBox->addItem(scrs.c_str(), 2);
            resolutionComboBox->setSelected(2);
        }
        buttonToggleFullscreen = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 3*buttonHeight - 30, configuration.resolution.Width - 20, configuration.resolution.Height - 2*buttonHeight - 30), 0, ID_BUTTON_TOGGLE_FULLSCREEN, _wp("Fullscreen"), _wp("To Fullscreen Mode"));

    }
    colorDepthText = guiEnvironment->addStaticText(_wp("Color Depth:"), core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 30, configuration.resolution.Height - 5*buttonHeight - 50, configuration.resolution.Width - buttonWidth - 30, configuration.resolution.Height - 4*buttonHeight - 50), false);
    colorDepthComboBox = guiEnvironment->addComboBox(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 5*buttonHeight - 50, configuration.resolution.Width - 20, configuration.resolution.Height - 4*buttonHeight - 50), 0, ID_COMBOBOX_COLORDEPTH);
    colorDepthComboBox->addItem(L"8", 0);
    colorDepthComboBox->addItem(L"16", 1);
    colorDepthComboBox->addItem(L"32", 2);
    if (configuration.colordepth == 8)
        colorDepthComboBox->setSelected(0);
    else if (configuration.colordepth == 16)
        colorDepthComboBox->setSelected(1);
    else if (configuration.colordepth == 32)
        colorDepthComboBox->setSelected(2);
    languageText = guiEnvironment->addStaticText(_wp("Language:"), core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 30, configuration.resolution.Height - 4*buttonHeight - 40, configuration.resolution.Width - buttonWidth - 30, configuration.resolution.Height - 3*buttonHeight - 40), false);
    languageComboBox = guiEnvironment->addComboBox(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 4*buttonHeight - 40, configuration.resolution.Width - 20, configuration.resolution.Height - 3*buttonHeight - 40), 0, ID_COMBOBOX_LANGUAGE);
    languageComboBox->addItem(L"English", 0);
    languageComboBox->addItem(L"Русский", 1);
    languageComboBox->addItem(L"Qırımtatarca", 2);
    languageComboBox->addItem(L"Къырымтатарджа", 3);
    if (configuration.language == "en")
        languageComboBox->setSelected(0);
    else if (configuration.language == "ru")
        languageComboBox->setSelected(1);
    else if (configuration.language == "crh")
        languageComboBox->setSelected(2);
    else if (configuration.language == "crh@cyrillic")
        languageComboBox->setSelected(3);
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 2*buttonHeight - 20, configuration.resolution.Width - 20, configuration.resolution.Height - buttonHeight - 20),
        0, ID_BUTTON_MENU, _wp("Back"), _wp("Exit to Main menu"));
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - buttonHeight - 10, configuration.resolution.Width - 20, configuration.resolution.Height - 10),
        0, ID_BUTTON_QUIT, _wp("Quit"), _wp("Exit game"));
    stage = SETTINGS;
}

void GUI::terminateSettingsGUI()
{
    buttonMenu->remove();
    buttonQuit->remove();
    buttonToggleFullscreen->remove();
    screenSizeText->remove();
    resolutionText->remove();
    resolutionComboBox->remove();
    colorDepthText->remove();
    colorDepthComboBox->remove();
    languageText->remove();
    languageComboBox->remove();
}

void GUI::terminateGUI()
{
    switch (stage)
    {
    case(MENU):
        {
            terminateMenuGUI();
            stage = TERMINATED;
            break;
        }
    case(INGAME_MENU):
        {
            terminateInGameGUI();
            stage = TERMINATED;
            break;
        }
    case(SETTINGS):
        {
            terminateSettingsGUI();
            stage = TERMINATED;
            break;
        }
    case(TERMINATED):
        break;
    }
}

void GUI::setVisible(bool state)
{
    switch (stage)
    {
    case (MENU):
        {
            buttonStart->setVisible(state);
            buttonMenu->setVisible(state);
            buttonQuit->setVisible(state);
            break;
        }
    case (INGAME_MENU):
        {
            buttonResume->setVisible(state);
            buttonMenu->setVisible(state);
            buttonQuit->setVisible(state);
            screenSizeText->setVisible(state);
            break;
        }
    case (SETTINGS):
        {
            screenSizeText->setVisible(state);
            resolutionText->setVisible(state);
            resolutionComboBox->setVisible(state);
            colorDepthText->setVisible(state);
            colorDepthComboBox->setVisible(state);
            languageText->setVisible(state);
            languageComboBox->setVisible(state);
            buttonToggleFullscreen->setVisible(state);
            buttonMenu->setVisible(state);
            buttonQuit->setVisible(state);
            break;
        }
    case (TERMINATED):
        break;
    }
}

void GUI::resizeGUI()
{
    switch (stage)
    {
    case (MENU):
        {
            recalculationButtonProportions();
            buttonStart->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 3*buttonHeight - 30, configuration.resolution.Width - 20, configuration.resolution.Height - 2*buttonHeight - 30));
            buttonSettings->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 2*buttonHeight - 20, configuration.resolution.Width - 20, configuration.resolution.Height - buttonHeight - 20));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - buttonHeight - 10, configuration.resolution.Width - 20, configuration.resolution.Height - 10));
            break;
        }
    case (INGAME_MENU):
        {
            recalculationButtonProportions();
            buttonResume->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 3*buttonHeight - 30, configuration.resolution.Width - 20, configuration.resolution.Height - 2*buttonHeight - 30));
            buttonMenu->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 2*buttonHeight - 20, configuration.resolution.Width - 20, configuration.resolution.Height - buttonHeight - 20));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - buttonHeight - 10, configuration.resolution.Width - 20, configuration.resolution.Height - 10));
            break;
        }
    case (SETTINGS):
        {
            recalculationButtonProportions();
            resolutionText->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 30, configuration.resolution.Height - 6*buttonHeight - 60, configuration.resolution.Width - buttonWidth - 30, configuration.resolution.Height - 5*buttonHeight - 60));
            resolutionComboBox->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 6*buttonHeight - 60, configuration.resolution.Width - 20, configuration.resolution.Height - 5*buttonHeight - 60));
            colorDepthText->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 30, configuration.resolution.Height - 5*buttonHeight - 50, configuration.resolution.Width - buttonWidth - 30, configuration.resolution.Height - 4*buttonHeight - 50));
            colorDepthComboBox->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 5*buttonHeight - 50, configuration.resolution.Width - 20, configuration.resolution.Height - 4*buttonHeight - 50));
            languageText->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 30, configuration.resolution.Height - 4*buttonHeight - 40, configuration.resolution.Width - buttonWidth - 30, configuration.resolution.Height - 3*buttonHeight - 40));
            languageComboBox->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 4*buttonHeight - 40, configuration.resolution.Width - 20, configuration.resolution.Height - 3*buttonHeight - 40));
            buttonToggleFullscreen->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 3*buttonHeight - 30, configuration.resolution.Width - 20, configuration.resolution.Height - 2*buttonHeight - 30));
            buttonMenu->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - 2*buttonHeight - 20, configuration.resolution.Width - 20, configuration.resolution.Height - buttonHeight - 20));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 20, configuration.resolution.Height - buttonHeight - 10, configuration.resolution.Width - 20, configuration.resolution.Height - 10));
            resolutionComboBox->removeItem(2);
            core::stringw scrs;
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            scrs += _wp(" (Custom)");
            resolutionComboBox->addItem(scrs.c_str(), 2);
            resolutionComboBox->setSelected(2);
            break;
        }
    case (TERMINATED):
        break;
    }
}
