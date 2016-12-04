 
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

#include "screens/ScoreboardScreen.h"

ScoreboardScreen::ScoreboardScreen(const ConfigData &configuration, gui::IGUIEnvironment &guiEnvironment) :
    IGUIScreen(configuration, guiEnvironment) {}

ScoreboardScreen::~ScoreboardScreen()
{
    terminate();
}

void ScoreboardScreen::initialize(s32 buttonWidth, s32 buttonHeight)
{
    std::vector<s32> dataScore = Scoreboard::loadScore("score.txt");
    int n;
    (dataScore.size() < 30) ? (n = dataScore.size()) : (n = 30);
    std::string str;
    for (int i = 0; i < n; i++)
    {
       str = "#" + std::to_string(i + 1) + " : " + std::to_string(dataScore[i]);
       score.push_back(guiEnvironment.addStaticText(utf8_to_wide(str).c_str(), core::rect<s32>(10, 10 * i, 200, 10 * i + 20)));
    }
    
    buttonMenu = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonMenu->setID(ID_BUTTON_MENU);
    setCustomButtonSkin(*buttonMenu);

    buttonQuit = guiEnvironment.addButton(core::rect<s32>(0, 0, 0, 0));
    buttonQuit->setID(ID_BUTTON_QUIT);
    setCustomButtonSkin(*buttonQuit);

    reload(buttonWidth, buttonHeight);
    resize(buttonWidth, buttonHeight);
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
    for (int i = 0; i < score.size(); i++)
        score[i]->remove();
    buttonMenu->remove();
    buttonQuit->remove();
}

void ScoreboardScreen::resize(s32 buttonWidth, s32 buttonHeight)
{
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
    for (int i = 0; i < score.size(); i++)
        score[i]->setVisible(visible);
    buttonMenu->setVisible(visible);
    buttonQuit->setVisible(visible);
}
