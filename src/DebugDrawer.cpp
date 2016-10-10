#include "DebugDrawer.h"

#include <GL/gl.h>
#include <GL/glu.h>

using namespace irr;

DebugDrawer::DebugDrawer(IrrlichtDevice *device)
    : device(device) {}

void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
    device->getVideoDriver()->setTransform(video::ETS_WORLD, core::IdentityMatrix);
    device->getVideoDriver()->draw3DLine(
        core::vector3df(from.x(), from.y(), from.z()),
        core::vector3df(to.x(), to.y(), to.z()),
        video::SColor(255, 255, 255, 0));
}

void DebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance,
    int lifeTime, const btVector3 &color)
{

}

void DebugDrawer::reportErrorWarning(const char *warningString)
{
    std::cerr << "Error: " << warningString << std::endl;
}

void DebugDrawer::draw3dText(const btVector3 &location, const char *textString)
{

}

void DebugDrawer::setDebugMode(int debugMode)
{
    this->debugMode = (DebugDrawModes) debugMode;
}

int DebugDrawer::getDebugMode() const
{
    return debugMode;
}
