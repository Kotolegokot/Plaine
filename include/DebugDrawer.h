#ifndef DEBUGDRAWER_H
#define DEBUGDRAWER_H

#include <iostream>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "Plane.h"

using namespace irr;

class DebugDrawer : public btIDebugDraw
{
public:
    DebugDrawer(IrrlichtDevice *deivce);

    virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;
    virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance,
        int lifeTime,const btVector3 &color) override;
    virtual void reportErrorWarning(const char *warningString) override;
    virtual void draw3dText(const btVector3 &location, const char *textString) override;
    virtual void setDebugMode(int debugMode) override;
    virtual int getDebugMode() const override;

private:
    IrrlichtDevice *device = nullptr;
    DebugDrawModes debugMode = DBG_NoDebug;
};

#endif // DEBUGDRAWER_H
