#ifndef CONE_PRODUCER
#define CONE_PRODUCER

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btConvexPointCloudShape.h>
#include "IBodyProducer.h"
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

        return m_height * m_radius * m_radius * K * MASS_COEFFICIENT;
    }

protected:
    std::unique_ptr<scene::ISceneNode> createNode(IrrlichtDevice &irrlichtDevice,
                                                  const core::vector3df &position) const override
    {
        std::unique_ptr<scene::IMesh> mesh(irrlichtDevice.getSceneManager()->getMesh(CONE_MODEL));

        std::unique_ptr<scene::ISceneNode> node(irrlichtDevice.getSceneManager()->
                                    addMeshSceneNode(mesh.release(), 0, -1, position));
        node->setScale({ m_radius * 2, m_height, m_radius * 2 });
        node->setMaterialTexture(0,
                 irrlichtDevice.getVideoDriver()->getTexture("media/textures/cone.png"));
        node->setVisible(TEXTURES_ENABLED);
#ifdef FOG_ENABLED
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
                                                         btVector3(m_radius * 2, m_height, m_radius * 2));
    }


private:
    const btScalar m_radius;
    const btScalar m_height;

    static ObjMesh &objMesh;
};

#endif // CONE_PRODUCER
