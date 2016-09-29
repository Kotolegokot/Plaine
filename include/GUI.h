#ifndef GUI_H
#define GUI_H

#include "Config.h"
#include "util.h"

enum Stage { MENU, SETTINGS, INGAME_MENU, CONTROL_SETTINGS, TERMINATED };

class GUI
{
public:
    GUI(struct ConfigData &data, gui::IGUIEnvironment *guiEnv);
    ConfigData &configuration;
    int buttonWidth, buttonHeight;

    gui::IGUIEnvironment *guiEnvironment = nullptr;

    // GUI
    gui::IGUIButton *buttonStart = nullptr;
    gui::IGUIButton *buttonSettings = nullptr;
    gui::IGUIButton *buttonToggleFullscreen = nullptr;
    gui::IGUIButton *buttonControlSettings = nullptr;
    gui::IGUIButton *buttonControlUp = nullptr;
    gui::IGUIButton *buttonControlDown = nullptr;
    gui::IGUIButton *buttonControlLeft = nullptr;
    gui::IGUIButton *buttonControlRight = nullptr;
    gui::IGUIButton *buttonDefultControls = nullptr;
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
    gui::IGUIStaticText *textControlUp = nullptr;
    gui::IGUIStaticText *textControlDown = nullptr;
    gui::IGUIStaticText *textControlLeft = nullptr;
    gui::IGUIStaticText *textControlRight = nullptr;
    gui::IGUICheckBox *checkBoxVSync = nullptr;
    gui::IGUICheckBox *checkBoxStencilBuffer = nullptr;
    gui::IGUIWindow *settingsMessageBox = nullptr;


    void recalculateButtonProportions();
    void initialize(Stage stage);
    void terminate();
    void resizeGUI();
    void setVisible(bool state);
    Stage getStage();
private:
    Stage stage;
    const int SPACE = 10;

    void initializeMenuGUI();
    void initializeInGameGUI();
    void initializeSettingsGUI();
    void initializeControlSettingsGUI();

    void terminateMenuGUI();
    void terminateSettingsGUI();
    void terminateControlSettingsGUI();
    void terminateInGameGUI();
};

#endif // GUI_H
