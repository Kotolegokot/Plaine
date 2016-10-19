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

#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <functional>
#include <irrlicht.h>
#include "util.h"
#include "GUI.h"

using namespace irr;

// this class receives different kinds of event
//      like keys pressed or buttons clicked
class EventReceiver : public IEventReceiver
{
public:
    EventReceiver(GUI &gui);
    virtual bool OnEvent(const SEvent &event) override;
    bool IsKeyDown(EKEY_CODE keyCode) const;

    bool quit = false;
    bool start = false;
    bool changingControlUp = false;
    bool changingControlDown = false;
    bool changingControlLeft = false;
    bool changingControlRight = false;
    bool changingControlCwRoll = false;
    bool changingControlCcwRoll = false;
    bool defaultControls = false;
    bool toggleGUI = false;
    bool toggleGraphicMode = false;
    bool toggleFullscreen = false;
    bool needRestartInMenu = false;
    bool toggleResolution = false;
    bool toggleLanguage = false;
    bool escapePressed = false;
    bool tabPressed = false;
    bool upPressed = false;
    bool leftPressed = false;
    bool downPressed = false;
    bool rightPressed = false;

    EKEY_CODE getLastKey() const;
    bool lastKeyAvailable() const;
    void clearLastKey();

    GUIState desiredState = MENU; // desired state of GUI in menu; GUI must change to match it ASAP

    static std::function<bool()> emptyFunction;
    static std::function<bool(s32)> emptyS32Function;
    static std::function<bool(bool)> emptyBoolFunction;
    static std::function<bool(f32)> emptyF32Function;

    std::function<bool()> quitClicked = emptyFunction;
    std::function<bool()> startClicked = emptyFunction;
    std::function<bool()> settingsClicked = emptyFunction;
    std::function<bool()> menuClicked = emptyFunction;
    std::function<bool()> toggleFullscreenClicked = emptyFunction;
    std::function<bool()> resumeClicked = emptyFunction;
    std::function<bool()> controlSettingsClicked = emptyFunction;
    std::function<bool()> controlUpClicked = emptyFunction;
    std::function<bool()> controlLeftClicked = emptyFunction;
    std::function<bool()> controlDownClicked = emptyFunction;
    std::function<bool()> controlRightClicked = emptyFunction;
    std::function<bool()> controlCWClicked = emptyFunction;
    std::function<bool()> controlCCWClicked = emptyFunction;
    std::function<bool()> defaultControlsClicked = emptyFunction;
    std::function<bool(s32)> resolutionChanged = emptyS32Function;
    std::function<bool(s32)> colordepthChanged = emptyS32Function;
    std::function<bool(s32)> languageChanged = emptyS32Function;
    std::function<bool(bool)> vSyncChanged = emptyBoolFunction;
    std::function<bool(bool)> stencilBufferChanged = emptyBoolFunction;
    std::function<bool(f32)> renderDistanceChanged = emptyF32Function;
private:
    GUI &gui;

    bool pressedKeys[KEY_KEY_CODES_COUNT]; // this array shows which keys are pressed and which are not
    EKEY_CODE lastKey = KEY_KEY_CODES_COUNT; // last pressed key (only works in Controls menu)
};

#endif // EVENTRECEIVER_H
