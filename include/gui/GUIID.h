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

#ifndef GUIID_H
#define GUIID_H

enum GUI_ID {
    ID_BUTTON_QUIT,
    ID_BUTTON_START,
    ID_BUTTON_SCOREBOARD,
    ID_BUTTON_SETTINGS,
    ID_BUTTON_MENU,
    ID_BUTTON_TOGGLE_FULLSCREEN,
    ID_BUTTON_RESUME,
    ID_BUTTON_CONTROL_SETTINGS,
    ID_BUTTON_CONTROL_UP,
    ID_BUTTON_CONTROL_LEFT,
    ID_BUTTON_CONTROL_DOWN,
    ID_BUTTON_CONTROL_RIGHT,
    ID_BUTTON_CONTROL_CW_ROLL,
    ID_BUTTON_CONTROL_CCW_ROLL,
    ID_BUTTON_DEFAULT_CONTROLS,
    ID_COMBOBOX_RESOLUTION,
    ID_COMBOBOX_LANGUAGE,
    ID_CHECKBOX_VSYNC,
    ID_CHECKBOX_STENCILBUFFER,
    ID_SPINBOX_RENDER_DISTANCE,

    GUI_IDS_COUNT};

#endif // GUIID_H
