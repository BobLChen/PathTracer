#include "glad/glad.h"

#include "Renderer.h"
#include "Scene.h"

namespace GLSLPT
{
    Program* LoadShaders(const std::string& vertFileName, const std::string& fragFileName)
    {
        std::vector<Shader> shaders;
        shaders.push_back(Shader(vertFileName, GL_VERTEX_SHADER));
        shaders.push_back(Shader(fragFileName, GL_FRAGMENT_SHADER));
        return new Program(shaders);
    }

	void GenTexture2D(GLuint& target, GLint internalformat, GLenum format, GLenum type, int width, int height, void* data)
	{
		glGenTextures(1, &target);
		glBindTexture(GL_TEXTURE_2D, target);
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

    Renderer::Renderer(Scene* scene, const std::string& shadersDirectory) 
		: textureMapsArrayTex(0)
		, hdrTex(0)
		, hdrMarginalDistTex(0)
		, hdrConditionalDistTex(0)
        , initialized(false)
        , scene(scene)
        , screenSize(scene->renderOptions.resolution)
        , shadersDirectory(shadersDirectory)
    {

    }

    Renderer::~Renderer()
    {
		if (initialized) 
		{
			this->Dispose();
		}
    }

    void Renderer::Dispose()
    {
		if (!initialized) 
		{
			return;
		}
        
		glDeleteTextures(1, &bvhTex);
		glDeleteTextures(1, &aabbMinTex);
		glDeleteTextures(1, &aabbMaxTex);
		glDeleteTextures(1, &vertexIndicesTex);
		glDeleteTextures(1, &verticesTex);
		glDeleteTextures(1, &normalsTex);
		glDeleteTextures(1, &materialsTex);
		glDeleteTextures(1, &transformsTex);
		glDeleteTextures(1, &lightsTex);
		glDeleteTextures(1, &textureMapsArrayTex);
		glDeleteTextures(1, &hdrTex);
		glDeleteTextures(1, &hdrMarginalDistTex);
		glDeleteTextures(1, &hdrConditionalDistTex);

        initialized = false;
		printf("Renderer disposed!\n");
    }

    void Renderer::Init()
    {
		if (initialized) {
			return;
		}
        
        if (scene == nullptr)
        {
			printf("Error: No Scene Found\n");
            return ;
        }

        quad = new Quad();

		// Create texture for BVH Tree
		GenTexture2D(bvhTex, GL_RGB32I, GL_RGB_INTEGER, GL_INT, scene->bvhTranslator.nodeTexWidth, scene->bvhTranslator.nodeTexWidth, &scene->bvhTranslator.nodes[0]);
		
		// Create texture for Bounding boxes
		GenTexture2D(aabbMinTex, GL_RGB32F, GL_RGB, GL_FLOAT, scene->bvhTranslator.nodeTexWidth, scene->bvhTranslator.nodeTexWidth, &scene->bvhTranslator.bboxmin[0]);
		GenTexture2D(aabbMaxTex, GL_RGB32F, GL_RGB, GL_FLOAT, scene->bvhTranslator.nodeTexWidth, scene->bvhTranslator.nodeTexWidth, &scene->bvhTranslator.bboxmax[0]);
		
		// Create texture for VertexIndices
		GenTexture2D(vertexIndicesTex, GL_RGB32I, GL_RGB_INTEGER, GL_INT, scene->indicesTexWidth, scene->indicesTexWidth, &scene->vertIndices[0]);
		
		// Create texture for Vertices
		GenTexture2D(verticesTex, GL_RGBA32F, GL_RGBA, GL_FLOAT, scene->triDataTexWidth, scene->triDataTexWidth, &scene->verticesUVX[0]);
		GenTexture2D(normalsTex,  GL_RGBA32F, GL_RGBA, GL_FLOAT, scene->triDataTexWidth, scene->triDataTexWidth, &scene->normalsUVY[0]);

		// Create texture for Materials
		GenTexture2D(materialsTex,  GL_RGBA32F, GL_RGBA, GL_FLOAT, (sizeof(Material) / sizeof(Vector4)) * scene->materials.size(), 1, &scene->materials[0]);

		// Create texture for Transforms
		GenTexture2D(transformsTex,  GL_RGBA32F, GL_RGBA, GL_FLOAT, (sizeof(Matrix4x4) / sizeof(Vector4)) * scene->transforms.size(), 1, &scene->transforms[0]);

		// Create Buffer and Texture for Lights
		numOfLights = int(scene->lights.size());

		if (numOfLights > 0)
		{
			// Create texture for lights
			GenTexture2D(lightsTex,  GL_RGB32F, GL_RGB, GL_FLOAT, (sizeof(Light) / sizeof(Vector3)) * scene->lights.size(), 1, &scene->lights[0]);
		}

		if (scene->textures.size() > 0)
		{
			glGenTextures(1, &textureMapsArrayTex);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D_ARRAY, textureMapsArrayTex);
			glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB8, scene->texWidth, scene->texHeight, scene->textures.size(), 0, GL_RGB, GL_UNSIGNED_BYTE, &scene->textureMapsArray[0]);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		}

		// Environment Map
		if (scene->hdrData != nullptr)
		{
			GenTexture2D(hdrTex, GL_RGB32F, GL_RGB, GL_FLOAT, scene->hdrData->width, scene->hdrData->height, scene->hdrData->cols);
			glBindTexture(GL_TEXTURE_2D, hdrTex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			GenTexture2D(hdrMarginalDistTex, GL_RGB32F, GL_RG, GL_FLOAT, scene->hdrData->height, 1, scene->hdrData->marginalDistData);
			GenTexture2D(hdrConditionalDistTex, GL_RG32F, GL_RG, GL_FLOAT, scene->hdrData->width, scene->hdrData->height, scene->hdrData->conditionalDistData);
		}

        initialized = true;
    }
	
	void Renderer::Update(float secondsElapsed)
	{
		if (scene->instancesModified)
		{
			glBindTexture(GL_TEXTURE_2D, transformsTex);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (sizeof(Matrix4x4) / sizeof(Vector4)) * scene->transforms.size(), 1, GL_RGBA, GL_FLOAT, &scene->transforms[0]);

			glBindTexture(GL_TEXTURE_2D, materialsTex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (sizeof(Material) / sizeof(Vector4)) * scene->materials.size(), 1, 0, GL_RGBA, GL_FLOAT, &scene->materials[0]);

			int yPos  = scene->bvhTranslator.topLevelIndexPackedXY & 0x00000FFF;
			int index = yPos * scene->bvhTranslator.nodeTexWidth;

			glBindTexture(GL_TEXTURE_2D, bvhTex);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, yPos, scene->bvhTranslator.nodeTexWidth, scene->bvhTranslator.nodeTexWidth - yPos, GL_RGB_INTEGER, GL_INT, &scene->bvhTranslator.nodes[index]);

			glBindTexture(GL_TEXTURE_2D, aabbMinTex);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, yPos, scene->bvhTranslator.nodeTexWidth, scene->bvhTranslator.nodeTexWidth - yPos, GL_RGB, GL_FLOAT, &scene->bvhTranslator.bboxmin[index]);

			glBindTexture(GL_TEXTURE_2D, aabbMaxTex);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, yPos, scene->bvhTranslator.nodeTexWidth, scene->bvhTranslator.nodeTexWidth - yPos, GL_RGB, GL_FLOAT, &scene->bvhTranslator.bboxmax[index]);
		}
	}
}
