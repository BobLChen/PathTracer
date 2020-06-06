#pragma once

#include "Renderer.h"

namespace GLSLPT
{
    class Scene;

    class TiledRenderer : public Renderer
    {
    public:
        TiledRenderer(Scene* scene, const std::string& shadersDirectory);
        ~TiledRenderer();
        
        void Init();
        void Dispose();

        void Render();
        void Update(float secondsElapsed);
        float GetProgress() const;
        int GetSampleCount() const;

	private:
		GLuint pathTraceFBO;
		GLuint pathTraceFBOLowRes;
		GLuint accumFBO;
		GLuint outputFBO;

		Program* pathTraceShader;
		Program* pathTraceShaderLowRes;
		Program* accumShader;
		Program* tileOutputShader;
		Program* outputShader;

		GLuint pathTraceTexture;
		GLuint pathTraceTextureLowRes;
		GLuint accumTexture;
		GLuint tileOutputTexture[2];

		int tileX;
		int tileY;
		int numTilesX;
		int numTilesY;
		int tileWidth;
		int tileHeight;
		int currentBuffer;

		float sampleCounter;
		float totalTime;
    };
}
