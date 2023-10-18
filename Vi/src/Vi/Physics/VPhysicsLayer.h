
#pragma once
#include "Vi/Core/Layer.h"
#include "Vi/Events/ApplicationEvent.h"
#include "Vi/Events/KeyEvent.h"
#include "Vi/Events/MouseEvent.h"


#include "VPhysicsTypes.h"

namespace Vi {
	namespace Physics {

		class VPhysicsLayer : public Layer
		{
		public:

			VPhysicsLayer();
			~VPhysicsLayer() = default;

			virtual void OnAttach() override;
			virtual void OnDetach() override;

			virtual void OnFixedUpdate(Timestep ts)override;

			void BeginPhysics(Timestep ts);
			void EndPhysics();


		};
	}
}