#pragma once
#include "GameMenu.h"
#include <string>
#include "../game/DebugMode.h"

namespace NCL {
	namespace CSC8503 {
		class PauseMenu : public GameMenu
		{
		public:
			PauseMenu(){};
			virtual ~PauseMenu() {};

			virtual void Draw() override;
		
			bool hasInit = false;
			bool mainLevel = false;
			bool menuClose = false;
			bool quitGame = false;
			bool settingLevel = false;
		};

		class SettingMenu : public GameMenu
		{
		public:
			SettingMenu() {};
			virtual ~SettingMenu() {};

			virtual void Draw() override;

			std::string debugStr = "";
			bool toggleDebbug = false;
			bool back = false;
		};
	}
}



