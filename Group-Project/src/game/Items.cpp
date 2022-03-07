#include "Items.h"

Item::Item(Vector3 position, int score, RendererBase& r) {
	InitAssets(r); //Temp, Replace with loadAsset Class

	transform
		.SetPosition(position)
		.SetScale({ 1, 1, 1 })
		.SetOrientation({ 0, 0, 0, 1 });

	this->SetRenderObject(new RenderObject(&transform, itemMesh, itemTex, itemShader));
	transformConverter.BTNCLConvert(transform, bttransform);

	itemScore = score;
	itemMotion = new btDefaultMotionState(bttransform);
	itemShape = new btCapsuleShape(0.5, 1);
	btRigidBody::btRigidBodyConstructionInfo itemCI(0, itemMotion, itemShape, {0,0,0});
	itemRigidBody = new btRigidBody(itemCI);
}

Item::~Item() {
	delete itemMotion;
	delete itemShape;
	delete itemRigidBody;
	
	delete itemMesh;
	delete itemShader;
}

void Item::InitAssets(RendererBase& r) {
	itemMesh = r.LoadMesh("Cube.msh");
	itemTex = TextureLoader::LoadAPITexture("checkerboard.png");
	itemShader = r.LoadShader("GameTechShader.set");;
}

void Item::OnPlayerCollide() {
	//Free list pattern
}