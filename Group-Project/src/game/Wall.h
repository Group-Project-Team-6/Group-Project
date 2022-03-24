#pragma once

#include "GameEntity.h"
#include "AssetsManager.h"
//#include "../CSC8503/GameTechRenderer.h"
//#include "../common/TextureLoader.h"

class Wall : public GameEntity {
public:
	Wall(Transform buildTransform);
	virtual ~Wall();

	void InitAssets(); //Temp

	/*btCollisionObject* getCollisionObject() {
		return wallObject;
	}*/
private:

	btDefaultMotionState* wallMotion = nullptr;
	btCollisionShape* wallShape = nullptr;
	btRigidBody* wallRigidBody = nullptr;

	//graphics
	MeshPtr wallMesh = nullptr;
	TexturePtr wallTex = nullptr;
	ShaderPtr wallShader = nullptr;

	TexID texID = -1;
};
