#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht/irrlicht.h>
#include "util.h"

using namespace irr;

class EventReceiver : public IEventReceiver
{
public:
    EventReceiver();
    virtual bool OnEvent(const SEvent &event);
    virtual bool IsKeyDown(EKEY_CODE keyCode) const;

    bool quit = false;

private:
    bool PressedKeys[KEY_KEY_CODES_COUNT];
    bool UpKeys[KEY_KEY_CODES_COUNT];
};

#endif // EVENTRECEIVER_H
