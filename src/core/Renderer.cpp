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
    
    Renderer::Renderer(Scene* scene, const std::string& shadersDirectory)
        : initialized(false)
        , scene(scene)
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
        
        delete bvhTex;
        delete aabbMinTex;
        delete aabbMaxTex;
        delete vertexIndicesTex;
        delete verticesTex;
        delete normalsTex;
        delete materialsTex;
        delete transformsTex;
        
        if (lightsTex) {
            delete lightsTex;
        }
        
        if (textureMapsArrayTex) {
            delete textureMapsArrayTex;
        }
        
        if (hdrTex) {
            delete hdrTex;
        }
        
        if (hdrMarginalDistTex) {
            delete hdrMarginalDistTex;
        }
        
        if (hdrConditionalDistTex) {
            delete hdrConditionalDistTex;
        }
        
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
        bvhTex = new GfxTexture(GL_TEXTURE_2D, GL_RGB32I, GL_RGB_INTEGER, GL_INT, scene->bvhTranslator.nodeTexWidth, scene->bvhTranslator.nodeTexWidth, 1, &scene->bvhTranslator.nodes[0]);
		
		// Create texture for Bounding boxes
        aabbMinTex = new GfxTexture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT, scene->bvhTranslator.nodeTexWidth, scene->bvhTranslator.nodeTexWidth, 1, &scene->bvhTranslator.bboxmin[0]);
        aabbMaxTex = new GfxTexture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT, scene->bvhTranslator.nodeTexWidth, scene->bvhTranslator.nodeTexWidth, 1, &scene->bvhTranslator.bboxmax[0]);
		
		// Create texture for VertexIndices
        vertexIndicesTex = new GfxTexture(GL_TEXTURE_2D, GL_RGB32I, GL_RGB_INTEGER, GL_INT, scene->indicesTexWidth, scene->indicesTexWidth, 1, &scene->vertIndices[0]);
		
		// Create texture for Vertices
        verticesTex = new GfxTexture(GL_TEXTURE_2D, GL_RGBA32F, GL_RGBA, GL_FLOAT, scene->triDataTexWidth, scene->triDataTexWidth, 1, &scene->verticesUVX[0]);
        normalsTex = new GfxTexture(GL_TEXTURE_2D, GL_RGBA32F, GL_RGBA, GL_FLOAT, scene->triDataTexWidth, scene->triDataTexWidth, 1, &scene->normalsUVY[0]);

		// Create texture for Materials
        materialsTex = new GfxTexture(GL_TEXTURE_2D, GL_RGBA32F, GL_RGBA, GL_FLOAT, (sizeof(Material) / sizeof(Vector4)) * scene->materials.size(), 1, 1, &scene->materials[0]);

		// Create texture for Transforms
        transformsTex = new GfxTexture(GL_TEXTURE_2D, GL_RGBA32F, GL_RGBA, GL_FLOAT, (sizeof(Matrix4x4) / sizeof(Vector4)) * scene->transforms.size(), 1, 1, &scene->transforms[0]);

		// Create Buffer and Texture for Lights
		numOfLights = int(scene->lights.size());
		if (numOfLights > 0)
		{
            lightsTex = new GfxTexture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT, (sizeof(Light) / sizeof(Vector3)) * scene->lights.size(), 1, 1, &scene->lights[0]);
		}
        
		if (scene->textures.size() > 0)
		{
            textureMapsArrayTex = new GfxTexture(GL_TEXTURE_2D_ARRAY, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, scene->texWidth, scene->texHeight, scene->textures.size(), scene->textureMapsArray.data());
            textureMapsArrayTex->Filter(GL_LINEAR, GL_LINEAR);
		}
        
		// Environment Map
		if (scene->hdrData != nullptr)
		{
            hdrTex = new GfxTexture(GL_TEXTURE_2D, GL_RGB32F, GL_RGB, GL_FLOAT, scene->hdrData->width, scene->hdrData->height, 1, scene->hdrData->cols);
            hdrTex->Filter(GL_LINEAR, GL_LINEAR);

            hdrMarginalDistTex = new GfxTexture(GL_TEXTURE_2D, GL_RGB32F, GL_RG, GL_FLOAT, scene->hdrData->height, 1, 1, scene->hdrData->marginalDistData);
            
            hdrConditionalDistTex = new GfxTexture(GL_TEXTURE_2D, GL_RG32F, GL_RG, GL_FLOAT, scene->hdrData->width, scene->hdrData->height, 1, scene->hdrData->conditionalDistData);
		}

        initialized = true;
    }
	
	void Renderer::Update(float secondsElapsed)
	{
		if (scene->instancesModified)
		{
            transformsTex->SubImage2D(0, 0, 0, (sizeof(Matrix4x4) / sizeof(Vector4)) * scene->transforms.size(), 1, &scene->transforms[0]);
            
            materialsTex->SubImage2D(0, 0, 0, (sizeof(Material) / sizeof(Vector4)) * scene->materials.size(), 1, &scene->materials[0]);
            
			int yPos  = scene->bvhTranslator.topLevelIndexPackedXY & 0x00000FFF;
			int index = yPos * scene->bvhTranslator.nodeTexWidth;

            bvhTex->SubImage2D(0, 0, yPos, scene->bvhTranslator.nodeTexWidth, scene->bvhTranslator.nodeTexWidth - yPos, &scene->bvhTranslator.nodes[index]);
            
            aabbMinTex->SubImage2D(0, 0, yPos, scene->bvhTranslator.nodeTexWidth, scene->bvhTranslator.nodeTexWidth - yPos, &scene->bvhTranslator.bboxmin[index]);
            
            aabbMaxTex->SubImage2D(0, 0, yPos, scene->bvhTranslator.nodeTexWidth, scene->bvhTranslator.nodeTexWidth - yPos, &scene->bvhTranslator.bboxmax[index]);
		}

		if (scene->hdrModified && hdrTex)
		{
			hdrTex->SubImage2D(0, 0, 0, scene->hdrData->width, scene->hdrData->height, scene->hdrData->cols);
			hdrMarginalDistTex->SubImage2D(0, 0, 0, scene->hdrData->height, 1, scene->hdrData->marginalDistData);
			hdrConditionalDistTex->SubImage2D(0, 0, 0, scene->hdrData->width, scene->hdrData->height, scene->hdrData->conditionalDistData);
		}
	}
}
