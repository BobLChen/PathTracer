#pragma once

#include "math/Vector3.h"

namespace GLSLPT
{
	enum LightType
	{
		QuadLight,
		SphereLight
	};

	struct Light
	{
		Vector3 position;
		Vector3 emission;
		Vector3 u;
		Vector3 v;

		float radius;
		float area;
		float type;
	};
}
