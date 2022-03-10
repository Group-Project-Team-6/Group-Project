#include "Bullet.h"

bool Bullet::initialized = false;
MeshGeometry* Bullet::bulletMesh = nullptr;
TextureBase* Bullet::bulletTex = nullptr;
ShaderBase* Bullet::bulletShader = nullptr;

Bullet::~Bullet() {
	delete bulletMotion;
	delete bulletShape;
	delete bulletRigidBody;

	delete bulletMesh;
	delete bulletShader;
}

void Bullet::InitAssets(RendererBase& r) {
	bulletMesh = r.LoadMesh("Sphere.msh");
	bulletTex = TextureLoader::LoadAPITexture("checkerboard.png");
	bulletShader = r.LoadShader("GameTechShader.set");
}

void Bullet::Init(Transform startingTransform, int lifeTime, RendererBase& renderer) {
	//Pos from parameter
	if (!initialized) {
		InitAssets(renderer);
		initialized = true;
	}
	framesLeft = lifeTime; //Temp, Replace with loadAsset Class
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