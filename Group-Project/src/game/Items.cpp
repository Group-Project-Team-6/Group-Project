#include "Items.h"

Item::Item(Vector3 position, int score) {
	InitAssets(); //Temp, Replace with loadAsset Class

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

void Item::InitAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("Cube.msh", &itemMesh);

	itemTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	itemShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");
}

void Item::OnPlayerCollide() {
	//Free list pattern
}