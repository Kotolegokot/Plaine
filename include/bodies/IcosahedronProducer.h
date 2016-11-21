#ifndef ICOSAHEDRON_PRODUCER
#define ICOSAHEDRON_PRODUCER

#include <cmath>
#include <memory>
#include <btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionShapes/btConvexPointCloudShape.h"
#include <irrlicht.h>
#include "IBodyProducer.h"
#include "ObjMesh.h"
#include "options.h"
#include "util/Vector3.h"
#include "util.h"

using namespace irr;

#define ICOSAHEDRON_MODEL "media/models/icosahedron.obj"

class IcosahedronProducer : public IBodyProducer {
public:
    IcosahedronProducer(btScalar edge) :
        m_edge(edge) {}

    btScalar getMass() const override
    {
        static constexpr btScalar K = 5.0f / 12.0f * (3.0f + std::sqrt(5.0f));

        return m_edge * m_edge * m_edge * K * MASS_COEFFICIENT;
    }

protected:
    std::unique_ptr<scene::ISceneNode> createNode(IrrlichtDevice &irrlichtDevice,
                                                  const core::vector3df &position) const override
    {
        std::unique_ptr<scene::IMesh> mesh(irrlichtDevice.getSceneManager()->getMesh(ICOSAHEDRON_MODEL));

        std::unique_ptr<scene::ISceneNode> node(irrlichtDevice.getSceneManager()->
                                addMeshSceneNode(mesh.release(), 0, -1, position));
        node->setScale(core::vector3df(edge, edge, edge));
        node->setMaterialTexture(0, irrlichtDevice.getVideoDriver()->getTexture("media/textures/icosahedron.png"));
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
                                                         { edge, edge, edge });
    }


private:
    btScalar m_edge;

    static ObjMesh &objMesh;
};

#endif // ICOSAHEDRON_PRODUCER
