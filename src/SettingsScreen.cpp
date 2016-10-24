#include "SettingsScreen.h"

SettingsScreen::SettingsScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    IGUIScreen(configuration, guiEnvironment) {}

SettingsScreen::~SettingsScreen()
{
    terminate();
}

void SettingsScreen::initialize(s32 buttonWidth, s32 buttonHeight)
{
    textScreenSize = std::shared_ptr<gui::IGUIStaticText>
        (guiEnvironment.addStaticText(L"SCREEN_SIZE", core::rect<s32>(0, 0, 0, 0)));
    textLanguage = std::shared_ptr<gui::IGUIStaticText>
        (guiEnvironment.addStaticText(_wp("Language:"), core::rect<s32>(0, 0, 0, 0)));

    comboBoxLanguage = std::shared_ptr<gui::IGUIComboBox>(guiEnvironment.addComboBox(core::rect<s32>(0, 0, 0, 0)));
    comboBoxLanguage->setID(ID_COMBOBOX_LANGUAGE);
    comboBoxLanguage->addItem(_wp("System language"), 0);
    comboBoxLanguage->addItem(L"English", 1);
    comboBoxLanguage->addItem(L"Русский", 2);
    comboBoxLanguage->addItem(L"Qırımtatarca", 3);
    comboBoxLanguage->addItem(L"Къырымтатарджа", 4);
    if (configuration.language.empty())
        comboBoxLanguage->setSelected(0);
    else if (configuration.language == "en")
        comboBoxLanguage->setSelected(1);
    else if (configuration.language == "ru")
        comboBoxLanguage->setSelected(2);
    else if (configuration.language == "crh")
        comboBoxLanguage->setSelected(3);
    else if (configuration.language == "crh@cyrillic")
        comboBoxLanguage->setSelected(4);

    textResolution = std::shared_ptr<gui::IGUIStaticText>
        (guiEnvironment.addStaticText(_wp("Resolution:"), core::rect<s32>(0, 0, 0, 0)));

    comboBoxResolution = std::shared_ptr<gui::IGUIComboBox>(guiEnvironment.addComboBox(core::rect<s32>(0, 0, 0, 0)));
    comboBoxResolution->setID(ID_COMBOBOX_RESOLUTION);
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

    } else {
        comboBoxResolution->addItem(L"640x480", 0);
        comboBoxResolution->addItem(L"1240x720", 1);
        if (configuration.resizable) {
            core::stringw scrs;
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            scrs += _w(" (Custom)");
            comboBoxResolution->addItem(scrs.c_str(), 2);
            comboBoxResolution->setSelected(2);
        } else if (configuration.resolution == core::dimension2d<u32>(640, 480)) {
            comboBoxResolution->setSelected(0);
            comboBoxResolution->addItem(_wp("Custom Resolution"), 2);
        } else if (configuration.resolution == core::dimension2d<u32>(1240, 720)) {
            comboBoxResolution->setSelected(1);
            comboBoxResolution->addItem(_wp("Custom Resolution"), 2);
        } else {
            core::stringw scrs;
            scrs += configuration.resolution.Width;
            scrs += "x";
            scrs += configuration.resolution.Height;
            scrs += _w(" (Custom)");
            comboBoxResolution->addItem(scrs.c_str(), 2);
            comboBoxResolution->setSelected(2);
        }
    }

    textRenderDistance = std::shared_ptr<gui::IGUIStaticText>
        (guiEnvironment.addStaticText(_wp("Render distance:"), core::rect<s32>(0, 0, 0, 0)));
    spinBoxRenderDistance = std::shared_ptr<gui::IGUISpinBox>
        (guiEnvironment.addSpinBox(_wp("Render distance"), core::rect<s32>(0, 0, 0, 0)));
    spinBoxRenderDistance->setID(ID_SPINBOX_RENDER_DISTANCE);
    spinBoxRenderDistance->setRange(500, 5000);
    spinBoxRenderDistance->setStepSize(100);
    spinBoxRenderDistance->setValue(configuration.renderDistance);

    checkBoxVSync = std::shared_ptr<gui::IGUICheckBox>
        (guiEnvironment.addCheckBox(_wp("VSync"), core::rect<s32>(0, 0, 0, 0)));
    checkBoxVSync->setID(ID_CHECKBOX_VSYNC);

    checkBoxStencilBuffer = std::shared_ptr<gui::IGUICheckBox>
        (guiEnvironment.addCheckBox(_wp("Stencil buffer"), core::rect<s32>(0, 0, 0, 0)));
    checkBoxStencilBuffer->setID(ID_CHECKBOX_STENCILBUFFER);

    buttonToggleFullscreen = std::shared_ptr<gui::IGUIButton>(guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0)));
    buttonToggleFullscreen->setID(ID_BUTTON_TOGGLE_FULLSCREEN);
    if (configuration.fullscreen) {
        buttonToggleFullscreen->setText(_wp("Windowed"));
        buttonToggleFullscreen->setToolTipText(_wp("To windowed mode"));
    } else {
        buttonToggleFullscreen->setText(_wp("Fullscreen"));
        buttonToggleFullscreen->setToolTipText(_wp("To fullscreen mode"));
    }

    buttonControlSettings = std::shared_ptr<gui::IGUIButton>(guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0)));
    buttonControlSettings->setID(ID_BUTTON_CONTROL_SETTINGS);
    buttonControlSettings->setText(_wp("Controls"));
    buttonControlSettings->setToolTipText(_wp("Edit control settings"));

    buttonMenu = std::shared_ptr<gui::IGUIButton>(guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0)));
    buttonMenu->setID(ID_BUTTON_MENU);
    buttonMenu->setText(_wp("Back"));
    buttonMenu->setToolTipText(_wp("Exit to main menu"));

    buttonQuit = std::shared_ptr<gui::IGUIButton>(guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0)));
    buttonQuit->setID(ID_BUTTON_QUIT);
    buttonQuit->setText(_wp("Quit"));
    buttonQuit->setToolTipText(_wp("Exit game"));

    resize(buttonWidth, buttonHeight);
}

void SettingsScreen::terminate()
{
    textScreenSize->remove();
    textScreenSize.reset();

    textLanguage->remove();
    textLanguage.reset();

    textResolution->remove();
    textResolution.reset();

    textRenderDistance->remove();
    textRenderDistance.reset();

    comboBoxLanguage->remove();
    comboBoxLanguage.reset();

    comboBoxResolution->remove();
    comboBoxResolution.reset();

    spinBoxRenderDistance->remove();
    spinBoxRenderDistance.reset();

    checkBoxVSync->remove();
    checkBoxVSync.reset();

    checkBoxStencilBuffer->remove();
    checkBoxStencilBuffer.reset();

    buttonToggleFullscreen->remove();
    buttonToggleFullscreen.reset();

    buttonControlSettings->remove();
    buttonControlSettings.reset();

    buttonMenu->remove();
    buttonMenu.reset();

    buttonQuit->remove();
    buttonQuit.reset();
}

void SettingsScreen::resize(s32 buttonWidth, s32 buttonHeight)
{
    textLanguage->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3 * buttonWidth / 2 - 2 * space,
                                                      configuration.resolution.Height - 9 * buttonHeight - 9 * space,
                                                      configuration.resolution.Width - buttonWidth - 2 * space,
                                                      configuration.resolution.Height - 8 * buttonHeight - 9 * space));

    comboBoxLanguage->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                          configuration.resolution.Height - 9 * buttonHeight - 9 * space,
                                                          configuration.resolution.Width - 2 * space,
                                                          configuration.resolution.Height - 8 * buttonHeight - 9 * space));

    textResolution->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3 * buttonWidth / 2 - 2 * space,
                                                        configuration.resolution.Height - 8 * buttonHeight - 8 * space,
                                                        configuration.resolution.Width - buttonWidth - 2 * space,
                                                        configuration.resolution.Height - 7 * buttonHeight - 8 * space));

    comboBoxResolution->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                            configuration.resolution.Height - 8 * buttonHeight - 8 * space,
                                                            configuration.resolution.Width - 2 * space,
                                                            configuration.resolution.Height - 7 * buttonHeight - 8 * space));

    textRenderDistance->setRelativePosition(core::rect<s32>(configuration.resolution.Width - 3 * buttonWidth / 2 - 2 * space,
                                                            configuration.resolution.Height - 7 * buttonHeight - 7 * space,
                                                            configuration.resolution.Width - buttonWidth - 2 * space,
                                                            configuration.resolution.Height - 6 * buttonHeight - 7*space));

    spinBoxRenderDistance->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                            configuration.resolution.Height - 7 * buttonHeight - 7 * space,
                                                            configuration.resolution.Width - 2 * space,
                                                            configuration.resolution.Height - 6 * buttonHeight - 7 * space));

    checkBoxVSync->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                       configuration.resolution.Height - 6 * buttonHeight - 6 * space,
                                                       configuration.resolution.Width - 2 * space,
                                                       configuration.resolution.Height - 5 * buttonHeight - 6 * space));

    checkBoxStencilBuffer->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                               configuration.resolution.Height - 5 * buttonHeight - 5 * space,
                                                               configuration.resolution.Width - 2 * space,
                                                               configuration.resolution.Height - 4 * buttonHeight - 5 * space));

    buttonToggleFullscreen->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                                configuration.resolution.Height - 4 * buttonHeight - 4 * space,
                                                                configuration.resolution.Width - 2 * space,
                                                                configuration.resolution.Height - 3 * buttonHeight - 4 * space));

    buttonControlSettings->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                               configuration.resolution.Height - 3 * buttonHeight - 3 * space,
                                                               configuration.resolution.Width - 2 * space,
                                                               configuration.resolution.Height - 2 * buttonHeight - 3 * space));

    buttonMenu->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                    configuration.resolution.Height - 2 * buttonHeight - 2 * space,
                                                    configuration.resolution.Width - 2 * space,
                                                    configuration.resolution.Height - buttonHeight - 2 * space));

    buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                    configuration.resolution.Height - buttonHeight - space,
                                                    configuration.resolution.Width - 2 * space,
                                                    configuration.resolution.Height - space));

    //if window size has changed, its resolution is not standard
    comboBoxResolution->removeItem(2);
    core::stringw scrs;
    scrs += configuration.resolution.Width;
    scrs += "x";
    scrs += configuration.resolution.Height;
    scrs += _wp(" (Custom)");
    comboBoxResolution->addItem(scrs.c_str(), 2);
    comboBoxResolution->setSelected(2);
}

std::vector<std::weak_ptr<gui::IGUIElement>> SettingsScreen::getSelectableElements()
{
    return { checkBoxVSync, checkBoxStencilBuffer, buttonToggleFullscreen,
        buttonControlSettings, buttonMenu, buttonMenu };
}

void SettingsScreen::setVisible(bool visible)
{
    textScreenSize->setVisible(visible);
    textLanguage->setVisible(visible);
    textResolution->setVisible(visible);
    textRenderDistance->setVisible(visible);
    comboBoxLanguage->setVisible(visible);
    comboBoxResolution->setVisible(visible);
    spinBoxRenderDistance->setVisible(visible);
    checkBoxVSync->setVisible(visible);
    checkBoxStencilBuffer->setVisible(visible);
    buttonToggleFullscreen->setVisible(visible);
    buttonControlSettings->setVisible(visible);
    buttonMenu->setVisible(visible);
    buttonQuit->setVisible(visible);
}
