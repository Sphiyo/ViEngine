#pragma once

#include <PxPhysicsAPI.h>

#include "VPhysicsTypes.h"
class VPhysicsWorld;

namespace Vi {
    namespace Physics {

        struct VPhysicsSceneDescription {
            VPhysicsSceneDescription(physx::PxVec3 gravity = physx::PxVec3(0.0f, -9.81f, 0.0f), physx::PxU32 numThreads = 4) {

                Gravity = gravity;
                Dispatcher = physx::PxDefaultCpuDispatcherCreate(numThreads);
            }
            void Release() {
                Dispatcher = nullptr;
            }

            physx::PxVec3 Gravity;
            physx::PxDefaultCpuDispatcher* Dispatcher = NULL;
        };

        class VPhysicsScene {
        public:
            VPhysicsScene() {}
            ~VPhysicsScene() {}

            bool InitScene(VPhysicsSceneDescription sceneDescription = VPhysicsSceneDescription());
            void cleanup();

            static VPhysicsScene& Get() { return *s_Instance; }
            static physx::PxScene* GetScene() { return s_Instance->m_Scene; }


        private:

            physx::PxScene* m_Scene = NULL;

            static VPhysicsScene* s_Instance;
        };
    }
}

