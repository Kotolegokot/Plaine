#include "screens/ControlSettingsScreen.h"

ControlSettingsScreen::ControlSettingsScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    IGUIScreen(configuration, guiEnvironment) {}

ControlSettingsScreen::~ControlSettingsScreen()
{
    terminate();
}

void ControlSettingsScreen::initialize(s32 buttonWidth, s32 buttonHeight)
{
    textScreenSize = guiEnvironment.addStaticText(L"SCREEN_SIZE", core::rect<s32>(0, 0, 0, 0));

    static constexpr std::array<const char *, CONTROLS_COUNT> texts = { "Up: ", "Left: ", "Down: ", "Right: ", "CW roll", "CCW roll" };
    static constexpr std::array<u32, CONTROLS_COUNT> ids = { ID_BUTTON_CONTROL_UP, ID_BUTTON_CONTROL_LEFT,
                                                             ID_BUTTON_CONTROL_DOWN, ID_BUTTON_CONTROL_RIGHT,
                                                             ID_BUTTON_CONTROL_CW_ROLL, ID_BUTTON_CONTROL_CCW_ROLL };
    for (size_t i = 0; i < CONTROLS_COUNT; i++) {
        textsControl[i] = guiEnvironment.addStaticText(_wp(texts[i]), core::rect<s32>(0, 0, 0, 0));

        buttonsControl[i] = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
        buttonsControl[i]->setID(ids[i]);
    }

    buttonDefaultControls = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonDefaultControls->setID(ID_BUTTON_DEFAULT_CONTROLS);
    buttonDefaultControls->setText(_wp("Default"));
    buttonDefaultControls->setToolTipText(_wp("Default control settings"));

    buttonSettings = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonSettings->setID(ID_BUTTON_SETTINGS);
    buttonSettings->setText(_wp("Settings"));
    buttonSettings->setToolTipText(_wp("Back to game settings"));

    buttonQuit = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonQuit->setID(ID_BUTTON_QUIT);
    buttonQuit->setText(_wp("Quit"));
    buttonQuit->setToolTipText(_wp("Exit game"));

    resize(buttonWidth, buttonHeight);
}

void ControlSettingsScreen::terminate()
{
    for (size_t i = 0; i < CONTROLS_COUNT; i++) {
        textsControl[i]->remove();
        buttonsControl[i]->remove();
    }

    buttonDefaultControls->remove();
    buttonSettings->remove();
    buttonQuit->remove();
    textScreenSize->remove();
}

void ControlSettingsScreen::resize(s32 buttonWidth, s32 buttonHeight)
{
    for (size_t i = 0; i < CONTROLS_COUNT; i++) {
        textsControl[i]->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                             configuration.resolution.Height - (9 - i) * (buttonHeight + space),
                                                             configuration.resolution.Width - buttonWidth / 2 - 2 * space,
                                                             configuration.resolution.Height - (8 - i) * buttonHeight - (9 - i) * space));

        buttonsControl[i]->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth / 2 - 2 * space,
                                                               configuration.resolution.Height - (9 - i) * (buttonHeight + space),
                                                               configuration.resolution.Width - 2 * space,
                                                               configuration.resolution.Height - (8 - i) * buttonHeight - (9 - i) * space));
    }

    buttonDefaultControls->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                               configuration.resolution.Height - 3 * buttonHeight - 3 * space,
                                                               configuration.resolution.Width - 2 * space,
                                                               configuration.resolution.Height - 2 * buttonHeight - 3 * space));

    buttonSettings->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                        configuration.resolution.Height - 2 * buttonHeight - 2 * space,
                                                        configuration.resolution.Width - 2 * space,
                                                        configuration.resolution.Height - buttonHeight - 2 * space));

    buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * space,
                                                    configuration.resolution.Height - buttonHeight - space,
                                                    configuration.resolution.Width - 2 * space,
                                                    configuration.resolution.Height - space));
}

std::vector<gui::IGUIElement *> ControlSettingsScreen::getSelectableElements()
{
    std::vector<gui::IGUIElement *> selectableElements;
    for (auto &buttonControl : buttonsControl)
        selectableElements.push_back(buttonControl);


    selectableElements.push_back(buttonDefaultControls);
    selectableElements.push_back(buttonSettings);
    selectableElements.push_back(buttonQuit);

    return selectableElements;
}

void ControlSettingsScreen::setVisible(bool visible)
{
    textScreenSize->setVisible(visible);
    for (auto &buttonControl : buttonsControl)
        buttonControl->setVisible(visible);
    buttonDefaultControls->setVisible(visible);
    buttonSettings->setVisible(visible);
    buttonQuit->setVisible(visible);
}
