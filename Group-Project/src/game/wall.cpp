#include "Wall.h"

Wall::Wall(Transform buildTransform, RendererBase& r) {
	InitAssets(r); //Temp, Replace with loadAsset Class

	transform = buildTransform;		

	this->SetRenderObject(new RenderObject(&transform, wallMesh, wallTex, wallShader));
	transformConverter.BTNCLConvert(transform, bttransform);

	wallMotion = new btDefaultMotionState(bttransform);
	wallShape = new btCapsuleShape(0.5, 1);
	btRigidBody::btRigidBodyConstructionInfo itemCI(0, wallMotion, wallShape, { 0,0,0 });
	wallRigidBody = new btRigidBody(itemCI);
}

Wall::~Wall() {

}

void Wall::InitAssets(RendererBase& r) {

	wallMesh = r.LoadMesh("Cube.msh");
	wallTex = TextureLoader::LoadAPITexture("checkerboard.png");
	wallShader = r.LoadShader("GameTechShader.set");
}

