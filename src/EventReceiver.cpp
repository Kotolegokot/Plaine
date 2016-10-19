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

std::function<bool()> EventReceiver::emptyFunction = []() -> bool { return false; };
std::function<bool(s32)> EventReceiver::emptyS32Function = [](s32) -> bool { return false; };
std::function<bool(bool)> EventReceiver::emptyBoolFunction = [](bool) -> bool { return false; };
std::function<bool(f32)> EventReceiver::emptyF32Function = [](f32) -> bool { return false; };

EventReceiver::EventReceiver(GUI &gui) :
    gui(gui)
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
                desiredState = HUD;
                return true;

                return startClicked();
            }
            else if (id == ID_BUTTON_SETTINGS) {
                desiredState = SETTINGS;
                toggleGUI = true;
                changingControlUp = false;
                changingControlDown = false;
                changingControlLeft = false;
                changingControlRight = false;
                changingControlCwRoll = false;
                changingControlCcwRoll = false;
                return true;

                return settingsClicked();
            }
            else if (id == ID_BUTTON_RESUME) {
                desiredState = HUD;
                toggleGUI = true;
                return true;

                return resumeClicked();
            }
            else if (id == ID_BUTTON_QUIT) {
                quit = true;
                return true;

                return quitClicked();
            }
            else if (id == ID_BUTTON_MENU) {
                desiredState = MENU;
                toggleGUI = true;
                return true;

                return menuClicked();;
            }
            else if (id == ID_BUTTON_TOGGLE_FULLSCREEN) {
                toggleFullscreen = true;
                return true;

                return toggleFullscreenClicked();
            }
            else if (id == ID_BUTTON_CONTROL_SETTINGS) {
                desiredState = CONTROL_SETTINGS;
                toggleGUI = true;
                return true;

                return controlSettingsClicked();
            }
            else if (id == ID_BUTTON_CONTROL_UP) {
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlUp = true;
                }
                return true;

                return controlUpClicked();
            }
            else if (id == ID_BUTTON_CONTROL_DOWN) {
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlDown = true;
                }
                return true;

                return controlDownClicked();
            }
            else if (id == ID_BUTTON_CONTROL_LEFT) {
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlLeft = true;
                }
                return true;

                return controlLeftClicked();
            }
            else if (id == ID_BUTTON_CONTROL_RIGHT) {
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlRight = true;
                }
                return true;

                return controlRightClicked();
            }
            else if (id == ID_BUTTON_CONTROL_CW_ROLL) {
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlCwRoll = true;
                }
                return true;

                return controlCWClicked();
            }
            else if (id == ID_BUTTON_CONTROL_CCW_ROLL) {
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlCcwRoll = true;
                }
                return true;

                return controlCCWClicked();
            }
            else if (id == ID_BUTTON_DEFUALT_CONTROLS){
                defaultControls = true;
                return true;

                return defaultControlsClicked();
            }
            break;
        case gui::EGET_COMBO_BOX_CHANGED:
            if (id == ID_COMBOBOX_LANGUAGE)
            {
                toggleLanguage = true;

                return languageChanged(gui.comboBoxLanguage->getSelected());
            }
            else if (id == ID_COMBOBOX_RESOLUTION)
            {
                toggleResolution = true;
                return true;

                return resolutionChanged(gui.comboBoxResolution->getSelected());
            }
        case gui::EGET_CHECKBOX_CHANGED:
            if (id == ID_CHECKBOX_VSYNC)
            {
                toggleGraphicMode = true;
                return true;

                return vSyncChanged(gui.checkBoxVSync->isChecked());
            } else if (id == ID_CHECKBOX_STENCILBUFFER)
            {
                toggleGraphicMode = true;
                return true;

                return stencilBufferChanged(gui.checkBoxStencilBuffer->isChecked());
            }
        case gui::EGET_SPINBOX_CHANGED:
            if (id == ID_SPINBOX_RENDER_DISTANCE)
            {
                toggleGraphicMode = true;
                return true;

                return renderDistanceChanged(gui.spinBoxRenderDistance->getValue());
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
