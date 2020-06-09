/**********************************************************************
Copyright (c) 2020 BobLChen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
********************************************************************/

#include <map>

#include "math/Matrix4x4.h"
#include "math/Quat.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

#include "GLBLoader.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include "tiny_gltf.h"

#include "parser/stb_image.h"
#include "parser/stb_image_resize.h"

namespace GLSLPT
{
	struct GLTFBounds
	{
		Vector3 min;
		Vector3 max;
		Vector3 corners[8];

		GLTFBounds()
			: min(+MAX_int16, +MAX_int16, +MAX_int16)
			, max(-MAX_int16, -MAX_int16, -MAX_int16)
		{

		}

		GLTFBounds(const GLTFBounds& bounds)
			: min(bounds.min)
			, max(bounds.max)
		{

		}

		Vector3 Center() const
		{
			return (min + max) * 0.5f;
		}

		Vector3 Extents() const
		{
			return max - min;
		}

		void UpdateCorners()
		{
			corners[0].Set(min.x, min.y, min.z);
			corners[1].Set(max.x, min.y, min.z);
			corners[2].Set(min.x, max.y, min.z);
			corners[3].Set(max.x, max.y, min.z);

			corners[4].Set(min.x, min.y, max.z);
			corners[5].Set(max.x, min.y, max.z);
			corners[6].Set(min.x, max.y, max.z);
			corners[7].Set(max.x, max.y, max.z);
		}
	};

	struct GLTFNode
	{
		GLTFNode*	parent = nullptr;
		int			meshID = -1;
		int			materialID = -1;
		std::string name;
		Matrix4x4	matrix;
		GLTFBounds	bounds;

		Matrix4x4 WorldMatrix()
		{
			Matrix4x4 transform = matrix;
			if (parent) {
				transform.Append(parent->WorldMatrix());
			}
			return transform;
		}
	};

	void LoadGLTFNode(GLTFNode* parent, tinygltf::Node& gltfNode, tinygltf::Model &gltfModel, std::vector<GLTFNode*>& nodes)
	{
		GLTFNode* node = new GLTFNode();
		node->name   = gltfNode.name;
		node->parent = parent;

		nodes.push_back(node);

		if (gltfNode.matrix.size() == 16)
		{
			node->matrix.m[0][0] = gltfNode.matrix[0];
			node->matrix.m[0][1] = gltfNode.matrix[1];
			node->matrix.m[0][2] = gltfNode.matrix[2];
			node->matrix.m[0][3] = gltfNode.matrix[3];

			node->matrix.m[1][0] = gltfNode.matrix[4];
			node->matrix.m[1][1] = gltfNode.matrix[5];
			node->matrix.m[1][2] = gltfNode.matrix[6];
			node->matrix.m[1][3] = gltfNode.matrix[7];

			node->matrix.m[2][0] = gltfNode.matrix[8];
			node->matrix.m[2][1] = gltfNode.matrix[9];
			node->matrix.m[2][2] = gltfNode.matrix[10];
			node->matrix.m[2][3] = gltfNode.matrix[11];

			node->matrix.m[3][0] = gltfNode.matrix[12];
			node->matrix.m[3][1] = gltfNode.matrix[13];
			node->matrix.m[3][2] = gltfNode.matrix[14];
			node->matrix.m[3][3] = gltfNode.matrix[15];
		}
		else
		{
			if (gltfNode.rotation.size() == 4) 
			{
				Quat quat(gltfNode.rotation[0], gltfNode.rotation[1], gltfNode.rotation[2], gltfNode.rotation[3]);
				node->matrix.Append(quat.ToMatrix());
			}
			if (gltfNode.scale.size() == 3) 
			{
				node->matrix.AppendScale(Vector3(gltfNode.scale[0], gltfNode.scale[1], gltfNode.scale[2]));
			}
			if (gltfNode.translation.size() == 3) 
			{
				node->matrix.AppendTranslation(Vector3(gltfNode.translation[0], gltfNode.translation[1], gltfNode.translation[2]));
			}
		}

		if (gltfNode.mesh > -1) 
		{
			node->meshID = gltfNode.mesh;
		}

		for (int i = 0; i < gltfNode.children.size(); ++i) 
		{
			LoadGLTFNode(node, gltfModel.nodes[gltfNode.children[i]], gltfModel, nodes);
		}
	}

	bool LoadSceneFromGLTF(const std::string& filename, Scene* scene)
	{
		tinygltf::Model gltfModel;
		tinygltf::TinyGLTF gltfContext;
		std::string error;
		std::string warning;

		gltfContext.LoadBinaryFromFile(&gltfModel, &error, &warning, filename);

		// load textures
		std::vector<int> textures;
		for (int32 i = 0; i < gltfModel.textures.size(); ++i)
		{
			tinygltf::Texture& tex = gltfModel.textures[i];
			tinygltf::Image& image = gltfModel.images[tex.source];
			int textureID = scene->AddTexture(tex.name + std::to_string(i), image.image.data(), image.width, image.height, image.component);
			textures.push_back(textureID);
		}

		// load materials
		std::vector<int> materials;
		for (int32 i = 0; i < gltfModel.materials.size(); ++i)
		{
			tinygltf::Material& gltfMat = gltfModel.materials[i];
			Material material;
			material.roughness = 0.99f;
			material.metallic  = 0.99f;

			if (gltfMat.values.find("baseColorFactor") != gltfMat.values.end()) 
			{
				material.albedo.x = gltfMat.values["baseColorFactor"].ColorFactor().data()[0];
				material.albedo.y = gltfMat.values["baseColorFactor"].ColorFactor().data()[1];
				material.albedo.z = gltfMat.values["baseColorFactor"].ColorFactor().data()[2];
			}

			if (gltfMat.values.find("baseColorTexture") != gltfMat.values.end()) 
			{
				material.albedoTexID = textures[gltfMat.values["baseColorTexture"].TextureIndex()];
				// gltfMat.values["baseColorTexture"].TextureTexCoord();
			}

			if (gltfMat.values.find("metallicRoughnessTexture") != gltfMat.values.end()) 
			{
				material.paramsTexID = textures[gltfMat.values["metallicRoughnessTexture"].TextureIndex()];
				// gltfMat.values["metallicRoughnessTexture"].TextureTexCoord();
			}

			if (gltfMat.values.find("roughnessFactor") != gltfMat.values.end()) 
			{
				material.roughness = gltfMat.values["roughnessFactor"].Factor();
			}

			if (gltfMat.values.find("metallicFactor") != gltfMat.values.end()) 
			{
				material.metallic = gltfMat.values["metallicFactor"].Factor();
			}

			if (gltfMat.additionalValues.find("normalTexture") != gltfMat.additionalValues.end()) 
			{
				material.normalmapTexID = textures[gltfMat.additionalValues["normalTexture"].TextureIndex()];
				// gltfMat.additionalValues["normalTexture"].TextureTexCoord();
			}

			if (gltfMat.additionalValues.find("emissiveTexture") != gltfMat.additionalValues.end()) 
			{
				material.emissionTexID = gltfMat.additionalValues["emissiveTexture"].TextureIndex();
				gltfMat.additionalValues["emissiveTexture"].TextureTexCoord();
			}

			if (gltfMat.additionalValues.find("emissiveFactor") != gltfMat.additionalValues.end()) 
			{
				material.emission.x = gltfMat.additionalValues["emissiveFactor"].ColorFactor().data()[0];
				material.emission.y = gltfMat.additionalValues["emissiveFactor"].ColorFactor().data()[1];
				material.emission.z = gltfMat.additionalValues["emissiveFactor"].ColorFactor().data()[2];
			}

			int materialID = scene->AddMaterial(material);
			materials.push_back(materialID);
		}

		// load meshes
		struct GLTFMesh
		{
			int			mesh;
			int			meshID;
			int			materialID;
			GLTFBounds	bounds;

			GLTFMesh(int id, int inMesh, int inMaterial, const GLTFBounds& inBounds)
				: mesh(id)
				, meshID(inMesh)
				, materialID(inMaterial)
				, bounds(inBounds)
			{

			}
		};

		std::vector<GLTFMesh> meshes;
		for (int32 i = 0; i < gltfModel.meshes.size(); ++i)
		{
			tinygltf::Mesh& gltfMesh = gltfModel.meshes[i];

			for (int32 j = 0; j < gltfMesh.primitives.size(); ++j)
			{
				tinygltf::Primitive& primitive = gltfMesh.primitives[j];

				std::vector<Vector4> verticesUVX;
				std::vector<Vector4> normalsUVY;
				std::vector<int>     indices;
				GLTFBounds           bounds;

				tinygltf::Accessor& posAccessor = gltfModel.accessors[primitive.attributes.find("POSITION")->second];
				tinygltf::BufferView& posView   = gltfModel.bufferViews[posAccessor.bufferView];
				uint8* bufferPos                = &(gltfModel.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]);

				tinygltf::Accessor& normAccessor = gltfModel.accessors[primitive.attributes.find("NORMAL")->second];
				tinygltf::BufferView& normView   = gltfModel.bufferViews[normAccessor.bufferView];
				uint8* bufferNormals             = &(gltfModel.buffers[normView.buffer].data[normAccessor.byteOffset + normView.byteOffset]);

				tinygltf::Accessor& uvAccessor = gltfModel.accessors[primitive.attributes.find("TEXCOORD_0")->second];
				tinygltf::BufferView& uvView   = gltfModel.bufferViews[uvAccessor.bufferView];
				uint8* bufferUV0               = &(gltfModel.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]);

				for (int32 v = 0; v < posAccessor.count; ++v)
				{
					Vector4 pos;
					Vector4 nrm;

					// pos
					if (bufferPos)
					{
						const float* buf = (const float*)(bufferPos);
						pos.x = buf[v * 3 + 0];
						pos.y = buf[v * 3 + 1];
						pos.z = buf[v * 3 + 2];
					}
					else
					{
						pos.x = 0;
						pos.y = 0;
						pos.z = 0;
					}

					bounds.min = Vector3::Min(bounds.min, pos);
					bounds.max = Vector3::Max(bounds.max, pos);

					// uv
					if (bufferUV0)
					{
						const float* buf = (const float*)(bufferUV0);
						pos.w = buf[v * 2 + 0];
						nrm.w = 1.0f - buf[v * 2 + 1];
					}
					else
					{
						pos.w = 0;
						nrm.w = 0;
					}
					// normal
					if (bufferNormals)
					{
						const float* buf = (const float*)(bufferNormals);
						nrm.x = buf[v * 3 + 0];
						nrm.y = buf[v * 3 + 1];
						nrm.z = buf[v * 3 + 2];
					}
					else
					{
						nrm.x = 0;
						nrm.y = 0;
						nrm.z = 0;
					}

					verticesUVX.push_back(pos);
					normalsUVY.push_back(nrm);
				}

				// indices
				tinygltf::Accessor& indicesAccessor     = gltfModel.accessors[primitive.indices];
				tinygltf::BufferView& indicesBufferView = gltfModel.bufferViews[indicesAccessor.bufferView];
				uint8* bufferIndices                    = &(gltfModel.buffers[indicesBufferView.buffer].data[indicesAccessor.byteOffset + indicesBufferView.byteOffset]);

				for (int32 v = 0; v < indicesAccessor.count; ++v)
				{
					if (indicesAccessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
					{
						const uint32* buf = (const uint32*)(bufferIndices);
						indices.push_back(buf[v]);
					}
					else if (indicesAccessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
					{
						const uint16* buf = (const uint16*)(bufferIndices);
						indices.push_back(buf[v]);
					}
					else if (indicesAccessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
					{
						const uint8* buf = (const uint8*)(bufferIndices);
						indices.push_back(buf[v]);
					}
				}

				Mesh* mesh = new Mesh();
				mesh->loaded = true;

				// vertices
				for (int v = 0; v < indices.size(); ++v)
				{
					Vector4 posu = verticesUVX[indices[v]];
					Vector4 nrmv = normalsUVY[indices[v]];
					mesh->verticesUVX.push_back(posu);
					mesh->normalsUVY.push_back(nrmv);
				}

				int meshID = scene->AddMesh(mesh);
				int matID  = materials[primitive.material];
				meshes.push_back(GLTFMesh(i, meshID, matID, bounds));
			}
		}

		// load nodes
		tinygltf::Scene& gltfScene = gltfModel.scenes[0];
		GLTFNode* rootNode = new GLTFNode();
		std::vector<GLTFNode*> nodeList;
		for (int i = 0; i < gltfScene.nodes.size(); ++i)
		{
			tinygltf::Node& gltfNode = gltfModel.nodes[gltfScene.nodes[i]];
			LoadGLTFNode(rootNode, gltfNode, gltfModel, nodeList);
		}

		// instances
		GLTFBounds sceneBounds;
		for (int i = 0; i < nodeList.size(); ++i)
		{
			GLTFNode* node = nodeList[i];
			if (node->meshID == -1) {
				continue;
			}

			for (int j = 0; j < meshes.size(); ++j) 
			{
				GLTFMesh& mesh = meshes[j];
				if (mesh.mesh == node->meshID)
				{
					Matrix4x4 matrix = node->WorldMatrix();

					// add to instance
					scene->AddMeshInstance(MeshInstance(mesh.meshID, matrix, mesh.materialID, node->name));

					// calc world bounds
					GLTFBounds& bounds = mesh.bounds;
					bounds.UpdateCorners();
					for (int b = 0; b < 8; ++b)
					{
						Vector3 vec = matrix.TransformPosition(bounds.corners[b]);
						sceneBounds.min = Vector3::Min(sceneBounds.min, vec);
						sceneBounds.max = Vector3::Max(sceneBounds.max, vec);
					}

					break;
				}
			}
		}

		for (int i = 0; i < nodeList.size(); ++i) 
		{
			delete nodeList[i];
		}

		// fit scene
		Vector3 center = sceneBounds.Center();
		Vector3 extent = sceneBounds.Extents();
		Vector3 eye    = Vector3(center.x, center.y, center.z - extent.Size() * 1.0f);
		Vector3 at     = center;
		scene->AddCamera(eye, at, 60.0f);

		scene->CreateAccelerationStructures();

		return true;
	}
}
