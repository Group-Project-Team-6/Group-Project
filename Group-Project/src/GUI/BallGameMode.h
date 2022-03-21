#pragma once 
#include "GameMode.h"
#include "../GUI/BallLevelMenu.h"


namespace NCL {
	namespace CSC8503 {

		class BallGameMode : public GameMode
		{
		public:
			BallGameMode(PhysicsTestScene* game);
			virtual ~BallGameMode();

			virtual void SetupScene() override;
			virtual void Update(float dt) override;


		protected:
			std::shared_ptr<BallLevelMenu> menu = nullptr;
		};
	}
}