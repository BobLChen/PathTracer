#pragma once

#include <glm/glm.hpp>

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
			albedo = glm::vec3(1.0f, 1.0f, 1.0f);
			materialType = DISNEY;

			emission = glm::vec3(0.0f, 0.0f, 0.0f);
			unused = 0;

			metallic = 0.0f;
			roughness = 0.5f;
			ior = 1.45f;
			transmittance = 0.0f;

			albedoTexID = -1.0f;
			metallicRoughnessTexID = -1.0f;
			normalmapTexID = -1.0f;
			heightmapTexID = -1.0f;
		};

		glm::vec3 albedo;
		float materialType;

		glm::vec3 emission;
		float unused;

		float metallic;
		float roughness;
		float ior;
		float transmittance;

		float albedoTexID;
		float metallicRoughnessTexID;
		float normalmapTexID;
		float heightmapTexID;
	};
}