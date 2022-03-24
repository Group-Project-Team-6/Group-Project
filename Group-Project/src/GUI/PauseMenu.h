#pragma once
#include "GameMenu.h"
#include <string>
#include "../DebugMode/DebugMode.h"

const int msgLimit = 100;
const int fpsLimit = 100;

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
			bool toggleDebug = false;
			bool back = false;
		};

		class GameHUD : public GameMenu
		{
		public:
			GameHUD() {};
			virtual ~GameHUD() {};
			
			virtual void Draw() override;

			void AddMessage(std::string s);
			void AddFPS(float s);

			int numPlayer = 4;
			int numTeam = 2;
			int team1 = 0;
			int team2 = 0;
			float hp[4] = { 0.0f,0.0f,0.0f,0.0f };
			float time = 0.0f;
			bool debug = false;
		private:
			int fpsPos = 0;
			int msgPos = 0;
			std::string msg[msgLimit];
			float fps[fpsLimit];
		};
	}
}



