/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#include "EventReceiver.h"

using namespace irr;

EventReceiver::EventReceiver()
{
    pressedKeys.fill(false);
    checkedKeys.fill(false);
    guiEvents.fill(false);
}

bool EventReceiver::OnEvent(const SEvent &event)
{
    // if the event is related to keys
    if (event.EventType == EET_KEY_INPUT_EVENT) {
        pressedKeys[event.KeyInput.Key] = event.KeyInput.PressedDown;
        if (!event.KeyInput.PressedDown)
            checkedKeys[event.KeyInput.Key] = false;

        if (catchingKey)
            lastKey = event.KeyInput.Key;

        return pressedKeys[KEY_ESCAPE];
    }

    // if the event is related to GUI
    if (event.EventType == EET_GUI_EVENT) {
        s32 id = event.GUIEvent.Caller->getID();

        switch (event.GUIEvent.EventType) {
        case gui::EGET_BUTTON_CLICKED:
        case gui::EGET_COMBO_BOX_CHANGED:
        case gui::EGET_CHECKBOX_CHANGED:
        case gui::EGET_SPINBOX_CHANGED:
            guiEvents[id] = true;
            break;
        default:
            {
                break;
            }
        }
    }
    return false;
}

// returns true if keyCode is pressed
bool EventReceiver::isKeyDown(EKEY_CODE keyCode) const
{
    return pressedKeys[keyCode];
}

EKEY_CODE EventReceiver::getLastKey() const
{
    return lastKey;
}

bool EventReceiver::lastKeyAvailable() const
{
    return lastKey != KEY_KEY_CODES_COUNT;
}

bool EventReceiver::checkEvent(GUI_ID id)
{
    bool result = guiEvents[id];
    guiEvents[id] = false;

    return result;
}

bool EventReceiver::checkKeyPressed(EKEY_CODE keyCode)
{
    bool result = pressedKeys[keyCode] && !checkedKeys[keyCode];
    checkedKeys[keyCode] = pressedKeys[keyCode];

    return result;
}

void EventReceiver::startCatchingKey()
{
    catchingKey = true;
}

void EventReceiver::stopCatchingKey()
{
    catchingKey = false;
    lastKey = KEY_KEY_CODES_COUNT;
}
