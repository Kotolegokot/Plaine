/* This file is part of Plaine.
 *
 * Plaine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plaine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plaine. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONE_PRODUCER
#define CONE_PRODUCER

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btConvexPointCloudShape.h>
#include "interfaces/IBodyProducer.h"
#include "ObjMesh.h"
#include "util/Vector3.h"
#include "util/constants.h"
#include "util/options.h"

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
                                                  const btTransform &absoluteTransform) const override
    {
        std::unique_ptr<scene::IMesh> mesh(irrlichtDevice.getSceneManager()->getMesh(CONE_MODEL));

        std::unique_ptr<scene::ISceneNode> node(irrlichtDevice.getSceneManager()->
                                    addMeshSceneNode(mesh.release()));
        node->setPosition(bullet2irrlicht(absoluteTransform.getOrigin()));
        node->setRotation(quatToEulerDeg(absoluteTransform.getRotation()));
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
