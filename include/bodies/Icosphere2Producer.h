#ifndef ICOSPHERE2_PRODUCER
#define ICOSPHERE2_PRODUCER

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btConvexPointCloudShape.h>
#include "IBodyProducer.h"
#include "ObjMesh.h"
#include "options.h"
#include "util/Vector3.h"
#include "util.h"

using namespace irr;

#define ICOSPHERE2_MODEL "media/models/icosphere2.obj"

class Icosphere2Producer : public IBodyProducer {
public:
    Icosphere2Producer(btScalar radius) :
        m_radius(radius) {}

    btScalar getMass() const override
    {
        static constexpr btScalar K = 4.0f / 3.0f * PI<btScalar>;

        return radius * radius * radius * K * MASS_COEFFICIENT;
    }

protected:
    std::unique_ptr<scene::ISceneNode> createNode(IrrlichtDevice &irrlichtDevice,
                                                  const btVectore &position) const override
    {
        std::unique_ptr<scene::IMesh> mesh(irrlichtDevice.getSceneManager()->getMesh(ICOSPHERE2_MODEL));

        std::unique_ptr<scene::ISceneNode> node(irrlichtDevice.getSceneManager()->
                                addMeshSceneNode(mesh.release(), 0, -1, position));
        node->setScale(core::vector3df(radius, radius, radius) * 2);
        node->setMaterialTexture(0, irrlichtDevice.getVideoDriver()->getTexture("media/textures/icosphere2.png"));
        node->setVisible(TEXTURES_ENABLED);
#if LOG_ENABLED
        node->setMaterialFlag(video::EMF_FOG_ENABLE, true);
#endif
        node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);
        node->setMaterialFlag(video::EMF_TRILINEAR_FILTER, true);
        node->setMaterialFlag(video::EMF_ANTI_ALIASING, true);

        return node;
    }

    std::unique_ptr<btCollisionShape> createShape() const override
    {
        return std::make_unique<btConvexPointCloudShape>(objMesh.getPoints(), objMesh.getPointsCount(),
                                                         btVector3(radius, radius, radius) * 2);
    }


private:
    const btScalar m_radius;

    static ObjMesh &objMesh;
};

#endif // ICOSPHERE2_PRODUCER
