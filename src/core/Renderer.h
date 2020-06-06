#pragma once

#include <vector>

#include "math/Vector2.h"

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
            resolution = Vector2(1280, 720);
			intensity  = 1.0f;
        }

        Vector2 resolution;
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

        const Vector2 getScreenSize() const 
		{ 
			return screenSize; 
		}

        virtual void Init();
        virtual void Dispose();

        virtual void Render() = 0;
        virtual void Update(float secondsElapsed);

        virtual float GetProgress() const = 0;
        virtual int GetSampleCount() const = 0;

	protected:
		GLuint bvhTex;
		GLuint aabbMinTex;
		GLuint aabbMaxTex;
		GLuint vertexIndicesTex;
		GLuint verticesTex;
		GLuint normalsTex;
		GLuint materialsTex;
		GLuint transformsTex;
		GLuint lightsTex;
		GLuint textureMapsArrayTex;
		GLuint hdrTex;
		GLuint hdrMarginalDistTex;
		GLuint hdrConditionalDistTex;

		bool initialized;

		Scene* scene;
		Quad *quad;
		int numOfLights;
		Vector2 screenSize;
		std::string shadersDirectory;
    };
}
