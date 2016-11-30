/* This file is part of PlaneRunner.
 *
 * PlaneRunner is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PlaneRunner is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PlaneRunner. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ICOSAHEDRON_PRODUCER
#define ICOSAHEDRON_PRODUCER

#include <cmath>
#include <memory>
#include <btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionShapes/btConvexPointCloudShape.h"
#include <irrlicht.h>
#include "interfaces/IBodyProducer.h"
#include "ObjMesh.h"
#include "util/Vector3.h"
#include "util/constants.h"
#include "util/options.h"

using namespace irr;

#define ICOSAHEDRON_MODEL "media/models/icosahedron.obj"

class IcosahedronProducer : public IBodyProducer {
public:
    IcosahedronProducer(btScalar edge) :
        m_edge(edge) {}

    btScalar getMass() const override
    {
        static btScalar K = 5.0f / 12.0f * (3.0f + std::sqrt(5.0f));

        return m_edge * m_edge * m_edge * K * MASS_COEFFICIENT;
    }

protected:
    std::unique_ptr<scene::ISceneNode> createNode(IrrlichtDevice &irrlichtDevice,
                                                  const btTransform &absoluteTransform) const override
    {
        std::unique_ptr<scene::IMesh> mesh(irrlichtDevice.getSceneManager()->getMesh(ICOSAHEDRON_MODEL));

        std::unique_ptr<scene::ISceneNode> node(irrlichtDevice.getSceneManager()->
                                addMeshSceneNode(mesh.release()));
        node->setPosition(bullet2irrlicht(absoluteTransform.getOrigin()));
        node->setRotation(quatToEulerDeg(absoluteTransform.getRotation()));
        node->setScale({ m_edge, m_edge, m_edge });
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
                                                         btVector3(m_edge, m_edge, m_edge));
    }


private:
    const btScalar m_edge;

    static ObjMesh &objMesh;
};

#endif // ICOSAHEDRON_PRODUCER
