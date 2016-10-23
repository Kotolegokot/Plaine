#ifndef GUISCREEN_H
#define GUISCREEN_H

#include <string>
#include <irrlicht.h>

using namespace irr;

class GUIScreen
{
public:
    GUIScreen();
    virtual ~GUIScreen();

    virtual unsigned getIndex() const = 0;
    virtual void initialize(s32 buttonWidth, s32 buttonHeight) = 0;
    virtual void reload(s32 buttonWidth, s32 buttonHeight) = 0;
    virtual void terminate() = 0;
    virtual void resize(s32 buttonWidth, s32 buttonHeight) = 0;

protected:

private:
};

#endif // GUISCREEN_H
