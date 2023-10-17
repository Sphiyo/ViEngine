#include "Vipch.h"
#include "VPhysicsScene.h" 


#include "VPhysicsWorld.h" 

namespace Vi {
    namespace Physics {

        VPhysicsScene* VPhysicsScene::s_Instance = new VPhysicsScene();
         
          
 
        //
        //physx::PxScene* InitScene() {
        //    if (Physics) {
        //        PxSceneDesc sceneDesc(Physics->getTolerancesScale());
        //        PxScene* scene = Physics->InitScene(sceneDesc);
        //        if (scene) {
        //            scenes.push_back(scene);
        //            return scene;
        //        }
        //    }
        //    return nullptr;
        //}
        // 
        // 
        // // Create a scene
        //physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
        //m_Scene = m_Physics->InitScene(sceneDesc);
        //if (!m_Scene) {
        //    std::cerr << "Error creating the PhysX scene." << std::endl;
        //    cleanup();
        //    return false;
        //}


        bool VPhysicsScene::InitScene(VPhysicsSceneDescription sceneDescription) {
            if (m_Scene) {
                return false; // Scene already exists
                std::cout << "Scene already exists \n";
            }
            // refs
            
            

            physx::PxSceneDesc sceneDesc(VPhysicsWorld::GetPhysics()->getTolerancesScale());
            sceneDesc.gravity = sceneDescription.Gravity;
            
            VPhysicsWorld::GetDispatcher() = sceneDescription.Dispatcher;
         //   VPhysicsWorld::GetDispatcher() = *sceneDescription.Dispatcher;
            sceneDesc.cpuDispatcher = VPhysicsWorld::GetDispatcher();
            sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

            sceneDescription.Release();

            m_Scene = VPhysicsWorld::GetPhysics()->createScene(sceneDesc);
          

            physx::PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
            if (pvdClient)
            {
                pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
                pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
                pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
            }
            if (!m_Scene) {
                std::cerr << "Error creating the PhysX scene." << std::endl;
                return false;
            }
            std::cout << "Scene Created \n";
            return true;
        }

        void VPhysicsScene::cleanup() {
            if (m_Scene) {
                PX_RELEASE(m_Scene);
                m_Scene = nullptr;
            }
         /*   auto Dispach = VPhysicsWorld::GetDispatcher();
            if (&VPhysicsWorld::GetDispatcher()) {
                PX_RELEASE(VPhysicsWorld::GetDispatcher());
                &VPhysicsWorld::GetDispatcher() = nullptr;
            }*/

        }



    }
}
