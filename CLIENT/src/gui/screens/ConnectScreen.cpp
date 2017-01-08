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

#include "gui/screens/ConnectScreen.hpp"

ConnectScreen::ConnectScreen(const ConfigData &configuration, gui::IGUIEnvironment &gui_env) :
    IGUIScreen(configuration, gui_env) {}

ConnectScreen::~ConnectScreen()
{
    terminate();
}

void ConnectScreen::initialize(s32 button_width, s32 button_height)
{
    text_nick = guiEnvironment.addStaticText(_wp(""), core::rect<s32>());

    editbox_nick = guiEnvironment.addEditBox(L"", core::rect<s32>());
    editbox_nick->setID(ID_EDITBOX_NICK);

    text_host = guiEnvironment.addStaticText(_wp(""), core::rect<s32>());

    editbox_host = guiEnvironment.addEditBox(L"127.0.0.1", core::rect<s32>());
    editbox_host->setID(ID_EDITBOX_HOST);

    text_port = guiEnvironment.addStaticText(_wp(""), core::rect<s32>());

    spinbox_port = guiEnvironment.addSpinBox(L"36363", core::rect<s32>());
    spinbox_port->setID(ID_SPINBOX_PORT);

    button_connect = guiEnvironment.addButton(core::rect<s32>());
    button_connect->setID(ID_BUTTON_CONNECT);
    setCustomButtonSkin(*button_connect);

    button_menu = guiEnvironment.addButton(core::rect<s32>());
    button_menu->setID(ID_BUTTON_MENU);
    setCustomButtonSkin(*button_menu);

    button_quit = guiEnvironment.addButton(core::rect<s32>());
    button_quit->setID(ID_BUTTON_QUIT);
    setCustomButtonSkin(*button_quit);

    reload(button_width, button_height);
    resize(button_width, button_height);

    initialized = true;
}

void ConnectScreen::reload(s32 /*button_width*/, s32 /*button_height*/)
{
    text_nick->setText(_wp("Nick:"));
    text_host->setText(_wp("Host:"));
    text_port->setText(_wp("Port:"));

    spinbox_port->setRange(0, std::numeric_limits<f32>::max());

    button_connect->setText(_wp("Connect"));
    button_connect->setToolTipText(_wp("Connect tot he host"));

    button_menu->setText(_wp("Back"));
    button_menu->setToolTipText(_wp("Exit to main menu"));

    button_quit->setText(_wp("Quit"));
    button_quit->setToolTipText(_wp("Exit game"));
}

void ConnectScreen::terminate()
{
    if (initialized) {
        text_nick->remove();
        text_host->remove();
        text_port->remove();
        editbox_nick->remove();
        editbox_host->remove();
        spinbox_port->remove();
        button_connect->remove();
        button_menu->remove();
        button_quit->remove();

        initialized = false;
    }
}

core::rect<s32> ConnectScreen::left_column(int n, s32 button_width, s32 button_height) const
{
    u32 width = configuration.resolution.Width;
    u32 height = configuration.resolution.Height;

    return core::rect<s32>(width - 3 * button_width / 2 - 2 * SPACE,
                           height - n * button_height - n * SPACE,
                           width - button_width - 2 * SPACE,
                           height - (n - 1) * button_height - n * SPACE);
}

core::rect<s32> ConnectScreen::right_column(int n, s32 button_width, s32 button_height) const
{
    u32 width = configuration.resolution.Width;
    u32 height = configuration.resolution.Height;

    return core::rect<s32>(width - button_width - 2 * SPACE,
                           height - n * button_height - n * SPACE,
                           width - 2 * SPACE,
                           height - (n - 1) * button_height - n * SPACE);
}

void ConnectScreen::resize(s32 button_width, s32 button_height)
{
    text_nick->setRelativePosition(left_column(6, button_width, button_height));
    editbox_nick->setRelativePosition(right_column(6, button_width, button_height));

    text_host->setRelativePosition(left_column(5, button_width, button_height));
    editbox_host->setRelativePosition(right_column(5, button_width, button_height));

    text_port->setRelativePosition(left_column(4, button_width, button_height));
    spinbox_port->setRelativePosition(right_column(4, button_width, button_height));

    button_connect->setRelativePosition(right_column(3, button_width, button_height));
    button_menu->setRelativePosition(right_column(2, button_width, button_height));
    button_quit->setRelativePosition(right_column(1, button_width, button_height));
}

std::vector<gui::IGUIElement *> ConnectScreen::getSelectableElements()
{
    return { editbox_nick, editbox_host, spinbox_port,
             button_connect, button_menu, button_quit };
}

void ConnectScreen::setVisible(bool visible)
{
    text_nick->setVisible(visible);
    text_host->setVisible(visible);
    text_port->setVisible(visible);
    editbox_nick->setVisible(visible);
    editbox_host->setVisible(visible);
    spinbox_port->setVisible(visible);
    button_connect->setVisible(visible);
    button_menu->setVisible(visible);
    button_quit->setVisible(visible);
}
