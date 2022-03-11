#pragma once
#include "../common/Vector3.h"
#include "GameEntity.h"


class Bullet : public GameEntity{
public:
	Bullet() {
		bulletMesh = nullptr;
		bulletTex = nullptr;
		bulletShader = nullptr;
		bulletMotion = nullptr;
		bulletShape = nullptr;
		bulletRigidBody = nullptr;
	};
	Bullet(GameWorld& world, btDiscreteDynamicsWorld& dynamicsWorld);
	~Bullet();

	void Init(btRigidBody& player, btVector3 force, int lifeTime, GameWorld& world, btDiscreteDynamicsWorld& physicsWorld);
	void Animate();
	void InitAssets();
	void RemoveFromPool();

	bool inUse() const { return framesLeft > 0; }

	virtual btRigidBody* GetRigidBody() const override {
		return bulletRigidBody;
	}

	virtual void SetRigidBody(btRigidBody* newRigidBody) override {
		bulletRigidBody = newRigidBody;
	}

	virtual void UpdateRenderPositions() override {

		bttransform = bulletRigidBody->getWorldTransform();

		btRot = bttransform.getRotation();
		btPos = bttransform.getOrigin();

		nclRot = { btRot.getX(), btRot.getY(), btRot.getZ(), btRot.getW() };
		nclPos = { btPos.getX(), btPos.getY(), btPos.getZ() };

		transform.SetOrientation(nclRot);
		transform.SetPosition(nclPos);

	}

private:
	int framesLeft;

	OGLMesh* bulletMesh;
	OGLTexture* bulletTex;
	OGLShader* bulletShader;

	//Transform transform;
	//btTransform bttransform;
	TransformConverter transformConverter;
	int bulletMass;
	btVector3 bulletInertia;

	btDefaultMotionState* bulletMotion;
	btCollisionShape* bulletShape;
	btRigidBody* bulletRigidBody;
};
