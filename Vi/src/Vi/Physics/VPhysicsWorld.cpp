#include "Vipch.h"
#include "VPhysicsWorld.h" 

#include <PxPhysicsAPI.h>
#include <pvd/PxPvd.h>

//#include "../../snippets/snippetcommon/SnippetPVD.h"

#define PVD_HOST "127.0.0.1"

namespace Vi {
    namespace Physics {

    //  physx::PxFoundation* m_Foundation= NULL;
    //  physx::PxPhysics* VPhysicsWorld::m_Physics;
    //  physx::PxDefaultAllocator VPhysicsWorld::m_Allocator;
    //  physx::PxDefaultErrorCallback VPhysicsWorld::m_ErrorCallback;
    //  physx::PxDefaultCpuDispatcher* VPhysicsWorld::m_Dispatcher = NULL;
    //  physx::PxPvd* VPhysicsWorld::m_Pvd = NULL;

        VPhysicsWorld* VPhysicsWorld::s_Instance = new VPhysicsWorld();

      
 


        bool Physics::VPhysicsWorld::initialize() {

            m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);
            if (!m_Foundation) {
                std::cerr << "Error initializing the PhysX foundation." << std::endl;
                return false;
            }


            m_Pvd = physx::PxCreatePvd(*m_Foundation);
            if (!m_Pvd) {
                std::cerr << "Error initializing PhysX Visual Debugger (PVD)." << std::endl;
            }
            else {
                physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
                m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
            }
            ///////////////////////////////////////////////////////////
            m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), true, m_Pvd);
            if (!m_Physics) {
                std::cerr << "Error initializing the PhysX SDK." << std::endl;
                cleanup();
                return false;
            }

            return true;
        }
        // Dont forget relese scene before; and move dispatcher to scene
        void VPhysicsWorld::cleanup() {

              if (m_Dispatcher) {
                  m_Dispatcher->release();
                  m_Dispatcher = nullptr;
              }

            if (m_Physics) {
                PX_RELEASE(m_Physics);
                m_Physics = nullptr;
            }

            if (m_Pvd)
            {
                physx::PxPvdTransport* transport = m_Pvd->getTransport();
                m_Pvd->disconnect();
                PX_RELEASE(m_Pvd);
                m_Pvd = NULL;
                PX_RELEASE(transport);
                transport = NULL;
            }
            if (m_Foundation) {
                PX_RELEASE(m_Foundation);
                m_Foundation = nullptr;
            }
        }


    }
}

//int main() {
//    // Create an instance of the VPhysicsWorld class within the Physics namespace
//    Physics::VPhysicsWorld VPhysicsWorld;
//
//    // Initialize the physics engine
//    if (!VPhysicsWorld.initialize()) {
//        std::cerr << "Physics engine initialization failed." << std::endl;
//        return 1;
//    }
//
//    // Access the physics and scene objects
//    physx::PxPhysics* physics = VPhysicsWorld.getPhysics();
//    physx::PxScene* scene = VPhysicsWorld.getScene();
//
//    // Now you can use 'physics' and 'scene' to work with NVIDIA PhysX
//
//    // Cleanup resources when done
//    VPhysicsWorld.cleanup();
//
//    return 0;
//}
