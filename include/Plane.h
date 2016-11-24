#ifndef PLANE_H
#define PLANE_H

#include <memory>
#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include "MotionState.h"
#include "Body.h"

using namespace irr;

class Plane : public Body {
public:
    Plane(btDynamicsWorld &physicsWorld, std::unique_ptr<btRigidBody> rigidBody) :
        Body(physicsWorld, std::move(rigidBody)) {}

    void explode() { m_exploded = true; }
    bool exploded() const { return m_exploded; }

    void addScore(long score)
    {
        if (score > 0)
            m_score += score * positiveMultiplier;
        else
            m_score += score * negativeMultiplier;

        if (score < 0)
            score = 0;
    }

    long score() const { return m_score; }
    void nullifyScore() { m_score = 0; }

    int positiveMultiplier = 1;
    int negativeMultiplier = 1;
private:
    bool m_exploded = false;
    long m_score = 0;
};

#endif // PLANE_H
