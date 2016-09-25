#include "GUI.h"

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
    buttonStart = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE),
        0, ID_BUTTON_START, _wp("Start"), _wp("Start game"));
    buttonSettings = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE),
        0, ID_BUTTON_SETTINGS, _wp("Settings"), _wp("Game settings"));
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE),
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
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    buttonResume = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE), 0, ID_BUTTON_RESUME, _wp("Resume"), _wp("Resume Game"));
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE),
        0, ID_BUTTON_MENU, _wp("Menu"), _wp("Exit to Main menu"));
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE),
        0, ID_BUTTON_QUIT, _wp("Quit"), _wp("Exit game"));
    stage = INGAME_MENU;
}

void GUI::terminateInGameGUI()
{
    buttonResume->remove();
    buttonMenu->remove();
    buttonQuit->remove();
    cameraPosText->remove();
    screenSizeText->remove();
}

void GUI::initializeSettingsGUI()
{
    recalculationButtonProportions();
    screenSizeText = guiEnvironment->addStaticText(L"SCREEN_SIZE", core::rect<s32>(10, 10, 200, 30), false);
    languageText = guiEnvironment->addStaticText(_wp("Language:"), core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 3*SPACE, configuration.resolution.Height - 6*buttonHeight - 6*SPACE, configuration.resolution.Width - buttonWidth - 3*SPACE, configuration.resolution.Height - 5*buttonHeight - 6*SPACE), false);
    languageComboBox = guiEnvironment->addComboBox(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 6*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 6*SPACE), 0, ID_COMBOBOX_LANGUAGE);
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
    resolutionText = guiEnvironment->addStaticText(_wp("Resolution:"), core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 3*SPACE, configuration.resolution.Height - 5*buttonHeight - 5*SPACE, configuration.resolution.Width - buttonWidth - 3*SPACE, configuration.resolution.Height - 4*buttonHeight - 5*SPACE), false);
    resolutionComboBox = guiEnvironment->addComboBox(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 5*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 5*SPACE), 0, ID_COMBOBOX_RESOLUTION);
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
        buttonToggleFullscreen = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE), 0, ID_BUTTON_TOGGLE_FULLSCREEN, _wp("Windowed"), _wp("To Windowed Mode"));

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
        buttonToggleFullscreen = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE), 0, ID_BUTTON_TOGGLE_FULLSCREEN, _wp("Fullscreen"), _wp("To Fullscreen Mode"));

    }
    colorDepthText = guiEnvironment->addStaticText(_wp("Color Depth:"), core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 3*SPACE, configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - buttonWidth - 3*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE), false);
    colorDepthComboBox = guiEnvironment->addComboBox(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE), 0, ID_COMBOBOX_COLORDEPTH);
    colorDepthComboBox->addItem(L"8", 0);
    colorDepthComboBox->addItem(L"16", 1);
    colorDepthComboBox->addItem(L"32", 2);
    if (configuration.colordepth == 8)
        colorDepthComboBox->setSelected(0);
    else if (configuration.colordepth == 16)
        colorDepthComboBox->setSelected(1);
    else if (configuration.colordepth == 32)
        colorDepthComboBox->setSelected(2);
    buttonMenu = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE),
        0, ID_BUTTON_MENU, _wp("Back"), _wp("Exit to Main menu"));
    buttonQuit = guiEnvironment->addButton(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE),
        0, ID_BUTTON_QUIT, _wp("Quit"), _wp("Exit game"));
    stage = SETTINGS;
}

void GUI::terminateSettingsGUI()
{
    screenSizeText->remove();
    languageText->remove();
    languageComboBox->remove();
    resolutionText->remove();
    resolutionComboBox->remove();
    colorDepthText->remove();
    colorDepthComboBox->remove();
    buttonToggleFullscreen->remove();
    buttonMenu->remove();
    buttonQuit->remove();
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
            languageText->setVisible(state);
            languageComboBox->setVisible(state);
            resolutionText->setVisible(state);
            resolutionComboBox->setVisible(state);
            colorDepthText->setVisible(state);
            colorDepthComboBox->setVisible(state);
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
            buttonStart->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE));
            buttonSettings->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE));
            break;
        }
    case (INGAME_MENU):
        {
            recalculationButtonProportions();
            buttonResume->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE));
            buttonMenu->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE));
            break;
        }
    case (SETTINGS):
        {
            recalculationButtonProportions();
            languageText->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 3*SPACE, configuration.resolution.Height - 6*buttonHeight - 6*SPACE, configuration.resolution.Width - buttonWidth - 3*SPACE, configuration.resolution.Height - 5*buttonHeight - 6*SPACE));
            languageComboBox->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 6*buttonHeight - 6*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 6*SPACE));
            resolutionText->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 3*SPACE, configuration.resolution.Height - 5*buttonHeight - 5*SPACE, configuration.resolution.Width - buttonWidth - 3*SPACE, configuration.resolution.Height - 4*buttonHeight - 5*SPACE));
            resolutionComboBox->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 5*buttonHeight - 5*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 5*SPACE));
            colorDepthText->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3*buttonWidth/2 - 3*SPACE, configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - buttonWidth - 3*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE));
            colorDepthComboBox->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 4*buttonHeight - 4*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 4*SPACE));
            buttonToggleFullscreen->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 3*buttonHeight - 3*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 3*SPACE));
            buttonMenu->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - 2*buttonHeight - 2*SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - buttonHeight - 2*SPACE));
            buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2*SPACE, configuration.resolution.Height - buttonHeight - SPACE, configuration.resolution.Width - 2*SPACE, configuration.resolution.Height - SPACE));
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
