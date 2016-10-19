/* This file is part of PlaneRunner.
 *
 * PlaneRunner is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlaneRunner is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlaneRunner. If not, see <http://www.gnu.org/licenses/>.
 */

#include "EventReceiver.h"

using namespace irr;

EventReceiver::EventReceiver()
{
    for (u32 i = 0; i < KEY_KEY_CODES_COUNT; i++)
        pressedKeys[i] = false;
}

bool EventReceiver::OnEvent(const SEvent &event)
{
    // if the event is related to keys
    if (event.EventType == EET_KEY_INPUT_EVENT) {
        if (!(changingControlUp || changingControlDown || changingControlLeft ||
              changingControlRight || changingControlCwRoll || changingControlCcwRoll))
        {
            pressedKeys[event.KeyInput.Key] = event.KeyInput.PressedDown;
            if (pressedKeys[KEY_TAB])
                tabPressed = true;
            if (pressedKeys[KEY_UP])
                upPressed = true;
            if (pressedKeys[KEY_DOWN])
                downPressed = true;

            return escapePressed;
        } else
            lastKey = event.KeyInput.Key;
    }

    // if the event is related to GUI
    else if (event.EventType == EET_GUI_EVENT) {
        s32 id = event.GUIEvent.Caller->getID();

        switch (event.GUIEvent.EventType) {
        case gui::EGET_BUTTON_CLICKED:
            if (id == ID_BUTTON_START) {
                startClicked = true;
                desiredState = HUD;
                return true;
            }
            else if (id == ID_BUTTON_SETTINGS) {
                settingsClicked = true;
                desiredState = SETTINGS;
                toggleGUI = true;
                changingControlUp = false;
                changingControlDown = false;
                changingControlLeft = false;
                changingControlRight = false;
                changingControlCwRoll = false;
                changingControlCcwRoll = false;
                return true;
            }
            else if (id == ID_BUTTON_RESUME) {
                resumeClicked = true;
                desiredState = HUD;
                toggleGUI = true;
                return true;
            }
            else if (id == ID_BUTTON_QUIT) {
                quitClicked = true;
                return true;
            }
            else if (id == ID_BUTTON_MENU) {
                menuClicked = true;
                desiredState = MENU;
                toggleGUI = true;
                return true;
            }
            else if (id == ID_BUTTON_TOGGLE_FULLSCREEN) {
                toggleFullscreenClicked = true;
                toggleFullscreen = true;
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_SETTINGS) {
                controlSettingsClicked = true;
                desiredState = CONTROL_SETTINGS;
                toggleGUI = true;
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_UP) {
                controlUpClicked = true;
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlUp = true;
                }
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_DOWN) {
                controlDownClicked = true;
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlDown = true;
                }
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_LEFT) {
                controlLeftClicked = true;
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlLeft = true;
                }
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_RIGHT) {
                controlRightClicked = true;
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlRight = true;
                }
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_CW_ROLL) {
                controlCWClicked = true;
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlCwRoll = true;
                }
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_CCW_ROLL) {
                controlCCWClicked = true;
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlCcwRoll = true;
                }
                return true;
            }
            else if (id == ID_BUTTON_DEFUALT_CONTROLS) {
                defaultControlsClicked = true;
                defaultControls = true;
                return true;
            }
            break;
        case gui::EGET_COMBO_BOX_CHANGED:
            if (id == ID_COMBOBOX_LANGUAGE)
            {
                languageChanged = true;
                toggleLanguage = true;
                break;
            }
            else if (id == ID_COMBOBOX_RESOLUTION)
            {
                resolutionChanged = true;
                toggleResolution = true;
                return true;
            }
        case gui::EGET_CHECKBOX_CHANGED:
            if (id == ID_CHECKBOX_VSYNC) {
                vSyncChanged = true;
                toggleGraphicMode = true;
                return true;
            } else if (id == ID_CHECKBOX_STENCILBUFFER) {
                stencilBufferChanged = true;
                toggleGraphicMode = true;
                return true;
            }
        case gui::EGET_SPINBOX_CHANGED:
            if (id == ID_SPINBOX_RENDER_DISTANCE) {
                renderDistanceChanged = true;
                toggleGraphicMode = true;
                return true;
            }
        default:
            {
                break;
            }
        }
    }
    return false;
}

// returns true if keyCode is pressed
bool EventReceiver::IsKeyDown(EKEY_CODE keyCode) const
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

void EventReceiver::clearLastKey()
{
    lastKey = KEY_KEY_CODES_COUNT;
}
