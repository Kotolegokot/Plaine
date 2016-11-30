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

#ifndef BOX_PRODUCER
#define BOX_PRODUCER

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <irrlicht.h>
#include "interfaces/IBodyProducer.h"
#include "options.h"
#include "util/Vector3.h"
#include "util/constants.h"

using namespace irr;

class BoxProducer : public IBodyProducer {
public:
    BoxProducer(const btVector3 &halfExtents) :
        m_halfExtents(halfExtents) {}

    btScalar getMass() const override
    {
        return 8*m_halfExtents.x()*m_halfExtents.y()*m_halfExtents.z()*MASS_COEFFICIENT;
    }

protected:
    std::unique_ptr<scene::ISceneNode> createNode(IrrlichtDevice &irrlichtDevice,
                                                  const btTransform &absoluteTransform) const override
    {
        std::unique_ptr<scene::ISceneNode> node(irrlichtDevice.getSceneManager()->
                                addCubeSceneNode(1));
        node->setPosition(bullet2irrlicht(absoluteTransform.getOrigin()));
        node->setRotation(quatToEulerDeg(absoluteTransform.getRotation()));
        node->setScale(bullet2irrlicht(m_halfExtents) * 2);
        node->setMaterialTexture(0,
                irrlichtDevice.getVideoDriver()->getTexture("media/textures/square.png"));
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
        // create shape for cubes
        return std::make_unique<btBoxShape>(m_halfExtents);
    }

private:
    btVector3 m_halfExtents;
};

#endif // BOX_PRODUCER
