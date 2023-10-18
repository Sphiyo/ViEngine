#pragma once
#include "VPhysicsWorld.h"
#include "VPhysicsMaterial.h"
#include <vector>


namespace Vi {
	namespace Physics {




		class VPhysicsRigidActor {
		public:
			VPhysicsRigidActor() {}
			~VPhysicsRigidActor() {}

			void cleanup();

			void CreateRigidDynamic(const physx::PxVec3& Position, const physx::PxVec3& Rotation, const physx::PxGeometry& geometry, const VPhysicsMaterialParams& MaterialParams = VPhysicsMaterialParams());
			void CreateRigidStatic(const physx::PxVec3& Position, const physx::PxVec3& Rotation, const physx::PxGeometry& geometry, const VPhysicsMaterialParams& MaterialParams = VPhysicsMaterialParams());


			static std::vector < physx::PxRigidDynamic* >& GetRigidDynamic() { return s_Instance->m_RigidDynamic; }
			static std::vector < physx::PxRigidStatic*  >& GetRigidStatic() { return s_Instance->m_RigidStatic; }
			static std::vector < physx::PxMaterial*		>& GetMaterials() { return s_Instance->m_MaterialStat; }
			static VPhysicsRigidActor& Get() { return *s_Instance; }
		private:

			static std::vector < physx::PxRigidDynamic* > m_RigidDynamic;
			static std::vector < physx::PxRigidStatic*  > m_RigidStatic;
			static std::vector < physx::PxMaterial*		> m_MaterialDyn;
			static std::vector < physx::PxMaterial*		> m_MaterialStat;

			static VPhysicsRigidActor* s_Instance;
		};

	}
}
