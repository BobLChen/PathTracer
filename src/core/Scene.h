#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>

#include "Renderer.h"
#include "Mesh.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "Light.h"

#include "bvh/Bvh.h"
#include "bvh/BvhTranslator.h"

#include "parser/HDRLoader.h"

#include "math/Math.h"

namespace GLSLPT
{
	class Camera;

	struct Indices
	{
		int x, y, z;
	};

	class Scene
	{
	public:
		Scene() 
			: camera(nullptr)
			, hdrData(nullptr) 
		{
			sceneBvh = new RadeonRays::Bvh(10.0f, 64, false);
		}

		~Scene() 
		{ 
			if (camera) 
			{
				delete camera; 
				camera = nullptr;
			}
			
			if (sceneBvh)
			{
				delete sceneBvh; 
				sceneBvh = nullptr;
			}
			
			if (hdrData)
			{
				delete hdrData;
				hdrData = nullptr;
			}
		}

		void AddCamera(glm::vec3 eye, glm::vec3 lookat, float fov);
		int AddMesh(const std::string &filename);
		int AddTexture(const std::string &filename);
		int AddMaterial(const Material &material);
		int AddMeshInstance(const MeshInstance &meshInstance);
		int AddLight(const Light &light);
		void AddHDR(const std::string &filename);
		void CreateAccelerationStructures();
		void RebuildInstancesData();

	private:
		void CreateBLAS();
		void CreateTLAS();

	public:
		// Options
		RenderOptions renderOptions;
		// Mesh Data
		std::vector<Mesh*> meshes;
		// Instance Data
		std::vector<Material> materials;
		std::vector<MeshInstance> meshInstances;
		// Lights
		std::vector<Light> lights;
		// HDR
		HDRData* hdrData;
		// Camera
		Camera* camera;
		// Scene Mesh Data 
		std::vector<Indices> vertIndices;
		std::vector<glm::vec4> verticesUVX;
		std::vector<glm::vec4> normalsUVY;
		std::vector<glm::mat4> transforms;
		// texture size
		int indicesTexWidth;
		int triDataTexWidth;
		// Bvh
		RadeonRays::BvhTranslator bvhTranslator;
		// Texture Data
		std::vector<Texture*> textures;
		std::vector<uint8> textureMapsArray;
		int texWidth;
		int texHeight;
		Bounds3D sceneBounds;
		bool instancesModified = false;

	private:
		RadeonRays::Bvh* sceneBvh;
	};
}