#include "Bullet.h"
#include <math.h>

Bullet::Bullet(GameWorld& world, btDiscreteDynamicsWorld& dynamicsWorld) : framesLeft(0) {

	InitAssets(); //Temp, Replace with loadAsset Class
	
	bulletMass = 4;
	bulletInertia = { 1, 1, 1 };
	transform.SetPosition({ 0, 5, 0 });
	transform.SetOrientation({ 1, 1, 1, 1 });
	transform.SetScale({ .2, .2, .2 });
	this->SetRenderObject(new RenderObject(&transform, bulletMesh.get(), bulletTex.get(), bulletShader.get()));
	transformConverter.BTNCLConvert(transform, bttransform);
	bulletMotion = new btDefaultMotionState(bttransform);
	bulletShape = new btSphereShape(0.2);
	btRigidBody::btRigidBodyConstructionInfo playerCI(bulletMass, bulletMotion, bulletShape, bulletInertia);
	bulletRigidBody = new btRigidBody(playerCI);
	world.AddGameObject(this);
	dynamicsWorld.addRigidBody(bulletRigidBody);

	this->setActive(false);
	bulletRigidBody->setActivationState(false);
	//set collision flags
};

Bullet::~Bullet() {
	delete bulletMotion;
	delete bulletShape;
	delete bulletRigidBody;
}

void Bullet::InitAssets() {
	bulletMesh = AssetsManager::FetchMesh("SphereMesh");
	bulletTex = AssetsManager::FetchTexture("CheckerboardTex");
	bulletShader = AssetsManager::FetchShader("GameTechShaderSet");
}

void Bullet::Init(btRigidBody& player, btVector3 force, int lifeTime, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld) {

	this->setActive(1);
	bulletRigidBody->setActivationState(1);
	framesLeft = lifeTime; 

	btScalar x, y, z;
	player.getWorldTransform().getRotation().getEulerZYX(z,y,x);
	float angle = z > 0 ? (z- y) : y;
	
	bulletRigidBody->getWorldTransform().getOrigin().setX(-sin(angle));
	bulletRigidBody->getWorldTransform().getOrigin().setY(0);
	bulletRigidBody->getWorldTransform().getOrigin().setZ(-cos(angle));

	bulletRigidBody->getWorldTransform().setOrigin((bulletRigidBody->getWorldTransform().getOrigin()) + player.getWorldTransform().getOrigin());
	btQuaternion quat;
	quat.setEuler(angle, 0, 0);
	bulletRigidBody->getWorldTransform().setRotation(quat);

	bulletRigidBody->applyCentralImpulse(bulletRigidBody->getWorldTransform().getBasis().getColumn(2) * -100);
}

void Bullet::Animate() {
	if (!inUse()) return;

	framesLeft--;
	if (framesLeft == 0) { //make while statement
		RemoveFromPool();
	}

	//Bullet tick to limit speed of shootiung

	return;
}

void Bullet::RemoveFromPool() {
	bulletRigidBody->clearForces();
	this->setActive(0);
	bulletRigidBody->setActivationState(0);
	//needs to be called on collision

}