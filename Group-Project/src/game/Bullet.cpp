#include "Bullet.h"

Bullet::~Bullet() {
	delete bulletMotion;
	delete bulletShape;
	delete bulletRigidBody;

	delete bulletMesh;
	delete bulletShader;
}

void Bullet::InitAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("sphere.msh", &bulletMesh);

	bulletTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	bulletShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");
}

void Bullet::Init(Transform startingTransform, int lifeTime) {
	//Pos from parameter
	framesLeft = lifeTime;
	InitAssets(); //Temp, Replace with loadAsset Class
	transform = startingTransform;
	transform.SetScale({ .2, .2, .2 });

	this->SetRenderObject(new RenderObject(&transform, bulletMesh, bulletTex, bulletShader));
	transformConverter.BTNCLConvert(transform, bttransform);

	bulletMotion = new btDefaultMotionState(bttransform);
	bulletShape = new btSphereShape(0.2);
	bulletMass = 5;
	bulletInertia = { 1, 1, 1 };
	bulletShape->calculateLocalInertia(bulletMass, bulletInertia);
	btRigidBody::btRigidBodyConstructionInfo playerCI(bulletMass, bulletMotion, bulletShape, bulletInertia);
	bulletRigidBody = new btRigidBody(playerCI);
	//Info from default constructor
}

bool Bullet::Animate() {
	if (!inUse()) return false;

	framesLeft--;

	return framesLeft;
}