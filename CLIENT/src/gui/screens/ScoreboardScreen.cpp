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

#include "gui/screens/ScoreboardScreen.hpp"

ScoreboardScreen::ScoreboardScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    IGUIScreen(configuration, guiEnvironment) {}

ScoreboardScreen::~ScoreboardScreen()
{
    terminate();
}

void ScoreboardScreen::initialize(s32 buttonWidth, s32 buttonHeight)
{
    tableScore = guiEnvironment.addTable(core::rect<s32>(0, 0, 0, 0));
    tableScore->setID(ID_LISTBOX_SCORE);
    tableScore->setResizableColumns(false);

    tableScore->addColumn(_wp("Place"), 0);
    tableScore->addColumn(_wp("Score"), 1);

    std::vector<s32> dataScore = Scoreboard::loadScore("score.txt");

    std::string str;

    for (std::size_t i = 0; i < dataScore.size(); i++)
    {
       str = "#" + std::to_string(i + 1);
       tableScore->addRow(i);
       tableScore->setCellText(i, 0, utf8_to_wide(str).c_str());
       str = std::to_string(dataScore[i]);
       tableScore->setCellText(i, 1, utf8_to_wide(str).c_str());
    }

    buttonMenu = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonMenu->setID(ID_BUTTON_MENU);
    setCustomButtonSkin(*buttonMenu);

    buttonQuit = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonQuit->setID(ID_BUTTON_QUIT);
    setCustomButtonSkin(*buttonQuit);

    reload(buttonWidth, buttonHeight);
    resize(buttonWidth, buttonHeight);

    initialized = true;
}

void ScoreboardScreen::reload(s32 /*buttonWidth*/, s32 /*buttonHeight*/)
{
    buttonMenu->setText(_wp("Menu"));
    buttonMenu->setToolTipText(_wp("Exit to main menu"));

    buttonQuit->setText(_wp("Quit"));
    buttonQuit->setToolTipText(_wp("Exit game"));
}

void ScoreboardScreen::terminate()
{
    if (initialized) {
        tableScore->remove();
        buttonMenu->remove();
        buttonQuit->remove();

        initialized = false;
    }
}

void ScoreboardScreen::resize(s32 buttonWidth, s32 buttonHeight)
{
    tableScore->setRelativePosition(core::rect<s32>(2*SPACE, SPACE,
                                                      configuration.resolution.Width - 4*SPACE - buttonWidth,
                                                      configuration.resolution.Height - 2*SPACE));
    tableScore->setColumnWidth(0, 6*SPACE);
    tableScore->setColumnWidth(1, configuration.resolution.Width - 12*SPACE - buttonWidth - 1);
    buttonMenu->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * SPACE,
                                                    configuration.resolution.Height - 2 * buttonHeight - 2 * SPACE,
                                                    configuration.resolution.Width - 2 * SPACE,
                                                    configuration.resolution.Height - buttonHeight - 2 * SPACE));

    buttonQuit->setRelativePosition(core::rect<s32>(configuration.resolution.Width - buttonWidth - 2 * SPACE,
                                                    configuration.resolution.Height - buttonHeight - SPACE,
                                                    configuration.resolution.Width - 2 * SPACE,
                                                    configuration.resolution.Height - SPACE));

}

std::vector<gui::IGUIElement *> ScoreboardScreen::getSelectableElements()
{
    return {buttonMenu, buttonQuit};
}

void ScoreboardScreen::setVisible(bool visible)
{
    tableScore->setVisible(visible);
    buttonMenu->setVisible(visible);
    buttonQuit->setVisible(visible);
}
