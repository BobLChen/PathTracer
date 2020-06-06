#pragma once

#include "math/Matrix4x4.h"
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

#include "core/Scene.h"

namespace GLSLPT
{
	void LoadCornellTestScene(const std::string& rootPath, Scene* scene, RenderOptions &renderOptions)
	{
		renderOptions.maxDepth  = 5;
		renderOptions.numTilesY = 5;
		renderOptions.numTilesX = 5;
		renderOptions.hdrMultiplier = 1.0f;
		renderOptions.useEnvMap = true;
		scene->AddCamera(Vector3(0.276f, 0.275f, -0.75f), Vector3(0.276f, 0.275f, 0), 65.0f);

		int ceiling_mesh_id   = scene->AddMesh(rootPath + "assets/cornell_box/cbox_ceiling.obj");
		int floor_mesh_id     = scene->AddMesh(rootPath + "assets/cornell_box/cbox_floor.obj");
		int back_mesh_id      = scene->AddMesh(rootPath + "assets/cornell_box/cbox_back.obj");
		int greenwall_mesh_id = scene->AddMesh(rootPath + "assets/cornell_box/cbox_greenwall.obj");
		int largebox_mesh_id  = scene->AddMesh(rootPath + "assets/cornell_box/cbox_largebox.obj");
		int redwall_mesh_id   = scene->AddMesh(rootPath + "assets/cornell_box/cbox_redwall.obj");
		int smallbox_mesh_id  = scene->AddMesh(rootPath + "assets/cornell_box/cbox_smallbox.obj");

		Material white;
		white.albedo = Vector3(0.725f, 0.71f, 0.68f);

		Material red;
		red.albedo = Vector3(0.63f, 0.065f, 0.05f);

		Material green;
		green.albedo = Vector3(0.14f, 0.45f, 0.091f);

		int white_mat_id = scene->AddMaterial(white);
		int red_mat_id = scene->AddMaterial(red);
		int green_mat_id = scene->AddMaterial(green);

		Light light;
		light.type = LightType::QuadLight;
		light.position = Vector3(.34299999f, .54779997f, .22700010f);
		light.u = Vector3(.34299999f, .54779997f, .33200008f) - light.position;
		light.v = Vector3(.21300001f, .54779997f, .22700010f) - light.position;
        light.area = Vector3::CrossProduct(light.u, light.v).Size();
		light.emission = Vector3(17, 12, 4);

		scene->AddLight(light);

        Matrix4x4 xform;
        xform.SetScale(Vector3(0.01f, 0.01f, 0.01f), 1.0f);
		
		MeshInstance instance1(ceiling_mesh_id, xform, white_mat_id);
		MeshInstance instance2(floor_mesh_id, xform, white_mat_id);
		MeshInstance instance3(back_mesh_id, xform, white_mat_id);
		MeshInstance instance4(greenwall_mesh_id, xform, green_mat_id);
		MeshInstance instance5(largebox_mesh_id, xform, white_mat_id);
		MeshInstance instance6(redwall_mesh_id, xform, red_mat_id);
		MeshInstance instance7(smallbox_mesh_id, xform, white_mat_id);

		scene->AddMeshInstance(instance1);
		scene->AddMeshInstance(instance2);
		scene->AddMeshInstance(instance3);
		scene->AddMeshInstance(instance4);
		scene->AddMeshInstance(instance5);
		scene->AddMeshInstance(instance6);
		scene->AddMeshInstance(instance7);

		scene->AddHDR(rootPath + "assets/HDR/sunset.hdr");

		scene->CreateAccelerationStructures();

	}
}
