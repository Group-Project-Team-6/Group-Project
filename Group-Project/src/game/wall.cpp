#include "Wall.h"

Wall::Wall(Transform buildTransform) {
	InitAssets(); //Temp, Replace with loadAsset Class

	transform = buildTransform;		

	this->SetRenderObject(new RenderObject(&transform, wallMesh, wallTex, wallShader));
	transformConverter.BTNCLConvert(transform, bttransform);

	wallMotion = new btDefaultMotionState(bttransform);
	wallShape = new btBoxShape({ 1,1,1 });
	btRigidBody::btRigidBodyConstructionInfo itemCI(0, wallMotion, wallShape, { 0,0,0 });
	wallRigidBody = new btRigidBody(itemCI);
}

Wall::~Wall() {

}

void Wall::InitAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("Cube.msh", &wallMesh);

	wallTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	wallShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");
}

