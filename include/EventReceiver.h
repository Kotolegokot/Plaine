#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht/irrlicht.h>
#include "util.h"

using namespace irr;

class EventReceiver : public IEventReceiver
{
public:
    EventReceiver(const Context &context);

    virtual bool OnEvent(const SEvent &event);
    virtual bool IsKeyDown(EKEY_CODE keyCode) const;

private:
    bool PressedKeys[KEY_KEY_CODES_COUNT];
    bool UpKeys[KEY_KEY_CODES_COUNT];
    Context context;
};

#endif // EVENTRECEIVER_H
