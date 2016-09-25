#ifndef GUI_H
#define GUI_H

#include "Config.h"
#include "util.h"

enum Stage{ MENU, SETTINGS, INGAME_MENU, CONTROL_SETTINGS, TERMINATED };

class GUI
{
public:
    GUI(struct ConfigData &data, gui::IGUIEnvironment *guiEnv) : configuration(data), guiEnvironment(guiEnv){}

    ConfigData &configuration;
    int buttonWidth, buttonHeight;
    const int SPACE = 10;
    Stage stage;

    gui::IGUIEnvironment *guiEnvironment = nullptr;

    // GUI
    gui::IGUIButton *buttonStart = nullptr;
    gui::IGUIButton *buttonSettings = nullptr;
    gui::IGUIButton *buttonToggleFullscreen = nullptr;
    gui::IGUIButton *buttonControlSettings = nullptr;
    gui::IGUIButton *buttonResume = nullptr;
    gui::IGUIButton *buttonMenu = nullptr;
    gui::IGUIButton *buttonQuit = nullptr;
    gui::IGUIComboBox *comboBoxResolution = nullptr;
    gui::IGUIComboBox *comboBoxColorDepth = nullptr;
    gui::IGUIComboBox *comboBoxLanguage = nullptr;
    gui::IGUIStaticText *textScreenSize = nullptr;
    gui::IGUIStaticText *textCubeCount = nullptr;
    gui::IGUIStaticText *textResolution = nullptr;
    gui::IGUIStaticText *textColorDepth = nullptr;
    gui::IGUIStaticText *textLanguage = nullptr;
    gui::IGUIStaticText *textCameraPos = nullptr;
    gui::IGUICheckBox *checkBoxVSync = nullptr;
    gui::IGUICheckBox *checkBoxStencilBuffer = nullptr;


    void recalculateButtonProportions();
    void initializeMenuGUI();
    void initializeInGameGUI();
    void initializeSettingsGUI();
    void initializeControlSettingsGUI();
    void terminateGUI();
    void resizeGUI();
    void setVisible(bool state);
private:
    void terminateMenuGUI();
    void terminateSettingsGUI();
    void terminateControlSettingsGUI();
    void terminateInGameGUI();
};

#endif // GUI_H
