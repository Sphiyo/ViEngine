#include "Vipch.h"
#include "VPhysicsActor.h"

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

#include "VPhysicsScene.h"
#include "VPhysicsMaterial.h"



namespace Vi {
    namespace Physics {

        VPhysicsRigidActor* VPhysicsRigidActor::s_Instance = new VPhysicsRigidActor();

        std::vector < physx::PxRigidDynamic* >    VPhysicsRigidActor::m_RigidDynamic;
        std::vector < physx::PxRigidStatic*  >    VPhysicsRigidActor::m_RigidStatic;
        std::vector < physx::PxMaterial*	 >    VPhysicsRigidActor::m_MaterialDyn;
        std::vector < physx::PxMaterial*	 >    VPhysicsRigidActor::m_MaterialStat;

        void VPhysicsRigidActor::cleanup()
        {
            for (auto& rd : m_RigidDynamic)
                PX_RELEASE(rd);
            for (auto& rs : m_RigidStatic)
                PX_RELEASE(rs);

        }



        void VPhysicsRigidActor::CreateRigidDynamic(const physx::PxVec3& Position, const physx::PxVec3& Rotation, const physx::PxGeometry& geometry, const VPhysicsMaterialParams& MaterialParams) {


            m_MaterialDyn.push_back(VPhysicsWorld::GetPhysics()->createMaterial(MaterialParams.StaticFriction, MaterialParams.DynamicFriction, MaterialParams.Restitution));

            PxQuat rotation = PxIdentity;
            rotation = PxQuat(Rotation.x, PxVec3(1.0f, 0.0f, 0.0f)) *
                PxQuat(Rotation.y, PxVec3(0.0f, 1.0f, 0.0f)) *
                PxQuat(Rotation.z, PxVec3(0.0f, 0.0f, 1.0f));
            PxTransform Transform(Position, rotation);

            m_RigidDynamic.push_back(PxCreateDynamic(*VPhysicsWorld::GetPhysics(), Transform, geometry, *m_MaterialDyn.back(), 10.0f));
            m_RigidDynamic.back()->setMass(PxReal(1600.0f));

            VPhysicsScene::GetScene()->addActor(*m_RigidDynamic.back());

        }



        void VPhysicsRigidActor::CreateRigidStatic(const physx::PxVec3& Position, const physx::PxVec3& Rotation, const physx::PxGeometry& geometry, const VPhysicsMaterialParams& MaterialParams)
        {
            m_MaterialStat.push_back(VPhysicsWorld::GetPhysics()->createMaterial(MaterialParams.StaticFriction, MaterialParams.DynamicFriction, MaterialParams.Restitution));

            PxQuat rotation = PxIdentity;
            rotation = PxQuat(Rotation.x, PxVec3(1.0f, 0.0f, 0.0f)) *
                PxQuat(Rotation.y, PxVec3(0.0f, 1.0f, 0.0f)) *
                PxQuat(Rotation.z, PxVec3(0.0f, 0.0f, 1.0f));
            PxTransform Transform(Position, rotation);

            //     m_RigidStatic.push_back(PxCreatePlane(*VPhysicsWorld::GetPhysics(), PxPlane(0, 1, 0, 0), *m_MaterialStat.back()));
            m_RigidStatic.push_back(PxCreateStatic(*VPhysicsWorld::GetPhysics(), Transform, geometry, *m_MaterialStat.back()));
            VPhysicsScene::GetScene()->addActor(*m_RigidStatic.back());
        }
    };
}



