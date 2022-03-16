#pragma once
#include "../common/Camera.h"
#include "../game/TransformConverter.h"
#include "../game/TransformConverter.h"
#include "../common/RendererBase.h"

#include <vector>
#include <functional>

class GameEntity;
namespace NCL {
		class Camera;
	namespace CSC8503 {
		
		typedef std::function<void(GameEntity*)> GameObjectFunc;
		typedef std::vector<GameEntity*>::const_iterator GameObjectIterator;

		class GameWorld	{
		public:
			GameWorld();
			~GameWorld();

			void Clear();
			void ClearAndErase();

			void AddGameObject(GameEntity* o);
			void RemoveGameObject(GameEntity* o, bool andDelete = false);

			void SetRenderer(RendererBase* renderer) {
				this->renderer = renderer;
			}

			RendererBase* GetRenderer() {
				return renderer;
			}

			Camera* GetMainCamera() const {
				return mainCamera;
			}

			vector<GameEntity*> GetGameObjects() const {
				return gameObjects;
			}

			void ShuffleConstraints(bool state) {
				shuffleConstraints = state;
			}

			void ShuffleObjects(bool state) {
				shuffleObjects = state;
			}

			virtual void UpdateWorld(float dt);

			void UpdatePositions();

			void OperateOnContents(GameObjectFunc f);

			void GetObjectIterators(
				GameObjectIterator& first,
				GameObjectIterator& last) const;

		protected:
			std::vector<GameEntity*> gameObjects;
			RendererBase* renderer;

			Camera* mainCamera;

			bool	shuffleConstraints;
			bool	shuffleObjects;
			int		worldIDCounter;

			TransformConverter transformConverter;
		};
	}
}

