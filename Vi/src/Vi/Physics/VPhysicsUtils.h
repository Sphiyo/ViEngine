#pragma once

#include<Vi.h> 
#include <PxPhysicsAPI.h>


namespace Vi {
    namespace PhysicsUtils {
        using namespace physx;

        // Function to convert radians to degrees
        inline double RadiansToDegrees(double radians) {
            return radians * 180.0 / 3.14159265359;
        }

        // Function to calculate Euler angles from a quaternion
        inline void QuaternionToEulerAngles(const PxQuat& quaternion, double& roll, double& pitch, double& yaw) {
            double sinr_cosp = 2.0 * (quaternion.x * quaternion.w + quaternion.y * quaternion.z);
            double cosr_cosp = 1.0 - 2.0 * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
            roll = std::atan2(sinr_cosp, cosr_cosp);

            double sinp = 2.0 * (quaternion.w * quaternion.x - quaternion.y * quaternion.z);
            if (std::abs(sinp) >= 1)
                pitch = std::copysign(90.0, sinp); // Use 90 degrees if out of range
            else
                pitch = std::asin(sinp);

            double siny_cosp = 2.0 * (quaternion.x * quaternion.y + quaternion.z * quaternion.w);
            double cosy_cosp = 1.0 - 2.0 * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
            yaw = std::atan2(siny_cosp, cosy_cosp);

            // Convert Euler angles from radians to degrees
            roll = RadiansToDegrees(roll);
            pitch = RadiansToDegrees(pitch);
            yaw = RadiansToDegrees(yaw);
        }
    }
}