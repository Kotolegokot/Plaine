find include src . -maxdepth 1 | grep -P '.*\.(cpp|h)' | grep  -v 'CGUITTFont' | xargs wc -l
