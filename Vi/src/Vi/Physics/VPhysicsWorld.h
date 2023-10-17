#pragma once
#include <PxPhysicsAPI.h>

#include "VPhysicsTypes.h"


namespace Vi {
    namespace Physics {

        class VPhysicsWorld {
        public:

            VPhysicsWorld() {}
            ~VPhysicsWorld() {}

            bool initialize();
            void cleanup();

            static VPhysicsWorld& Get() { return *s_Instance; }
            static physx::PxFoundation*& GetFoundation() { return s_Instance->m_Foundation; }
            static physx::PxPhysics*& GetPhysics() { return s_Instance->m_Physics; }
            static physx::PxDefaultCpuDispatcher*& GetDispatcher() { return s_Instance->m_Dispatcher; }
            static physx::PxPvd*& GetPvd() { return s_Instance->m_Pvd; }
           
           

        private:


            physx::PxFoundation* m_Foundation = NULL;
            physx::PxPhysics* m_Physics = NULL;
            physx::PxDefaultAllocator m_Allocator;
            physx::PxDefaultErrorCallback m_ErrorCallback;
            physx::PxDefaultCpuDispatcher* m_Dispatcher = NULL;
            physx::PxPvd* m_Pvd = NULL;

            static VPhysicsWorld* s_Instance;


        };
    }
}
