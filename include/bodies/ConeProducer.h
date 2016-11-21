#ifndef CONE_PRODUCER
#define CONE_PRODUCER

#include <memory>
#include <btBulletDynamicsCommon.h>
#include "IBodyProducer.h"
#include "BulletCollision/CollisionShapes/btConvexPointCloudShape.h"
#include "ObjMesh.h"
#include "options.h"
#include "util/Vector3.h"
#include "util.h"

using namespace irr;

#define CONE_MODEL "media/models/cone.obj"

class ConeProducer : public IBodyProducer {
public:
    ConeProducer(btScalar radius, btScalar height) :
        m_radius(radius), m_height(height) {}

    btScalar getMass() const override
    {
        static constexpr btScalar K = PI<btScalar> * 1.0f / 3.0f;

        return height * radius * radius * K * MASS_COEFFICIENT;
    }

protected:
    std::unique_ptr<scene::ISceneNode> createNode(IrrlichtDevice &irrlichtDeivce,
                                                  const core::vector3df &position) const override
    {
        std::unique_ptr<scene::IMesh> mesh(irrlichtDevice.getSceneManager()->getMesh(CONE_MODEL));

        std::unique_ptr<scene::ISceneNode> node(irrlichtDevice.getSceneManager()->
                                    addMeshSceneNode(mesh.release(), 0, -1, position));
        node->setScale({ radius * 2, height, radius * 2 });
        node->setMaterialTexture(0,
                 device.getVideoDriver()->getTexture("media/textures/cone.png"));
        node->setVisible(TEXTURES_ENABLED);
#ifdef FOG_ENABLED
        node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
#endif FOG_ENABLED
        node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
        node->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true);
        node->setMaterialFlag(video::EMF_ANTI_ALIASING, true);

        return node;
    }

    std::unique_ptr<btMotionState>
        createMotionState(std::unique_ptr<scene::ISceneNode> node,
                          btVector3 &position) const override
    {
        return std::make_unique<MotionState>(btTransform(btQuaternion(0, 0, 0, 1), position),
                                             node.release());
    }

    std::unique_ptr<btCollisionShape> createShape() const override
    {
        return std::make_unique<btConvexPointCloudShape>(objMesh.getPoints(), objMesh.getPointsCount(),
                                                         { radius * 2, height, radius * 2 });
    }


private:
    const btScalar radius;
    const btScalar height;

    static ObjMesh &objMesh;
};

#endif // CONE_PRODUCER
