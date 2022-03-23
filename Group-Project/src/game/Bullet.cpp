#include "Bullet.h"
#include <math.h>
#include "Painter.h"

Bullet::Bullet(GameWorld& world, btDiscreteDynamicsWorld& dynamicsWorld) : framesLeft(0) {

	InitAssets();
	name = "Bullet";
	speed = 20;

	//bulletMass = 4;
	//bulletInertia = { 1, 1, 1 };
	transform.SetPosition({ 0, 25, 0 });
	transform.SetOrientation({ 1, 1, 1, 1 });
	transform.SetScale({ .2, .2, .2 });
	this->SetRenderObject(new RenderObject(&transform, bulletMesh.get(), bulletTex.get(), bulletShader.get()));
	transformConverter.BTNCLConvert(transform, bttransform);
	bulletShape = new btSphereShape(0.2);

	ghost = new btGhostObject();
	ghost->setWorldTransform(bttransform);
	ghost->setCollisionShape(bulletShape);
	ghost->setUserPointer(this);
	ghost->setCollisionFlags(ghost->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE | btCollisionObject::CF_STATIC_OBJECT);
	isTrigger = true;
	isStatic = false;
	world.AddGameObject(this);
	dynamicsWorld.addCollisionObject(ghost);

	this->setActive(false);
	ghost->setActivationState(false);
};

Bullet::~Bullet() {
	delete bulletMotion;
	delete bulletShape;
	delete ghost;
}

void Bullet::InitAssets() {
	bulletMesh = AssetsManager::FetchMesh("SphereMesh");
	bulletTex = AssetsManager::FetchTexture("CheckerboardTex");
	bulletShader = AssetsManager::FetchShader("GameTechShaderSet");
}

void Bullet::Init(btRigidBody& player, btVector3 force, int lifeTime, Camera& camera, bool paintable) {

	this->setActive(1);
	ghost->setActivationState(1);
	framesLeft = lifeTime; 

	this->paintable = paintable;

	btScalar x, y, z;
	player.getWorldTransform().getRotation().getEulerZYX(z,y,x);
	float angle = z > 0 ? (z- y) : y;
	
	ghost->getWorldTransform().getOrigin().setX(-sin(angle));
	ghost->getWorldTransform().getOrigin().setY(0);
	ghost->getWorldTransform().getOrigin().setZ(-cos(angle));

	ghost->getWorldTransform().setOrigin((ghost->getWorldTransform().getOrigin()) + player.getWorldTransform().getOrigin());
	btQuaternion quat;
	quat.setEuler(angle, 0, 0);
	ghost->getWorldTransform().setRotation(quat);
}

void Bullet::Animate(btRigidBody& player, float dt) {
	if (!inUse()) return;
	if(paintable) Painter::Paint(this, this->GetTransform().GetPosition());
	framesLeft -=dt;
	if (framesLeft <= 0.0f) {
		RemoveFromPool();
	}

	ghost->getWorldTransform().setOrigin(ghost->getWorldTransform().getOrigin() + ghost->getWorldTransform().getBasis().getColumn(2) * dt * -speed);
}

void Bullet::RemoveFromPool() {
	this->setActive(0);
	ghost->setActivationState(0);
	ghost->getWorldTransform().setOrigin({ 0, -30, 0 });
}