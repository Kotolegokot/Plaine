#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht.h>
#include "util.h"
#include "GUI.h"

using namespace irr;

class EventReceiver : public IEventReceiver
{
public:
    EventReceiver();
    virtual bool OnEvent(const SEvent &event);
    virtual bool IsKeyDown(EKEY_CODE keyCode) const;

    bool quit = false, start = false, changingControlUp = false, changingControlDown = false, changingControlLeft = false,
    changingControlRight = false, defaultControls = false, toggleGUI = false, toggleFullscreen = false, toggleGraphicMode = false, toggleLanguage = false,
    resume = false, escapePressed = false;
    Stage stage = MENU;
    EKEY_CODE lastKey = KEY_KEY_CODES_COUNT;
private:
    bool PressedKeys[KEY_KEY_CODES_COUNT];
};

#endif // EVENTRECEIVER_H
