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

#include <array>
#include <irrlicht.h>
#include "util.h"
#include "gui/GUI.h"

using namespace irr;

// this class receives different kinds of event
//      like keys pressed or buttons clicked
class EventReceiver : public IEventReceiver
{
public:
    EventReceiver();
    virtual bool OnEvent(const SEvent &event) override;
    bool isKeyDown(EKEY_CODE keyCode) const;

    EKEY_CODE getLastKey() const;
    bool lastKeyAvailable() const;
    void startCatchingKey();
    void stopCatchingKey();

    bool checkEvent(GUI_ID id);
    bool checkKeyPressed(EKEY_CODE keyCode);
private:
    std::array<bool, KEY_KEY_CODES_COUNT> pressedKeys; // this array shows which keys are pressed and which are not
    std::array<bool, KEY_KEY_CODES_COUNT> checkedKeys;
    std::array<bool, GUI_IDS_COUNT> guiEvents; // this array shows if a button is clicked or a combobox is changed etc.
    EKEY_CODE lastKey = KEY_KEY_CODES_COUNT; // last pressed key (only works in Controls menu)

    bool catchingKey = false;
};

#endif // EVENTRECEIVER_H
