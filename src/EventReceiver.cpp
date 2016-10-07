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
        if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
            pressedKeys[event.KeyInput.Key] = event.KeyInput.PressedDown;
            if (pressedKeys[KEY_TAB])
                tabPressed = true;
            return escapePressed;
        } else {
            lastKey = event.KeyInput.Key;
            }
    }

    // if the event is related to GUI
    else if (event.EventType == EET_GUI_EVENT) {
        s32 id = event.GUIEvent.Caller->getID();
        switch (event.GUIEvent.EventType) {
        case gui::EGET_BUTTON_CLICKED:
            if (id == ID_BUTTON_START) {
                state = HUD;
                return true;
            }
            else if (id == ID_BUTTON_SETTINGS) {
                state = SETTINGS;
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
                state = HUD;
                toggleGUI = true;
                return true;
            }
            else if (id == ID_BUTTON_QUIT) {
                quit = true;
                return true;
            }
            else if (id == ID_BUTTON_MENU) {
                state = MENU;
                toggleGUI = true;
                return true;
            }
            else if (id == ID_BUTTON_TOGGLE_FULLSCREEN) {
                toggleFullscreen = true;
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_SETTINGS) {
                state = CONTROL_SETTINGS;
                toggleGUI = true;
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_UP) {
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlUp = true;
                }
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_DOWN) {
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlDown = true;
                }
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_LEFT) {
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlLeft = true;
                }
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_RIGHT) {
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlRight = true;
                }
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_CW_ROLL) {
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlCwRoll = true;
                }
                return true;
            }
            else if (id == ID_BUTTON_CONTROL_CCW_ROLL) {
                if (!(changingControlUp || changingControlDown || changingControlLeft || changingControlRight || changingControlCwRoll || changingControlCcwRoll)) {
                    changingControlCcwRoll = true;
                }
                return true;
            }
            else if (id == ID_BUTTON_DEFUALT_CONTROLS){
                defaultControls = true;
                return true;
            }
            break;
        case gui::EGET_COMBO_BOX_CHANGED:
            if (id == ID_COMBOBOX_LANGUAGE)
            {
                toggleLanguage = true;
                break;
            }
            else if (id == ID_COMBOBOX_RESOLUTION)
            {
                toggleResolution = true;
                return true;
            }
        case gui::EGET_CHECKBOX_CHANGED:
            {
                toggleGraphicMode = true;
                return true;
            }
        case gui::EGET_SPINBOX_CHANGED:
            {
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
