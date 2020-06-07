#pragma once

#include <vector>

#include "math/Vector2.h"
#include "gfx/GfxTexture.h"

#include "Quad.h"
#include "Program.h"

namespace GLSLPT
{
    Program* LoadShaders(const std::string& vertFileName, const std::string& fragFileName);

	void GenTexture2D(GLuint& target, GLint internalformat, GLenum format, GLenum type, int width, int height, void* data);

    struct RenderOptions
    {
        RenderOptions()
        {
            maxDepth   = 4;
            numTilesX  = 4;
            numTilesY  = 4;
            useEnvMap  = false;
            windowSize = Vector2(1280, 720);
            frameSize  = windowSize;
			intensity  = 1.0f;
        }

        Vector2 windowSize;
        Vector2 frameSize;
        
        int maxDepth;
        int numTilesX;
        int numTilesY;
        bool useEnvMap;
        float intensity;
    };

    class Scene;

    class Renderer
    {
    public:
        Renderer(Scene *scene, const std::string& shadersDirectory);

        virtual ~Renderer();
        
        virtual void Init();
        virtual void Dispose();

        virtual void Render() = 0;
        virtual void Update(float secondsElapsed);

        virtual float GetProgress() const = 0;
        virtual int GetSampleCount() const = 0;

	protected:
		GfxTexture* bvhTex = nullptr;
		GfxTexture* aabbMinTex = nullptr;
		GfxTexture* aabbMaxTex = nullptr;
		GfxTexture* vertexIndicesTex = nullptr;
		GfxTexture* verticesTex = nullptr;
		GfxTexture* normalsTex = nullptr;
		GfxTexture* materialsTex = nullptr;
		GfxTexture* transformsTex = nullptr;
		GfxTexture* lightsTex = nullptr;
		GfxTexture* textureMapsArrayTex = nullptr;
		GfxTexture* hdrTex = nullptr;
		GfxTexture* hdrMarginalDistTex = nullptr;
		GfxTexture* hdrConditionalDistTex = nullptr;
        
		bool initialized;

		Scene* scene;
		Quad *quad;
		int numOfLights;
		std::string shadersDirectory;
    };
}
