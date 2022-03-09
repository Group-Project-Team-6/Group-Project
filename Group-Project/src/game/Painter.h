#pragma once
#include "GameEntity.h"
#include "../common/Vector2.h"

using namespace NCL;
using namespace Rendering;
using namespace Maths;

typedef std::map<GameEntity*, Vector2> PainterMap;
typedef std::pair<GameEntity*, Vector2> PainterPair;

class Painter {
public:
	~Painter() {}
	static void Paint(GameEntity* entity, Vector2 uv);
	static PainterMap GetPaintInfos();
	static void ClearPaint();
private:
	Painter() {}
	static PainterMap paintInfos;
};