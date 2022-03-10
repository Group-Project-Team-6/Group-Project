#include "Bullet.h"

Bullet::Bullet() : framesLeft(0) {

	InitAssets(); //Temp, Replace with loadAsset Class
	//bulletShape = new btSphereShape(0.2);
	bulletMass = 4;
	bulletInertia = { 1, 1, 1 };
	transform.SetPosition({ 0, 5, 0 });
	transform.SetOrientation({ 1, 1, 1, 1 });
	transform.SetScale({ .2, .2, .2 });
	this->SetRenderObject(new RenderObject(&transform, bulletMesh, bulletTex, bulletShader));
	transformConverter.BTNCLConvert(transform, bttransform);
	bulletMotion = new btDefaultMotionState(bttransform);
	btRigidBody::btRigidBodyConstructionInfo playerCI(bulletMass, bulletMotion, bulletShape, bulletInertia);
	bulletRigidBody = new btRigidBody(playerCI);
	//this->setActive(0);
	std::cout << transform.GetPosition().y << std::endl;
	//bulletRigidBody->setActivationState(true);
	
	//this->setActive(true);
	//needs adding to world

};

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

void Bullet::Init(Transform& startingTransform, btVector3 force, int lifeTime, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) {
	//Pos from parameter
	world.AddGameObject(this);
	framesLeft = lifeTime; 
	//transform = startingTransform;
	//transform.SetPosition({ startingTransform.GetPosition().x, startingTransform.GetPosition().y, startingTransform.GetPosition().z +20000 });
	this->GetTransform().SetPosition({ 20, 20, 0 });
	renderObject->GetTransform()->SetPosition({ 20, 20, 0 });
	//transform.SetOrientation({1, 1, 1, 1});
	//transform.SetScale({ .2, .2, .2 });
	transformConverter.BTNCLConvert(transform, bttransform);

	//this->setActive(true);
	
	//bulletRigidBody->setActivationState(true);
	//bulletRigidBody->applyCentralImpulse({0, 0, 1000});
	//this->setActive(1);
	
	std::cout << transform.GetPosition().y << std::endl;
	std::cout << this->GetTransform().GetPosition().y << std::endl;
	std::cout << GetRenderObject()->GetTransform()->GetPosition().y << std::endl;


	//physicsWorld.addRigidBody(bulletRigidBody);

	//inUse(); //Eventually causes Exception

	//std::cout << &world << std::endl;
	//std::cout << &physicsWorld << std::endl;
	
}

bool Bullet::Animate() {
	//if (!inUse()) return false;

	/*framesLeft--;
	if (framesLeft == 0) { //make while statement
		RemoveFromPool();
	}*/

	return framesLeft;
}

void Bullet::RemoveFromPool() {
	//bulletRigidBody->clearForces();
	//this->setActive(0);
	//bulletRigidBody->setActivationState(0);
	//set collision flags
	
	//!inUse();
}