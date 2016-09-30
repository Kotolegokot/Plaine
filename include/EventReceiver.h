#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht.h>
#include "util.h"
#include "GUI.h"

using namespace irr;

// this class receives different kinds of event
//      like keys pressed or buttons clicked
class EventReceiver : public IEventReceiver
{
public:
    EventReceiver();
    virtual bool OnEvent(const SEvent &event);
    virtual bool IsKeyDown(EKEY_CODE keyCode) const;

    bool quit = false;
    bool start = false;
    bool changingControlUp = false;
    bool changingControlDown = false;
    bool changingControlLeft = false;
    bool changingControlRight = false;
    bool defaultControls = false;
    bool toggleGUI = false;
    bool toggleColorDepth = false;
    bool toggleFullscreen = false;
    bool needRestartInMenu = false;
    bool toggleResolution = false;
    bool toggleLanguage = false;
    bool resume = false;
    bool escapePressed = false;

    // states of GUI
    //
    // see GUI.h additional for information
    GUIState state = MENU;

    EKEY_CODE lastKey = KEY_KEY_CODES_COUNT;
private:
    // this array shows which keys are pressed and which are not
    bool PressedKeys[KEY_KEY_CODES_COUNT];
};

#endif // EVENTRECEIVER_H
