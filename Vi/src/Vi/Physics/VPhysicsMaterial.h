#pragma once

#include <PxPhysicsAPI.h>




namespace Vi {
    namespace Physics {
        using namespace physx;

        struct VPhysicsMaterialParams {
            VPhysicsMaterialParams(float staticFriction = 0.5f, float dynamicFriction = 0.5f, float restitution = 0.6f)
                : StaticFriction(staticFriction), DynamicFriction(dynamicFriction), Restitution(restitution)
            {
            }

            float StaticFriction;
            float DynamicFriction;
            float Restitution;
        };
       

    }
}