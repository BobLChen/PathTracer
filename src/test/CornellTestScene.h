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
		renderOptions.maxDepth  = 6;
		renderOptions.numTilesY = 4;
		renderOptions.numTilesX = 4;
		renderOptions.intensity = 1.0f;
		renderOptions.useEnvMap = true;

		// scene->AddHDR(rootPath + "assets/HDR/photo_studio_01_1k.hdr");
		scene->AddCamera(Vector3(0.276f, 0.275f, -0.75f), Vector3(0.276f, 0.275f, 0), 65.0f);

		int ceilingMeshID   = scene->AddMesh(rootPath + "assets/cornell_box/cbox_ceiling.obj");
		int floorMeshID     = scene->AddMesh(rootPath + "assets/cornell_box/cbox_floor.obj");
		int backMeshID      = scene->AddMesh(rootPath + "assets/cornell_box/cbox_back.obj");
		int greenWallMeshID = scene->AddMesh(rootPath + "assets/cornell_box/cbox_greenwall.obj");
		int largeBoxMeshID  = scene->AddMesh(rootPath + "assets/cornell_box/cbox_largebox.obj");
		int redWallMeshID   = scene->AddMesh(rootPath + "assets/cornell_box/cbox_redwall.obj");
		int smallBoxMeshID  = scene->AddMesh(rootPath + "assets/cornell_box/cbox_smallbox.obj");

		Material white;
		white.albedo = Vector3(0.725f, 0.71f, 0.68f);

		Material red;
		red.albedo = Vector3(0.63f, 0.065f, 0.05f);

		Material green;
		green.albedo = Vector3(0.14f, 0.45f, 0.091f);

		int whiteMatID = scene->AddMaterial(white);
		int redMatID   = scene->AddMaterial(red);
		int greenMatID = scene->AddMaterial(green);

		Light light;
		light.type     = LightType::QuadLight;
		light.position = Vector3(.34299999f, .54779997f, .22700010f);
		light.u        = Vector3(.34299999f, .54779997f, .33200008f) - light.position;
		light.v        = Vector3(.21300001f, .54779997f, .22700010f) - light.position;
        light.area     = Vector3::CrossProduct(light.u, light.v).Size();
		light.emission = Vector3(17, 12, 4);
		scene->AddLight(light);

        Matrix4x4 xform;
        xform.SetScale(Vector3(0.01f, 0.01f, 0.01f), 1.0f);
		
		MeshInstance instance1(ceilingMeshID,   xform, whiteMatID, "ceilling");
		MeshInstance instance2(floorMeshID,     xform, whiteMatID, "floor");
		MeshInstance instance3(backMeshID,      xform, whiteMatID, "back");
		MeshInstance instance4(greenWallMeshID, xform, greenMatID, "green wall");
		MeshInstance instance5(largeBoxMeshID,  xform, whiteMatID, "large box");
		MeshInstance instance6(redWallMeshID,   xform, redMatID,   "red wall");
		MeshInstance instance7(smallBoxMeshID,  xform, whiteMatID, "small box");

		scene->AddMeshInstance(instance1);
		scene->AddMeshInstance(instance2);
		scene->AddMeshInstance(instance3);
		scene->AddMeshInstance(instance4);
		scene->AddMeshInstance(instance5);
		scene->AddMeshInstance(instance6);
		scene->AddMeshInstance(instance7);

		scene->CreateAccelerationStructures();
	}
}
