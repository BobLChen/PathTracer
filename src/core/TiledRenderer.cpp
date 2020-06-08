#include "TiledRenderer.h"
#include "ShaderIncludes.h"
#include "Camera.h"
#include "Scene.h"

#include "glad/glad.h"

#include <string>

namespace GLSLPT
{
    TiledRenderer::TiledRenderer(Scene* scene, const std::string& shadersDirectory) 
		: Renderer(scene, shadersDirectory)
        , numTilesX(scene->renderOptions.numTilesX)
        , numTilesY(scene->renderOptions.numTilesY)
    {

    }

    TiledRenderer::~TiledRenderer() 
    {

    }

    void TiledRenderer::Init()
    {
		if (initialized) {
			return;
		}
        
        Renderer::Init();

		sampleCounter = 1;
		currentBuffer = 0;
		totalTime     = 0;
        
        Vector2 frameSize = scene->renderOptions.frameSize;
        
		tileWidth  = (int)frameSize.x / numTilesX;
		tileHeight = (int)frameSize.y / numTilesY;
		tileX      = -1;
		tileY      = numTilesY - 1;

		printf("Debug sizes : %d %d - %f %f\n", tileWidth, tileHeight, frameSize.x, frameSize.y);

        //----------------------------------------------------------
        // Shaders
        //----------------------------------------------------------
		pathTraceShader = LoadShaders(shadersDirectory + "common/Vertex.glsl", shadersDirectory + "Tiled.glsl");
		pathTraceShaderLowRes = LoadShaders(shadersDirectory + "common/Vertex.glsl", shadersDirectory + "Progressive.glsl");
		accumShader = LoadShaders(shadersDirectory + "common/Vertex.glsl", shadersDirectory + "Accumulation.glsl");
		tileOutputShader = LoadShaders(shadersDirectory + "common/Vertex.glsl", shadersDirectory + "TileOutput.glsl");
		outputShader = LoadShaders(shadersDirectory + "common/Vertex.glsl", shadersDirectory + "Output.glsl");

        //----------------------------------------------------------
        // FBO Setup
        //----------------------------------------------------------
	    // Create FBOs for path trace shader (Tiled)
		printf("Buffer pathTraceFBO\n");
		glGenFramebuffers(1, &pathTraceFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, pathTraceFBO);

		// Create Texture for FBO
		glGenTextures(1, &pathTraceTexture);
		glBindTexture(GL_TEXTURE_2D, pathTraceTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tileWidth, tileHeight, 0, GL_RGBA, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pathTraceTexture, 0);

		// Create FBOs for path trace shader (Progressive)
		printf("Buffer pathTraceFBOLowRes\n");
		glGenFramebuffers(1, &pathTraceFBOLowRes);
		glBindFramebuffer(GL_FRAMEBUFFER, pathTraceFBOLowRes);

		// Create Texture for FBO
		glGenTextures(1, &pathTraceTextureLowRes);
		glBindTexture(GL_TEXTURE_2D, pathTraceTextureLowRes);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, frameSize.x / 4, frameSize.y / 4, 0, GL_RGBA, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pathTraceTextureLowRes, 0);

		// Create FBOs for accum buffer
		printf("Buffer accumFBO\n");
		glGenFramebuffers(1, &accumFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, accumFBO);

		// Create Texture for FBO
		glGenTextures(1, &accumTexture);
		glBindTexture(GL_TEXTURE_2D, accumTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, frameSize.x, frameSize.y, 0, GL_RGBA, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumTexture, 0);

		// Create FBOs for tile output shader
		printf("Buffer outputFBO\n");
		glGenFramebuffers(1, &outputFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, outputFBO);

		// Create Texture for FBO
		glGenTextures(1, &tileOutputTexture[0]);
		glBindTexture(GL_TEXTURE_2D, tileOutputTexture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, frameSize.x, frameSize.y, 0, GL_RGBA, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenTextures(1, &tileOutputTexture[1]);
		glBindTexture(GL_TEXTURE_2D, tileOutputTexture[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, frameSize.x, frameSize.y, 0, GL_RGBA, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tileOutputTexture[currentBuffer], 0);

		GLuint shaderObject;

		{
			pathTraceShader->Active();
			shaderObject = pathTraceShader->Object();

			glUniform1f(glGetUniformLocation(shaderObject, "hdrResolution"), scene->hdrData == nullptr ? 0 : float(scene->hdrData->width * scene->hdrData->height));
			glUniform1i(glGetUniformLocation(shaderObject, "topBVHIndex"), scene->bvhTranslator.topLevelIndexPackedXY);
			glUniform1i(glGetUniformLocation(shaderObject, "vertIndicesSize"), scene->indicesTexWidth);
			glUniform2f(glGetUniformLocation(shaderObject, "screenResolution"), frameSize.x, frameSize.y);
			glUniform1i(glGetUniformLocation(shaderObject, "numOfLights"), numOfLights);
			glUniform1f(glGetUniformLocation(shaderObject, "invTileWidth"), 1.0f / numTilesX);
			glUniform1f(glGetUniformLocation(shaderObject, "invTileHeight"), 1.0f / numTilesY);
			glUniform1i(glGetUniformLocation(shaderObject, "accumTexture"), 0);
			glUniform1i(glGetUniformLocation(shaderObject, "BVH"), 1);
			glUniform1i(glGetUniformLocation(shaderObject, "BBoxMin"), 2);
			glUniform1i(glGetUniformLocation(shaderObject, "BBoxMax"), 3);
			glUniform1i(glGetUniformLocation(shaderObject, "vertexIndicesTex"), 4);
			glUniform1i(glGetUniformLocation(shaderObject, "verticesTex"), 5);
			glUniform1i(glGetUniformLocation(shaderObject, "normalsTex"), 6);
			glUniform1i(glGetUniformLocation(shaderObject, "materialsTex"), 7);
			glUniform1i(glGetUniformLocation(shaderObject, "transformsTex"), 8);
			glUniform1i(glGetUniformLocation(shaderObject, "lightsTex"), 9);
			glUniform1i(glGetUniformLocation(shaderObject, "textureMapsArrayTex"), 10);
			glUniform1i(glGetUniformLocation(shaderObject, "hdrTex"), 11);
			glUniform1i(glGetUniformLocation(shaderObject, "hdrMarginalDistTex"), 12);
			glUniform1i(glGetUniformLocation(shaderObject, "hdrCondDistTex"), 13);

			pathTraceShader->Deactive();
		}
		
		{
			pathTraceShaderLowRes->Active();
			shaderObject = pathTraceShaderLowRes->Object();

			glUniform1f(glGetUniformLocation(shaderObject, "hdrResolution"), scene->hdrData == nullptr ? 0 : float(scene->hdrData->width * scene->hdrData->height));
			glUniform1i(glGetUniformLocation(shaderObject, "topBVHIndex"), scene->bvhTranslator.topLevelIndexPackedXY);
			glUniform1i(glGetUniformLocation(shaderObject, "vertIndicesSize"), scene->indicesTexWidth);
			glUniform2f(glGetUniformLocation(shaderObject, "screenResolution"), frameSize.x, frameSize.y);
			glUniform1i(glGetUniformLocation(shaderObject, "numOfLights"), numOfLights);
			glUniform1i(glGetUniformLocation(shaderObject, "accumTexture"), 0);
			glUniform1i(glGetUniformLocation(shaderObject, "BVH"), 1);
			glUniform1i(glGetUniformLocation(shaderObject, "BBoxMin"), 2);
			glUniform1i(glGetUniformLocation(shaderObject, "BBoxMax"), 3);
			glUniform1i(glGetUniformLocation(shaderObject, "vertexIndicesTex"), 4);
			glUniform1i(glGetUniformLocation(shaderObject, "verticesTex"), 5);
			glUniform1i(glGetUniformLocation(shaderObject, "normalsTex"), 6);
			glUniform1i(glGetUniformLocation(shaderObject, "materialsTex"), 7);
			glUniform1i(glGetUniformLocation(shaderObject, "transformsTex"), 8);
			glUniform1i(glGetUniformLocation(shaderObject, "lightsTex"), 9);
			glUniform1i(glGetUniformLocation(shaderObject, "textureMapsArrayTex"), 10);
			glUniform1i(glGetUniformLocation(shaderObject, "hdrTex"), 11);
			glUniform1i(glGetUniformLocation(shaderObject, "hdrMarginalDistTex"), 12);
			glUniform1i(glGetUniformLocation(shaderObject, "hdrCondDistTex"), 13);

			pathTraceShaderLowRes->Deactive();
		}

		glActiveTexture(GL_TEXTURE1);
        bvhTex->Active();
		glActiveTexture(GL_TEXTURE2);
        aabbMinTex->Active();
		glActiveTexture(GL_TEXTURE3);
        aabbMaxTex->Active();
		glActiveTexture(GL_TEXTURE4);
        vertexIndicesTex->Active();
		glActiveTexture(GL_TEXTURE5);
        verticesTex->Active();
		glActiveTexture(GL_TEXTURE6);
        normalsTex->Active();
		glActiveTexture(GL_TEXTURE7);
        materialsTex->Active();
		glActiveTexture(GL_TEXTURE8);
        transformsTex->Active();
		glActiveTexture(GL_TEXTURE9);
        if (lightsTex) {
            lightsTex->Active();
        }
		glActiveTexture(GL_TEXTURE10);
        if (textureMapsArrayTex) {
            textureMapsArrayTex->Active();
        }
		glActiveTexture(GL_TEXTURE11);
        if (hdrTex) {
            hdrTex->Active();
        }
		glActiveTexture(GL_TEXTURE12);
        if (hdrMarginalDistTex) {
            hdrMarginalDistTex->Active();
        }
		glActiveTexture(GL_TEXTURE13);
        if (hdrConditionalDistTex) {
            hdrConditionalDistTex->Active();
        }
    }

    void TiledRenderer::Dispose()
    {
		if (!initialized) {
			return;
		}
        
		glDeleteTextures(1, &pathTraceTexture);
		glDeleteTextures(1, &pathTraceTextureLowRes);
		glDeleteTextures(1, &accumTexture);
		glDeleteTextures(1, &tileOutputTexture[0]);
		glDeleteTextures(1, &tileOutputTexture[1]);

		glDeleteFramebuffers(1, &pathTraceFBO);
		glDeleteFramebuffers(1, &pathTraceFBOLowRes);
		glDeleteFramebuffers(1, &accumFBO);
		glDeleteFramebuffers(1, &outputFBO);

		delete pathTraceShader;
		delete accumShader;
		delete tileOutputShader;
		delete outputShader;

        Renderer::Dispose();
    }

    void TiledRenderer::Render()
    {
        if (!initialized)
        {
            printf("Tiled Renderer is not initialized\n");
            return;
        }
        
        Vector2 frameSize = scene->renderOptions.frameSize;
        
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, accumTexture);

		if (!scene->camera->isMoving && !scene->instancesModified && !scene->hdrModified)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pathTraceFBO);
			glViewport(0, 0, tileWidth, tileHeight);
			quad->Draw(pathTraceShader);

			glBindFramebuffer(GL_FRAMEBUFFER, accumFBO);
			glViewport(tileWidth * tileX, tileHeight * tileY, tileWidth, tileHeight);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, pathTraceTexture);
			quad->Draw(accumShader);

			glBindFramebuffer(GL_FRAMEBUFFER, outputFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tileOutputTexture[currentBuffer], 0);
			glViewport(0, 0, frameSize.x, frameSize.y);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, accumTexture);
			quad->Draw(tileOutputShader);
            
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, frameSize.x, frameSize.y);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tileOutputTexture[1 - currentBuffer]);
			quad->Draw(outputShader);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pathTraceFBOLowRes);
			glViewport(0, 0, frameSize.x / 4, frameSize.y / 4);
			quad->Draw(pathTraceShaderLowRes);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, frameSize.x, frameSize.y);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, pathTraceTextureLowRes);
			quad->Draw(outputShader);
		}

		scene->hdrModified       = false;
		scene->instancesModified = false;
		scene->camera->isMoving  = false;
    }

    float TiledRenderer::GetProgress() const
    {
        return float((numTilesY - tileY - 1) * numTilesX + tileX) / float(numTilesX * numTilesY);
    }

	int TiledRenderer::GetSampleCount() const
	{
		return sampleCounter;
	}

    void TiledRenderer::Update(float secondsElapsed)
    {
		Renderer::Update(secondsElapsed);

		float r1;
		float r2;
		float r3;
		
        Vector2 frameSize = scene->renderOptions.frameSize;
        
		if (scene->camera->isMoving || scene->instancesModified || scene->hdrModified)
		{
			r1 = r2 = r3 = 0;
			tileX = -1;
			tileY = numTilesY - 1;
			sampleCounter = 1;

			glBindFramebuffer(GL_FRAMEBUFFER, accumFBO);
			glViewport(0, 0, frameSize.x, frameSize.y);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindFramebuffer(GL_FRAMEBUFFER, outputFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tileOutputTexture[1 - currentBuffer], 0);
			glViewport(0, 0, frameSize.x, frameSize.y);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, pathTraceTextureLowRes);
			quad->Draw(accumShader);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else
		{
			tileX++;
			if (tileX >= numTilesX)
			{
				tileX = 0;
				tileY--;
				if (tileY < 0)
				{
					tileX = 0;
					tileY = numTilesY - 1;
					sampleCounter++;
					currentBuffer = 1 - currentBuffer;
				}
			}

			r1 = ((float)rand() / (RAND_MAX));
			r2 = ((float)rand() / (RAND_MAX));
			r3 = ((float)rand() / (RAND_MAX));
		}

		GLuint shaderObject;

		{
			pathTraceShader->Active();
			shaderObject = pathTraceShader->Object();
			glUniform3f(glGetUniformLocation(shaderObject, "camera.position"),
                scene->camera->GetPosition().x,
                scene->camera->GetPosition().y,
                scene->camera->GetPosition().z
            );
			glUniform3f(glGetUniformLocation(shaderObject, "camera.right"),
                scene->camera->GetLeft().x,
                scene->camera->GetLeft().y,
                scene->camera->GetLeft().z
            );
			glUniform3f(glGetUniformLocation(shaderObject, "camera.up"),
                scene->camera->GetUp().x,
                scene->camera->GetUp().y,
                scene->camera->GetUp().z
            );
			glUniform3f(glGetUniformLocation(shaderObject, "camera.forward"),
                scene->camera->GetForward().x,
                scene->camera->GetForward().y,
                scene->camera->GetForward().z
            );
			glUniform1f(glGetUniformLocation(shaderObject, "camera.fov"), scene->camera->GetFov());
			glUniform1f(glGetUniformLocation(shaderObject, "camera.focalDist"), scene->camera->focalDist);
			glUniform1f(glGetUniformLocation(shaderObject, "camera.aperture"), scene->camera->aperture);
			glUniform3f(glGetUniformLocation(shaderObject, "randomVector"), r1, r2, r3);
			glUniform1i(glGetUniformLocation(shaderObject, "useEnvMap"), scene->hdrData == nullptr ? false : scene->renderOptions.useEnvMap);
			glUniform1f(glGetUniformLocation(shaderObject, "hdrMultiplier"), scene->renderOptions.intensity);
			glUniform1i(glGetUniformLocation(shaderObject, "maxDepth"), scene->camera->isMoving || scene->instancesModified ? 2 : scene->renderOptions.maxDepth);
			glUniform1i(glGetUniformLocation(shaderObject, "tileX"), tileX);
			glUniform1i(glGetUniformLocation(shaderObject, "tileY"), tileY);
			pathTraceShader->Deactive();
		}

		{
			pathTraceShaderLowRes->Active();
			shaderObject = pathTraceShaderLowRes->Object();
			glUniform3f(glGetUniformLocation(shaderObject, "camera.position"),
				scene->camera->GetPosition().x,
				scene->camera->GetPosition().y,
				scene->camera->GetPosition().z
			);
			glUniform3f(glGetUniformLocation(shaderObject, "camera.right"),
				scene->camera->GetLeft().x,
				scene->camera->GetLeft().y,
				scene->camera->GetLeft().z
			);
			glUniform3f(glGetUniformLocation(shaderObject, "camera.up"),
				scene->camera->GetUp().x,
				scene->camera->GetUp().y,
				scene->camera->GetUp().z
			);
			glUniform3f(glGetUniformLocation(shaderObject, "camera.forward"),
				scene->camera->GetForward().x,
				scene->camera->GetForward().y,
				scene->camera->GetForward().z
			);
			glUniform1f(glGetUniformLocation(shaderObject, "camera.fov"), scene->camera->GetFov());
			glUniform1f(glGetUniformLocation(shaderObject, "camera.focalDist"), scene->camera->focalDist);
			glUniform1f(glGetUniformLocation(shaderObject, "camera.aperture"), scene->camera->aperture);
			glUniform3f(glGetUniformLocation(shaderObject, "randomVector"), r1, r2, r3);
			glUniform1i(glGetUniformLocation(shaderObject, "useEnvMap"), scene->hdrData == nullptr ? false : scene->renderOptions.useEnvMap);
			glUniform1f(glGetUniformLocation(shaderObject, "hdrMultiplier"), scene->renderOptions.intensity);
			glUniform1i(glGetUniformLocation(shaderObject, "maxDepth"), scene->camera->isMoving || scene->instancesModified ? 2: scene->renderOptions.maxDepth);
			pathTraceShaderLowRes->Deactive();
		}

		{
			outputShader->Active();
			shaderObject = outputShader->Object();
			glUniform1f(glGetUniformLocation(shaderObject, "invSampleCounter"), scene->camera->isMoving ? 1.0f : 1.0f / sampleCounter);
			outputShader->Deactive();
		}
    }
}
