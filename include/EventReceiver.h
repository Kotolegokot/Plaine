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
    virtual bool OnEvent(const SEvent &event) override;
    bool IsKeyDown(EKEY_CODE keyCode) const;

    bool quitClicked = false;
    bool startClicked = false;
    bool settingsClicked = false;
    bool menuClicked = false;
    bool toggleFullscreenClicked = false;
    bool resumeClicked = false;
    bool controlSettingsClicked = false;
    bool controlUpClicked = false;
    bool controlLeftClicked = false;
    bool controlDownClicked = false;
    bool controlRightClicked = false;
    bool controlCWClicked = false;
    bool controlCCWClicked = false;
    bool defaultControlsClicked = false;
    bool resolutionChanged = false;
    bool languageChanged = false;
    bool vSyncChanged = false;
    bool stencilBufferChanged = false;
    bool renderDistanceChanged = false;

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
private:
    bool pressedKeys[KEY_KEY_CODES_COUNT]; // this array shows which keys are pressed and which are not
    EKEY_CODE lastKey = KEY_KEY_CODES_COUNT; // last pressed key (only works in Controls menu)
};

#endif // EVENTRECEIVER_H
