#!/bin/sh

# create new .pot
xgettext -k_wp -k_w -k_ src/*.cpp include/*.h main.cpp -o planerunner.pot

# update every localization
msgmerge media/locale/ru/LC_MESSAGES/planerunner.po planerunner.pot -U
