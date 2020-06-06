#include <iostream>

#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "parser/tiny_obj_loader.h"

namespace GLSLPT
{
	bool Mesh::LoadFromFile(const std::string& filename)
	{
		name = filename;
		loaded = true;

		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err;

		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), 0, true);

		if (!ret)
		{
			printf("Unable to load model\n");
			return false;
		}

		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) 
		{
			// Loop over faces(polygon)
			size_t indexOffset = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
			{
				int fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++)
				{
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];
					tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
					tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
					tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
					tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
					tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
					tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

					tinyobj::real_t tx, ty;
					
					// temporary fix
					if (attrib.texcoords.size() > 0)
					{
						tx = attrib.texcoords[2 * idx.texcoord_index + 0];
						ty = attrib.texcoords[2 * idx.texcoord_index + 1];
					}
					else
					{
						tx = ty = 0;
					}

					verticesUVX.push_back(Vector4(vx, vy, vz, tx));
					normalsUVY.push_back(Vector4(nx, ny, nz, ty));
				}
	
				indexOffset += fv;
			}
		}

		return true;
	}

	void Mesh::BuildBVH()
	{
		const int numTris = verticesUVX.size() / 3;
		std::vector<Bounds3D> bounds(numTris);

		for (int i = 0; i < numTris; ++i)
		{
			const Vector3 v1 = Vector3(verticesUVX[i * 3 + 0]);
			const Vector3 v2 = Vector3(verticesUVX[i * 3 + 1]);
			const Vector3 v3 = Vector3(verticesUVX[i * 3 + 2]);

			bounds[i].Expand(v1);
			bounds[i].Expand(v2);
			bounds[i].Expand(v3);
		}

		bvh->Build(&bounds[0], numTris);
	}
}
