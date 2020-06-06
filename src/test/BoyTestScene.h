#pragma once

#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/Matrix4x4.h"

#include "core/Scene.h"

namespace GLSLPT
{
	void LoadBoyTestScene(const std::string& rootPath, Scene* scene, RenderOptions &renderOptions)
	{
		renderOptions.maxDepth  = 6;
		renderOptions.numTilesY = 4;
		renderOptions.numTilesX = 4;
		renderOptions.intensity = 1.0f;
		renderOptions.useEnvMap = false;

		scene->AddCamera(Vector3(0.3f, 0.11f, 0.0f), Vector3(0.2f, 0.095f, 0.0f), 60.0f);
		scene->camera->aperture  = 0.000001f;
		scene->camera->focalDist = 0.262f;

		int meshID1 = scene->AddMesh(rootPath + "assets/Figurine/head.obj");
		int meshID2 = scene->AddMesh(rootPath + "assets/Figurine/body.obj");
		int meshID3 = scene->AddMesh(rootPath + "assets/Figurine/base.obj");
		int meshID4 = scene->AddMesh(rootPath + "assets/Figurine/background.obj");

		Material head;
		Material body;
		Material base;
		Material white;
		Material gold;
		Material redPlastic;

		int headAlbedo = scene->AddTexture(rootPath + "assets/Figurine/textures/01_Head_Base_Color.jpg");
		int bodyAlbedo = scene->AddTexture(rootPath + "assets/Figurine/textures/02_Body_Base_Color.jpg");
		int baseAlbedo = scene->AddTexture(rootPath + "assets/Figurine/textures/03_Base_Base_Color.jpg");
		int bgAlbedo   = scene->AddTexture(rootPath + "assets/Figurine/textures/grid.jpg");

		int headMatRgh = scene->AddTexture(rootPath + "assets/Figurine/textures/01_Head_MetallicRoughness.jpg");
		int bodyMatRgh = scene->AddTexture(rootPath + "assets/Figurine/textures/02_Body_MetallicRoughness.jpg");
		int baseMatRgh = scene->AddTexture(rootPath + "assets/Figurine/textures/03_Base_MetallicRoughness.jpg");

		head.albedoTexID = headAlbedo;
		head.paramsTexID = headMatRgh;
		
		body.albedoTexID = bodyAlbedo;
		body.paramsTexID = bodyMatRgh;

		base.albedoTexID = baseAlbedo;
		base.paramsTexID = baseMatRgh;

		white.albedoTexID = bgAlbedo;

		gold.albedo    = Vector3(1.0, 0.71, 0.29);
		gold.roughness = 0.2;
		gold.metallic  = 1.0;

		redPlastic.albedo    = Vector3(1.0, 0.0, 0.0);
		redPlastic.roughness = 0.01;
		redPlastic.metallic  = 0.0;

		int headMatID  = scene->AddMaterial(head);
		int bodyMatID  = scene->AddMaterial(body);
		int baseMatID  = scene->AddMaterial(base);
		int whiteMatID = scene->AddMaterial(white);
		int goldMatID  = scene->AddMaterial(gold);
		int redMatID   = scene->AddMaterial(redPlastic);

		Light light;
		light.type     = LightType::QuadLight;
		light.position = Vector3(-0.103555f, 0.284840f, 0.606827f);
		light.u        = Vector3(-0.103555f, 0.465656f, 0.521355f) - light.position;
		light.v        = Vector3(0.096445f, 0.284840f, 0.606827f) - light.position;
		light.area     = Vector3::CrossProduct(light.u, light.v).Size();
		light.emission = Vector3(40, 41, 41);
        
		Light light2;
		light2.type     = LightType::QuadLight;
		light2.position = Vector3(0.303145f, 0.461806f, -0.450967f);
		light2.u        = Vector3(0.362568f, 0.280251f, -0.510182f) - light2.position;
		light2.v        = Vector3(0.447143f, 0.461806f, -0.306465f) - light2.position;
		light2.area     = Vector3::CrossProduct(light2.u, light2.v).Size();
		light2.emission = Vector3(40, 41, 41);
        
		scene->AddLight(light);
		scene->AddLight(light2);

        Matrix4x4 xformBase;
        xformBase.SetPosition(Vector3(0, 0.0075, 0));
        Matrix4x4 xformBody;
        xformBody.SetPosition(Vector3(0, 0.049, 0));
		Matrix4x4 xformHead;
        xformHead.SetPosition(Vector3(0.017, 0.107, 0));

		Matrix4x4 xform1;
		Matrix4x4 xform2;
        xform2.SetPosition(Vector3(0, 0, -0.05));
		Matrix4x4 xform3;
        xform3.SetPosition(Vector3(0, 0,  0.05));
		Matrix4x4 xform4;
        xform4.SetPosition(Vector3(-0.1, 0.0, -0.15));
		Matrix4x4 xform5;
        xform5.SetPosition(Vector3(-0.1, 0, 0.15));
        
		MeshInstance instance1(meshID4, xform1, whiteMatID, "instance1");

		MeshInstance instance2(meshID1, xformHead * xform2, headMatID, "head0");
		MeshInstance instance3(meshID2, xformBody * xform2, bodyMatID, "body0");
		MeshInstance instance4(meshID3, xformBase * xform2, baseMatID, "base0");
		
		MeshInstance instance5(meshID1, xformHead * xform3, headMatID, "head1");
		MeshInstance instance6(meshID2, xformBody * xform3, bodyMatID, "body1");
		MeshInstance instance7(meshID3, xformBase * xform3, baseMatID, "base1");

		MeshInstance instance8(meshID1, xformHead * xform4, headMatID, "head2");
		MeshInstance instance9(meshID2, xformBody * xform4, bodyMatID, "body2");
		MeshInstance instance10(meshID3, xformBase * xform4, baseMatID, "base2");

		MeshInstance instance11(meshID1, xformHead * xform5, headMatID, "head3");
		MeshInstance instance12(meshID2, xformBody * xform5, bodyMatID, "body3");
		MeshInstance instance13(meshID3, xformBase * xform5, baseMatID, "base3");
        
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
