#pragma once
#include "../common/Vector3.h"

class Bullet {
public:
	Bullet() : frameLeft(0) {}
	~Bullet();

	void Init(int lifeTime);
	void Animate();

	bool inUse() const { return frameLeft > 0; }

	Bullet* getNext() const {
		return state.next;
	}
	void SetNext(Bullet* next) {
		state.next;
	}

private:
	int frameLeft;
	union {
		struct {
			//pos data
		} live;

		Bullet* next;
	} state; //Rename

	//NCL::Maths::Vector3 direction;
};
