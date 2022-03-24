#include "Bullet.h"
#include <math.h>
#include "Painter.h"

Bullet::Bullet(int team,  GameWorld& world, btDiscreteDynamicsWorld& dynamicsWorld) : framesLeft(0) {

	InitAssets();
	name = "Bullet";
	speed = 20;
	bulletTeam = team;
	transform.SetPosition({ 0, 25, 0 });
	transform.SetOrientation({ 1, 1, 1, 1 });
	transform.SetScale({ .2, .2, .2 });
	this->SetRenderObject(new RenderObject(&transform, bulletMesh.get(), bulletTex.get(), bulletShader.get()));
	transformConverter.BTNCLConvert(transform, bttransform);
	bulletMotion = new btDefaultMotionState(bttransform);
	bulletMass = 2;
	bulletInertia = { 0.2, 0.2, 0.2 };
	bulletFriction = 0.8;
	bulletRestitution = 0.8;
	bulletShape = new btSphereShape(0.2);
	btRigidBody::btRigidBodyConstructionInfo bulletCI(bulletMass, bulletMotion, bulletShape, bulletInertia);
	bulletRigidBody = new btRigidBody(bulletCI);
	bulletRigidBody->setFriction(bulletFriction);
	bulletRigidBody->setRestitution(bulletRestitution);
	bulletRigidBody->setUserPointer(this);

	//ghost = new btGhostObject();
	//ghost->setWorldTransform(bttransform);
	//ghost->setCollisionShape(bulletShape);
	//ghost->setUserPointer(this);
	//ghost->setCollisionFlags(ghost->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE | btCollisionObject::CF_STATIC_OBJECT);
	//isTrigger = true;
	//isStatic = false;

	world.AddGameObject(this);
	dynamicsWorld.addRigidBody(bulletRigidBody);
	//dynamicsWorld.addCollisionObject(ghost);

	this->setActive(false);
	bulletRigidBody->setActivationState(false);
};

Bullet::~Bullet() {
	delete bulletMotion;
	delete bulletShape;
	delete bulletRigidBody;
	//delete ghost;
}

void Bullet::InitAssets() {
	bulletMesh = AssetsManager::FetchMesh("SphereMesh");
	bulletTex = AssetsManager::FetchTexture("CheckerboardTex");
	bulletShader = AssetsManager::FetchShader("GameTechShaderSet");
}

void Bullet::Init(btRigidBody& player, btVector3 force, int lifeTime, Camera& camera, bool paintable) {

	this->setActive(1);
	bulletRigidBody->setActivationState(1);
	framesLeft = lifeTime;
	this->paintable = paintable;
	
	btScalar x, y, z;
	player.getWorldTransform().getRotation().getEulerZYX(z, y, x);
	float test = z > 0 ? (z - y) : y;

	bulletRigidBody->getWorldTransform().getOrigin().setX(-sin(test));
	bulletRigidBody->getWorldTransform().getOrigin().setY(0);
	bulletRigidBody->getWorldTransform().getOrigin().setZ(-cos(test));

	bulletRigidBody->getWorldTransform().setOrigin((bulletRigidBody->getWorldTransform().getOrigin()) + player.getWorldTransform().getOrigin());
	btQuaternion quat;
	quat.setEuler(test, 0, 0);
	bulletRigidBody->getWorldTransform().setRotation(quat);

	bulletRigidBody->applyCentralImpulse(bulletRigidBody->getWorldTransform().getBasis().getColumn(2) * -100);
}

void Bullet::Animate(btRigidBody& player, float dt) {
	if (!inUse()) return;
	//if(paintable) Painter::Paint(this, this->renderObject->GetTransform()->GetPosition());
	framesLeft -=dt;
	if (framesLeft <= 0.0f) {
		RemoveFromPool();
	}
}

void Bullet::RemoveFromPool() {
	this->setActive(0);
	bulletRigidBody->setActivationState(0);
	bulletRigidBody->getWorldTransform().setOrigin({ 0, -30, 0 });
}