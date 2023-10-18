#include "Vipch.h"
#include "VPhysicsLayer.h"

#include "VPhysicsWorld.h"
#include "VPhysicsScene.h"
#include "VPhysicsActor.h"
#include "VPhysicsMaterial.h"


namespace Vi {
	namespace Physics {




		VPhysicsLayer::VPhysicsLayer()
		{

			VPhysicsWorld::Get().initialize();

		}

		void VPhysicsLayer::OnAttach()
		{
			 
			VPhysicsScene::Get().InitScene();
			
			VPhysicsRigidActor::Get().CreateRigidStatic(physx::PxVec3(0.0f, 0.0f, 0.0f), physx::PxVec3(0.0f, 0.0f, 0.0f), physx::PxBoxGeometry(50.0f, 0.1f, 50.0f)/*, Mat*/); // plane
			for (float i = 0; i < 2; i++) {
				
				//physx::PxBoxGeometry* geom = new physx::PxBoxGeometry(1.0f, 1.0f, 1.0f);
			
				//VPhysicsMaterialParams Mat = VPhysicsMaterialParams(0.5f, 0.5f, 0.6f);
			
				//VPhysicsRigidActor DynamicActor = VRigidDynamic();
				VPhysicsRigidActor::Get().CreateRigidDynamic(physx::PxVec3(i * 10.1f, 20.0f, 0.0f), physx::PxVec3(45.0f, 45.0f, 0.0f), physx::PxBoxGeometry(1.0f, 1.0f, 1.0f)/*, Mat*/);
				
				//VRigidStatic StaticActor = VRigidStatic();
				//VPhysicsRigidActor::Get().CreateRigidStatic(/* Mat*/); // plane
			
				//delete geom;
				//geom = NULL;
			}
		
		}

		void VPhysicsLayer::OnDetach()
		{
			 
			VPhysicsScene::Get().cleanup();
			VPhysicsWorld::Get().cleanup();


		}

		void VPhysicsLayer::OnFixedUpdate(Timestep ts)
		{
			//if (!VPhysicsScene::Get()->m_Scene) return;

			std::cout << "FPS : " << 1 / ts << std::endl;
			std::cout << "DeltaSec : " << ts << std::endl;




		}

		void VPhysicsLayer::BeginPhysics(Timestep ts)
		{

			VPhysicsScene::GetScene()->simulate(ts);
			VPhysicsScene::GetScene()->fetchResults(true);
		}

		void VPhysicsLayer::EndPhysics()
		{
		}
	}
}