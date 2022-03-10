#pragma once
#include "GameMode.h"
#include "MainBallObject.h"
#include "BallLevelMenu.h"

namespace NCL {
	namespace CSC8503 {
		class ShootableObject;
		class TriggerObject;
		class CheckPointObject;

		class BallGameMode : public GameMode
		{
		public:
			BallGameMode(TutorialGame* game);
			virtual ~BallGameMode();

			virtual void SetupScene() override;
			virtual void Update(float dt) override;


		protected:
			MainBallObject* mainBall = nullptr;
			std::shared_ptr<BallLevelMenu> menu = nullptr;
		};
	}
}
