#include "EventReceiver.h"

using namespace irr;

EventReceiver::EventReceiver()
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i++)
        PressedKeys[i] = false;
}

bool EventReceiver::OnEvent(const SEvent &event)
{
    if (event.EventType == EET_KEY_INPUT_EVENT) {
        PressedKeys[event.KeyInput.Key] = event.KeyInput.PressedDown;
    } else if (event.EventType == EET_GUI_EVENT) {
        s32 id = event.GUIEvent.Caller->getID();

        switch (event.GUIEvent.EventType) {
        case gui::EGET_BUTTON_CLICKED:
            if (id == ID_BUTTON_QUIT) {
                quit = true;
                return true;
            }
            break;
        default:
            break;
        }
    }
    return false;
}

bool EventReceiver::IsKeyDown(EKEY_CODE keyCode) const
{
    return PressedKeys[keyCode];
}
