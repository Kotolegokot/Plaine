#ifndef GUI_H
#define GUI_H

#include "Config.h"
#include "util.h"

enum Stage{ MENU, SETTINGS, INGAME_MENU };

class GUI
{
public:
    GUI(struct ConfigData &data, gui::IGUIEnvironment *guiEnv) : configuration(data), guiEnvironment(guiEnv){}

    ConfigData &configuration;
    int buttonWidth, buttonHeight;
    Stage stage;

    gui::IGUIEnvironment *guiEnvironment = nullptr;

    // GUI
    gui::IGUIButton *buttonStart = nullptr;
    gui::IGUIButton *buttonSettings = nullptr;
    gui::IGUIButton *buttonToggleFullscreen = nullptr;
    gui::IGUIButton *buttonResume = nullptr;
    gui::IGUIButton *buttonMenu = nullptr;
    gui::IGUIButton *buttonQuit = nullptr;
    gui::IGUIComboBox *resolutionComboBox = nullptr;
    gui::IGUIComboBox *colorDepthComboBox = nullptr;
    gui::IGUIComboBox *languageComboBox = nullptr;
    gui::IGUIStaticText *screenSizeText = nullptr;
    gui::IGUIStaticText *resolutionText = nullptr;
    gui::IGUIStaticText *colorDepthText = nullptr;
    gui::IGUIStaticText *languageText = nullptr;
    gui::IGUIStaticText *cameraPosText = nullptr;


    void recalculationButtonProportions();
    void initializeMenuGUI();
    void initializeInGameGUI();
    void initializeSettingsGUI();
    void terminateGUI();
    void resizeGUI();
    void setVisible(bool state);
private:
    void terminateMenuGUI();
    void terminateSettingsGUI();
    void terminateInGameGUI();
};

#endif // GUI_H
