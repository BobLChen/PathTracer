#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "core/Scene.h"

namespace GLSLPT
{
	void LoadBoyTestScene(const std::string& rootPath, Scene* scene, RenderOptions &renderOptions)
	{
		renderOptions.maxDepth = 2;
		renderOptions.numTilesY = 5;
		renderOptions.numTilesX = 5;
		renderOptions.hdrMultiplier = 1.0f;
		renderOptions.useEnvMap = false;
		scene->AddCamera(glm::vec3(0.3f, 0.11f, 0.0f), glm::vec3(0.2f, 0.095f, 0.0f), 60.0f);
		scene->camera->aperture = 1e-6f; // 0.000001f;
		scene->camera->focalDist = 0.262f;

		int mesh_id1 = scene->AddMesh(rootPath + "assets/Figurine/head.obj");
		int mesh_id2 = scene->AddMesh(rootPath + "assets/Figurine/body.obj");
		int mesh_id3 = scene->AddMesh(rootPath + "assets/Figurine/base.obj");
		int mesh_id4 = scene->AddMesh(rootPath + "assets/Figurine/background.obj");

		Material head;
		Material body;
		Material base;
		Material white;
		Material gold;
		Material red_plastic;

		int headAlbedo = scene->AddTexture(rootPath + "assets/Figurine/textures/01_Head_Base_Color.png");
		int bodyAlbedo = scene->AddTexture(rootPath + "assets/Figurine/textures/02_Body_Base_Color.png");
		int baseAlbedo = scene->AddTexture(rootPath + "assets/Figurine/textures/03_Base_Base_Color.png");
		int bgAlbedo   = scene->AddTexture(rootPath + "assets/Figurine/textures/grid.jpg");

		int headMatRgh = scene->AddTexture(rootPath + "assets/Figurine/textures/01_Head_MetallicRoughness.png");
		int bodyMatRgh = scene->AddTexture(rootPath + "assets/Figurine/textures/02_Body_MetallicRoughness.png");
		int baseMatRgh = scene->AddTexture(rootPath + "assets/Figurine/textures/03_Base_MetallicRoughness.png");

		head.albedoTexID = headAlbedo;
		head.metallicRoughnessTexID = headMatRgh;
		
		body.albedoTexID = bodyAlbedo;
		body.metallicRoughnessTexID = bodyMatRgh;

		base.albedoTexID = baseAlbedo;
		base.metallicRoughnessTexID = baseMatRgh;

		white.albedoTexID = bgAlbedo;

		gold.albedo = glm::vec3(1.0, 0.71, 0.29);
		gold.roughness = 0.2;
		gold.metallic = 1.0;

		red_plastic.albedo = glm::vec3(1.0, 0.0, 0.0);
		red_plastic.roughness = 0.01;
		red_plastic.metallic = 0.0;

		int head_mat_id = scene->AddMaterial(head);
		int body_mat_id = scene->AddMaterial(body);
		int base_mat_id = scene->AddMaterial(base);
		int white_mat_id = scene->AddMaterial(white);
		int gold_mat_id = scene->AddMaterial(gold);
		int red_mat_id = scene->AddMaterial(red_plastic);

		Light light;
		light.type = LightType::QuadLight;
		light.position = glm::vec3(-0.103555f, 0.284840f, 0.606827f);
		light.u = glm::vec3(-0.103555f, 0.465656f, 0.521355f) - light.position;
		light.v = glm::vec3(0.096445f, 0.284840f, 0.606827f) - light.position;
		light.area = glm::length(glm::cross(light.u, light.v));
		light.emission = glm::vec3(40, 41, 41);

		Light light2;
		light2.type = LightType::QuadLight;
		light2.position = glm::vec3(0.303145f, 0.461806f, -0.450967f);
		light2.u = glm::vec3(0.362568f, 0.280251f, -0.510182f) - light2.position;
		light2.v = glm::vec3(0.447143f, 0.461806f, -0.306465f) - light2.position;
		light2.area = glm::length(glm::cross(light2.u, light2.v));
		light2.emission = glm::vec3(40, 41, 41);

		int light1_id = scene->AddLight(light);
		int light2_id = scene->AddLight(light2);

		glm::mat4 xform_base = glm::translate(glm::vec3(0, 0.0075, 0));
		glm::mat4 xform_body = glm::translate(glm::vec3(0, 0.049, 0));
		glm::mat4 xform_head = glm::translate(glm::vec3(0.017, 0.107, 0));

		glm::mat4 xform1;
		glm::mat4 xform2;
		glm::mat4 xform3;
		glm::mat4 xform4;
		glm::mat4 xform5;

		xform2 = glm::translate(glm::vec3(0, 0, -0.05));
		xform3 = glm::translate(glm::vec3(0, 0,  0.05));
		xform4 = glm::translate(glm::vec3(-0.1, 0.0, -0.15));
		xform5 = glm::translate(glm::vec3(-0.1, 0, 0.15));

		MeshInstance instance1(mesh_id4, xform1, white_mat_id);

		MeshInstance instance2(mesh_id1, xform_head * xform2, head_mat_id);
		MeshInstance instance3(mesh_id2, xform_body * xform2, body_mat_id);
		MeshInstance instance4(mesh_id3, xform_base * xform2, base_mat_id);
		
		MeshInstance instance5(mesh_id1, xform_head * xform3, head_mat_id);
		MeshInstance instance6(mesh_id2, xform_body * xform3, body_mat_id);
		MeshInstance instance7(mesh_id3, xform_base * xform3, base_mat_id);

		MeshInstance instance8(mesh_id1, xform_head * xform4, head_mat_id);
		MeshInstance instance9(mesh_id2, xform_body * xform4, body_mat_id);
		MeshInstance instance10(mesh_id3, xform_base * xform4, base_mat_id);

		MeshInstance instance11(mesh_id1, xform_head * xform5, head_mat_id);
		MeshInstance instance12(mesh_id2, xform_body * xform5, body_mat_id);
		MeshInstance instance13(mesh_id3, xform_base * xform5, base_mat_id);

		scene->AddMeshInstance(instance1);
		scene->AddMeshInstance(instance2);
		scene->AddMeshInstance(instance3);
		scene->AddMeshInstance(instance4);
		scene->AddMeshInstance(instance5);
		scene->AddMeshInstance(instance6);
		scene->AddMeshInstance(instance7);
		scene->AddMeshInstance(instance8);
		scene->AddMeshInstance(instance9);
		scene->AddMeshInstance(instance10);
		scene->AddMeshInstance(instance11);
		scene->AddMeshInstance(instance12);
		scene->AddMeshInstance(instance13);

		scene->CreateAccelerationStructures();
	}
}