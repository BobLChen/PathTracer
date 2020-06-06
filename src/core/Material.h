#pragma once

#include "math/Vector3.h"
#include "math/Vector4.h"

namespace GLSLPT
{
	enum MaterialType
	{
		DISNEY,
		GLASS
	};

	class Material
	{
	public:
		Material()
		{
			albedo = Vector3(1.0f, 1.0f, 1.0f);
			type   = DISNEY;

			emission = Vector3(0.0f, 0.0f, 0.0f);
			unused   = 0;

			metallic  = 0.0f;
			roughness = 0.5f;
			ior       = 1.45f;
			transmittance = 0.0f;

			albedoTexID    = -1.0f;
			paramsTexID    = -1.0f;
			normalmapTexID = -1.0f;
			emissionTexID  = -1.0f;
		};

		Vector3 albedo;
		float type;

		Vector3 emission;
		float unused;

		float metallic;
		float roughness;
		float ior;
		float transmittance;

		float albedoTexID;
		float paramsTexID; // metallicRoughness
		float normalmapTexID;
		float emissionTexID;
	};
}
