#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "bvh/SplitBvh.h"

namespace GLSLPT
{	
	class Mesh
	{
	public:
		Mesh()
		{ 
			bvh = new RadeonRays::SplitBvh(2.0f, 64, 0, 0.001f, 2.5f); 
		}

		~Mesh() 
		{ 
			if (bvh)
			{
				delete bvh;
				bvh = nullptr;
			}
		}
		
	public:

		// Mesh Data
		std::vector<glm::vec4> verticesUVX;
		std::vector<glm::vec4> normalsUVY;

		RadeonRays::Bvh *bvh;
		std::string name;

		void BuildBVH();
		bool LoadFromFile(const std::string& filename);
	};

	class MeshInstance
	{
	public:
		MeshInstance(int meshId, glm::mat4 xform, int matId) 
			: meshID(meshId)
			, transform(xform) 
			, materialID(matId) 
		{

		}

		~MeshInstance() 
		{

		}

		glm::mat4 transform;
		int materialID;
		int meshID;
	};
}
