#include "Bullet.h"

Bullet::Bullet(GameWorld& world, btDiscreteDynamicsWorld& dynamicsWorld) : framesLeft(0) {

	InitAssets(); //Temp, Replace with loadAsset Class
	
	bulletMass = 4;
	bulletInertia = { 1, 1, 1 };
	transform.SetPosition({ 0, 5, 0 });
	transform.SetOrientation({ 1, 1, 1, 1 });
	transform.SetScale({ .2, .2, .2 });
	this->SetRenderObject(new RenderObject(&transform, bulletMesh, bulletTex, bulletShader));
	transformConverter.BTNCLConvert(transform, bttransform);
	bulletMotion = new btDefaultMotionState(bttransform);
	bulletShape = new btSphereShape(0.2);
	btRigidBody::btRigidBodyConstructionInfo playerCI(bulletMass, bulletMotion, bulletShape, bulletInertia);
	bulletRigidBody = new btRigidBody(playerCI);
	world.AddGameObject(this);
	dynamicsWorld.addRigidBody(bulletRigidBody);

	this->setActive(false);
	bulletRigidBody->setActivationState(false);
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

void Bullet::Init(btRigidBody& player, btVector3 force, int lifeTime, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) {
	//Pos from parameter
	//world.AddGameObject(this);
	//physicsWorld.addRigidBody(bulletRigidBody);

	this->setActive(1);
	bulletRigidBody->setActivationState(1);

	framesLeft = lifeTime; 
	float x =player.getWorldTransform().getOrigin().x();
	float y = player.getWorldTransform().getOrigin().y();
	float z = player.getWorldTransform().getOrigin().z();

	//player.getWorldTransform().getBasis().getColumn(2);

	bulletRigidBody->getWorldTransform().setOrigin({ x, y, z });

	bulletRigidBody->applyCentralImpulse(bulletRigidBody->getWorldTransform().getBasis().getColumn(2) * -100);
	//player.GetRigidBody()->applyCentralImpulse(player.GetRigidBody()->getWorldTransform().getBasis().getColumn(0) * -1000);
}

void Bullet::Animate() {
	if (!inUse()) return;

	framesLeft--;
	if (framesLeft == 0) { //make while statement
		RemoveFromPool();
	}

	return;
}

void Bullet::RemoveFromPool() {
	bulletRigidBody->clearForces();
	this->setActive(0);
	bulletRigidBody->setActivationState(0);
	//set collision flags

}