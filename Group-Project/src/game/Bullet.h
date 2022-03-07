#pragma once
#include "../common/Vector3.h"
#include "GameEntity.h"

class Bullet : public GameEntity{
public:
	Bullet() : framesLeft(0) {};
	~Bullet();

	void Init(Transform startTransform, int lifeTime);
	bool Animate();
	void InitAssets();

	bool inUse() const { return framesLeft > 0; }

	Bullet* getNext() const {
		return state.next;
	}
	void SetNext(Bullet* next) {
		state.next;
	}

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
	union {
		struct {
			Vector3 position;
			Vector3 force;
		} live;

		Bullet* next;
	} state; //Rename

	OGLMesh* bulletMesh;
	OGLTexture* bulletTex;
	OGLShader* bulletShader;

	TransformConverter transformConverter;
	int bulletMass;
	btVector3 bulletInertia;

	btDefaultMotionState* bulletMotion;
	btCollisionShape* bulletShape;
	btRigidBody* bulletRigidBody;
};
