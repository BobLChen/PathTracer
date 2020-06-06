#pragma once

#include <vector>

#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"
#include "bvh/SplitBvh.h"

namespace GLSLPT
{	
	class Mesh
	{
	public:
		Mesh()
			: loaded(false)
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
		
		void BuildBVH();

		bool LoadFromFile(const std::string& filename);

	public:
		// Mesh Data
		std::vector<Vector4> verticesUVX;
		std::vector<Vector4> normalsUVY;

		RadeonRays::Bvh* bvh;
		std::string name;
		bool loaded;
	};

	class MeshInstance
	{
	public:
		MeshInstance(int meshId, Matrix4x4 xform, int matId, const std::string& inName)
            : transform(xform)
            , materialID(matId)
			, meshID(meshId)
			, name(inName)
		{

		}

		~MeshInstance() 
		{

		}

		Matrix4x4	transform;
		int			materialID;
		int			meshID;
		std::string name;
	};
}
