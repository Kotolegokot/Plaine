#!/bin/sh
find include src include/obstacles src/obstacles . -maxdepth 1 | grep -P '.*\.(cpp|h)' | grep  -v 'CGUITTFont' | xargs wc -l
