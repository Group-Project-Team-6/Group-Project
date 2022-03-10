#pragma once
#include "../common/Vector3.h"
#include "GameEntity.h"

class Bullet : public GameEntity{
public:
	Bullet() : framesLeft(0) {}; //Intialise variables to null
	~Bullet();

	void Init(Transform startTransform, int lifeTime, RendererBase& renderer);
	bool Animate();
	void InitAssets(RendererBase& r);

	bool inUse() const { return framesLeft > 0; }

	/*Bullet* getNext() const {
		return state.next;
	}
	void SetNext(Bullet* next) {
		state.next;
	}*/

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
	static bool initialized;
	int framesLeft;
	
	Bullet* next;

	static MeshGeometry* bulletMesh;
	static TextureBase* bulletTex;
	static ShaderBase* bulletShader;

	TransformConverter transformConverter;
	int bulletMass;
	btVector3 bulletInertia;

	btDefaultMotionState* bulletMotion;
	btCollisionShape* bulletShape;
	btRigidBody* bulletRigidBody;
};
