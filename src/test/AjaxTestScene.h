#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "core/Scene.h"

namespace GLSLPT
{
	void LoadAjaxTestScene(const std::string& rootPath, Scene* scene, RenderOptions &renderOptions)
	{
		renderOptions.maxDepth = 5;
		renderOptions.numTilesY = 5;
		renderOptions.numTilesX = 5;
		renderOptions.hdrMultiplier = 5.0f;
		renderOptions.useEnvMap = true;
		scene->AddCamera(glm::vec3(0.0f, 0.125f, -0.45f), glm::vec3(0.0f, 0.125f, 0.0f), 60.0f);

		int mesh_id = scene->AddMesh(rootPath + "assets/ajax/ajax.obj");

		Material black;
		black.albedo = glm::vec3(0.1f, 0.1f, 0.1f);
		black.roughness = 0.01f;
		black.metallic = 1.0f;

		Material red_plastic;
		red_plastic.albedo = glm::vec3(1.0, 0.0, 0.0);
		red_plastic.roughness = 0.01;
		red_plastic.metallic = 0.0;

		Material gold;
		gold.albedo = glm::vec3(1.0, 0.71, 0.29);
		gold.roughness = 0.2;
		gold.metallic = 1.0;

		int black_mat_id = scene->AddMaterial(black);
		int red_mat_id = scene->AddMaterial(red_plastic);
		int gold_mat_id = scene->AddMaterial(gold);

		glm::mat4 xform;
		glm::mat4 xform1;
		glm::mat4 xform2;

		xform = glm::scale(glm::vec3(0.25f));
		xform1 = glm::scale(glm::vec3(0.25f)) * glm::translate(glm::vec3(0.6f, 0.0f, 0.0f));
		xform2 = glm::scale(glm::vec3(0.25f)) * glm::translate(glm::vec3(-0.6f, 0.0f, 0.0f));
		
		MeshInstance instance(mesh_id, xform, black_mat_id);

		scene->AddMeshInstance(instance);

		scene->AddHDR(rootPath + "assets/HDR/sunset.hdr");

		scene->CreateAccelerationStructures();
	}
}