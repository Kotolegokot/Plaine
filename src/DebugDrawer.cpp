#include "DebugDrawer.h"

using namespace irr;

DebugDrawer::DebugDrawer(IrrlichtDevice *device, Plane *plane)
    : device(device), plane(plane) {}

void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
    btVector3 from_r = from;// - plane->getPosition();
    btVector3 to_r = to;// - plane->getPosition();

    device->getVideoDriver()->setTransform(video::ETS_WORLD, core::IdentityMatrix);
    device->getVideoDriver()->draw3DLine(
        core::vector3df(from_r.x(), from_r.y(), from_r.z()),
        core::vector3df(to_r.x(), to_r.y(), to_r.z()),
        video::SColor(255, 255, 255, 0));
}

void DebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance,
    int lifeTime, const btVector3 &color)
{

}

void DebugDrawer::reportErrorWarning(const char *warningString)
{

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
