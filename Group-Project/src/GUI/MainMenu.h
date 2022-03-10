#pragma once	
#include "GameMenu.h"

class PhysicsTestScene;

namespace NCL {
	namespace CSC8503 {
		class MainMenu : public GameMenu
		{
		public:
			MainMenu(PhysicsTestScene* inGame)
				: game(inGame)
			{};
			virtual ~MainMenu() {};

			virtual void Draw() override;
		protected:
			PhysicsTestScene* game;
		};

	}
}


