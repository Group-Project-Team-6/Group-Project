#include "Bullet.h"
#include <math.h>
#include "Painter.h"

Bullet::Bullet(GameWorld& world, btDiscreteDynamicsWorld& dynamicsWorld) : framesLeft(0) {

	InitAssets(); //Temp, Replace with loadAsset Class
	name = "Bullet";
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

void Bullet::Init(btRigidBody& player, btVector3 force, int lifeTime, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld, bool paintable) {

	this->setActive(1);
	bulletRigidBody->setActivationState(1);
	framesLeft = lifeTime; 
	this->paintable = paintable;
	//float yaw = player.getWorldTransform().getRotation().getY();

	//player.getWorldTransform().getOrigin();
	//player.getWorldTransform().getRotation().getEulerZYX(player.getWorldTransform().getOrigin().x(), player.getWorldTransform().getOrigin().y(), player.getWorldTransform().getOrigin().z());
	btScalar x, y, z;
	player.getWorldTransform().getRotation().getEulerZYX(z,y,x);
	float test = z > 0 ? (z- y) : y;
	
	bulletRigidBody->getWorldTransform().getOrigin().setX(-sin(test));
	bulletRigidBody->getWorldTransform().getOrigin().setY(0);
	bulletRigidBody->getWorldTransform().getOrigin().setZ(-cos(test));

	bulletRigidBody->getWorldTransform().setOrigin((bulletRigidBody->getWorldTransform().getOrigin()) + player.getWorldTransform().getOrigin());
	btQuaternion quat;
	quat.setEuler(test, 0, 0);
	bulletRigidBody->getWorldTransform().setRotation(quat);
	/*position = { playersPosition.x + 10 * sin(Maths::DegreesToRadians(yaw)),
		playersPosition.y + 5,
		playersPosition.z + 10 * cos(Maths::DegreesToRadians(yaw)) };*/
	//bulletRigidBody->getWorldTransform().setBasis(x, y, z);

	bulletRigidBody->applyCentralImpulse(bulletRigidBody->getWorldTransform().getBasis().getColumn(2) * -100);
}

void Bullet::Animate(float dt) {
	if (!inUse()) return;
	if(paintable) Painter::Paint(this, this->GetTransform().GetPosition());
	framesLeft -=dt;
	if (framesLeft <= 0.0f) { //make while statement
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