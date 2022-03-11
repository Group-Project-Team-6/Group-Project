#pragma once
#include "../Physics/PhysicsTestScene.h"

namespace NCL {
	namespace CSC8503 {
		class GameMode
		{
		public:
			GameMode(PhysicsTestScene* game)
				: gameInst(game)
			{};
			virtual ~GameMode() {};

			virtual void SetupScene() = 0;
			virtual void Update(float dt) = 0;
			
		protected:
			PhysicsTestScene* gameInst;
		};
	}
}