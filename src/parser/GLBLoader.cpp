#include <map>

#include "GLBLoader.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include "tiny_gltf.h"

#include "parser/stb_image.h"
#include "parser/stb_image_resize.h"

namespace GLSLPT
{
	struct GLTFNode
	{
		GLTFNode* parent = nullptr;
		int meshID = -1;
		int materialID = -1;
		glm::mat4x4 transform;
	};

	void LoadGLTFNode(GLTFNode& parent, tinygltf::Node& gltfNode, tinygltf::Model &gltfModel)
	{

	}

	bool LoadSceneFromGLTF(const std::string& filename, Scene* scene, RenderOptions& renderOptions)
	{
		tinygltf::Model gltfModel;
		tinygltf::TinyGLTF gltfContext;
		std::string error;
		std::string warning;

		gltfContext.LoadBinaryFromFile(&gltfModel, &error, &warning, filename);

		// load textures
		std::map<std::string, int> texturesMap;
		for (int32 i = 0; i < gltfModel.textures.size(); ++i)
		{
			tinygltf::Texture& tex = gltfModel.textures[i];
			tinygltf::Image& image = gltfModel.images[tex.source];
			int id = scene->AddTexture(tex.name, image.image.data, image.width, image.height, image.component);
			texturesMap.insert(std::make_pair(tex.name, id));
		}

		// load materials
		std::map<std::string, int> materialsMap;
		for (int32 i = 0; i < gltfModel.materials.size(); ++i)
		{
			tinygltf::Material& gltfMat = gltfModel.materials[i];
			Material material;

			if (gltfMat.values.find("baseColorFactor") != gltfMat.values.end()) {
				material.albedo.x = gltfMat.values["baseColorFactor"].ColorFactor().data()[0];
				material.albedo.y = gltfMat.values["baseColorFactor"].ColorFactor().data()[1];
				material.albedo.z = gltfMat.values["baseColorFactor"].ColorFactor().data()[2];
				// material.albedo.w = gltfMat.values["baseColorFactor"].ColorFactor().data()[3];
			}

			if (gltfMat.values.find("baseColorTexture") != gltfMat.values.end()) {
				material.albedoTexID = gltfMat.values["baseColorTexture"].TextureIndex();
			}

			if (gltfMat.values.find("roughnessFactor") != gltfMat.values.end()) {
				material.roughness = gltfMat.values["roughnessFactor"].Factor();
			}

			if (gltfMat.values.find("metallicFactor") != gltfMat.values.end()) {
				material.metallic = gltfMat.values["metallicFactor"].Factor();
			}
			
			int id = scene->AddMaterial(material);
			materialsMap.insert(std::make_pair(gltfMat.name, id));
		}

		// load meshes
		std::map<std::string, int> meshesMap;
		for (int32 i = 0; i < gltfModel.meshes.size(); ++i)
		{
			tinygltf::Mesh& gltfMesh = gltfModel.meshes[i];
			Mesh* mesh = new Mesh();
			mesh->name = gltfMesh.name;
			mesh->loaded = true;

			std::vector<glm::vec4> verticesUVX;
			std::vector<glm::vec4> normalsUVY;
			std::vector<int> indices;

			// merge primitives into one mesh
			int32 startIndex = 0;
			for (int32 j = 0; j < gltfMesh.primitives.size(); ++j)
			{
				tinygltf::Primitive& primitive = gltfMesh.primitives[j];

				// vertices
				uint8* bufferPos = nullptr;
				uint8* bufferNormals = nullptr;
				uint8* bufferUV0 = nullptr;

				tinygltf::Accessor& posAccessor = gltfModel.accessors[primitive.attributes.find("POSITION")->second];
				tinygltf::BufferView& posView = gltfModel.bufferViews[posAccessor.bufferView];
				bufferPos = &(gltfModel.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]);

				tinygltf::Accessor& normAccessor = gltfModel.accessors[primitive.attributes.find("NORMAL")->second];
				tinygltf::BufferView& normView = gltfModel.bufferViews[normAccessor.bufferView];
				bufferNormals = &(gltfModel.buffers[normView.buffer].data[normAccessor.byteOffset + normView.byteOffset]);

				tinygltf::Accessor& uvAccessor = gltfModel.accessors[primitive.attributes.find("TEXCOORD_0")->second];
				tinygltf::BufferView& uvView = gltfModel.bufferViews[uvAccessor.bufferView];
				bufferUV0 = &(gltfModel.buffers[uvView.buffer].data[uvAccessor.byteOffset + uvView.byteOffset]);

				for (int32 v = 0; v < posAccessor.count; ++v)
				{
					glm::vec4 pos;
					glm::vec4 nrm;

					// pos
					{
						const float* buf = (const float*)(bufferPos);
						pos.x = buf[v * 3 + 0];
						pos.y = buf[v * 3 + 1];
						pos.z = buf[v * 3 + 2];
					}
					// uv
					{
						const float* buf = (const float*)(bufferUV0);
						pos.w = buf[v * 2 + 0];
						nrm.y = buf[v * 2 + 1];
					}
					// normal
					{
						const float* buf = (const float*)(bufferNormals);
						nrm.x = buf[v * 3 + 0];
						nrm.y = buf[v * 3 + 1];
						nrm.z = buf[v * 3 + 2];
					}
					
					verticesUVX.push_back(pos);
					normalsUVY.push_back(nrm);
				}
				
				// indices
				tinygltf::Accessor& indicesAccessor = gltfModel.accessors[primitive.indices];
				tinygltf::BufferView& indicesBufferView = gltfModel.bufferViews[indicesAccessor.bufferView];
				uint8* bufferIndices = &(gltfModel.buffers[indicesBufferView.buffer].data[indicesAccessor.byteOffset + indicesBufferView.byteOffset]);

				for (int32 v = 0; v < indicesAccessor.count; ++v)
				{
					if (indicesAccessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
					{
						const uint32* buf = (const uint32*)(bufferIndices);
						indices.push_back(buf[v] + startIndex);
					}
					else if (indicesAccessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
					{
						const uint16* buf = (const uint16*)(bufferIndices);
						indices.push_back(buf[v] + startIndex);
					}
					else if (indicesAccessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
					{
						const uint8* buf = (const uint8*)(bufferIndices);
						indices.push_back(buf[v] + startIndex);
					}
				}
				startIndex += indicesAccessor.count;

				// sort vertices
				for (int i = 0; i < indices.size(); ++i)
				{
					glm::vec4 posu = verticesUVX[indices[i]];
					glm::vec4 nrmv = normalsUVY[indices[i]];
					mesh->verticesUVX.push_back(posu);
					mesh->normalsUVY.push_back(nrmv);
				}
				
				int id = scene->AddMesh(mesh);
				meshesMap.insert(std::make_pair(gltfMesh.name, id));
			}
		}

		// load nodes
		tinygltf::Scene& gltfScene = gltfModel.scenes[0];
		GLTFNode rootNode;
		for (int i = 0; i < gltfScene.nodes.size(); ++i)
		{
			tinygltf::Node& gltfNode = gltfModel.nodes[gltfScene.nodes[i]];
			LoadGLTFNode(rootNode, gltfNode, gltfModel);
		}
		
	}
}