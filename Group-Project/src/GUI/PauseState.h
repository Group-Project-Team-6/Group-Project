#pragma once

#include "../common/Window.h"
#include "PushdownState.h"
#include "PauseMenu.h"
#include "../Physics/PhysicsTestScene.h"

namespace NCL {
	namespace CSC8503 {
		class InGameState : public PushdownState
		{
		public:
			InGameState(PhysicsTestScene* g)
				: game(g)
			{};
			~InGameState() {};

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;

		private:
			PhysicsTestScene* game;
			bool quit = false;
			friend class PauseState;
		};

		class PauseState : public PushdownState
		{
		public:
			PauseState(InGameState* gs)
				: gameState(gs)
			{
				pauseMenu.reset(new PauseMenu());
			};
			~PauseState() {};

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;

		private:
			InGameState* gameState;
			std::shared_ptr<PauseMenu> pauseMenu;
		};
	}
}