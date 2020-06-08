#include <iostream>
#include <algorithm>
#include <thread>

#include "Scene.h"
#include "Camera.h"

namespace GLSLPT
{
	Scene::Scene() 
		: hdrData(nullptr)
		, camera(nullptr)
		, sceneBvh(nullptr)
	{
		taskPool = new TaskThreadPool();
		taskPool->Create(std::max((int)std::thread::hardware_concurrency(), 8));
	}

	Scene::~Scene() 
	{ 
		if (camera) 
		{
			delete camera; 
			camera = nullptr;
		}

		if (sceneBvh)
		{
			delete sceneBvh; 
			sceneBvh = nullptr;
		}

		if (hdrData)
		{
			delete hdrData;
			hdrData = nullptr;
		}
		
		if (taskPool)
		{
			delete taskPool;
			taskPool = nullptr;
		}
	}

    void Scene::AddCamera(Vector3 pos, Vector3 lookAt, float fov)
    {
		if (camera) 
		{
			delete camera;
			camera = nullptr;
		}
        camera = new Camera(pos, lookAt, fov);
    }

	int Scene::AddMesh(Mesh* mesh)
	{
		int id = meshes.size();
		meshes.push_back(mesh);
		return id;
	}

	int Scene::AddMesh(const std::string& filename)
	{
		for (int i = 0; i < meshes.size(); ++i) 
		{
			if (meshes[i]->name == filename) {
				return i;
			}
		}

		int id = meshes.size();
		Mesh* mesh = new Mesh;
		mesh->name = filename;
		meshes.push_back(mesh);

		return id;
	}

	int Scene::AddTexture(const std::string& filename, unsigned char* data, int width, int height, int comp)
	{
		int id = textures.size();

		Texture* texture = new Texture();
		texture->name    = filename;
		texture->width   = width;
		texture->height  = height;
		texture->comp    = comp;
		texture->loaded  = true;

		texture->texData.resize(width * height * comp);
		std::copy(data, data + width * height * comp, texture->texData.begin());
		
		textures.push_back(texture);

		return id;
	}

	int Scene::AddTexture(const std::string& filename)
	{
		for (int i = 0; i < textures.size(); ++i)
		{
			if (textures[i]->name == filename) {
				return i;
			}
		}

		int id = textures.size();
		Texture* texture = new Texture();
		texture->name = filename;
		textures.push_back(texture);

		return id;
	}

	int Scene::AddMaterial(const Material& material)
	{
		int id = materials.size();
		materials.push_back(material);
		return id;
	}

	void Scene::AddHDR(const std::string& filename)
	{
		hdrFile = filename;

		if (hdrData)
		{
			delete hdrData;
			hdrData = nullptr;
		}
		
		hdrData = HDRLoader::Load(filename.c_str());
		if (hdrData == nullptr)
		{
			printf("Unable to load HDR\n");
		}
		else
		{
			printf("HDR %s loaded\n", filename.c_str());
			renderOptions.useEnvMap = true;
		}

		hdrModified = true;
	}
	
	int Scene::AddMeshInstance(const MeshInstance &meshInstance)
	{
		int id = meshInstances.size();
		meshInstances.push_back(meshInstance);
		return id;
	}

	int Scene::AddLight(const Light &light)
	{
		int id = lights.size();
		lights.push_back(light);
		return id;
	}

	void Scene::LoadAssets()
	{
		class MeshLoadJob : public ThreadTask
		{
		public:
			MeshLoadJob(Mesh* inMesh)
				: mesh(inMesh)
				, done(false)
			{

			}

			virtual void DoThreadedWork() override
			{
				mesh->LoadFromFile(mesh->name);
				done = true;
			}

			virtual void Abandon() override
			{

			}

			Mesh* mesh;
            bool done;
		};

		class TextureLoadJob : public ThreadTask
		{
		public:
			TextureLoadJob(Texture* inTexture)
				: texture(inTexture)
				, done(false)
			{

			}

			virtual void DoThreadedWork() override
			{
				texture->LoadTexture(texture->name);
				done = true;
			}

			virtual void Abandon() override
			{

			}
            
			Texture* texture;
            bool done;
		};

		printf("Loading assets ...\n");

		std::vector<MeshLoadJob*> meshJobs;
		for (int i = 0; i < meshes.size(); ++i) {
			if (!meshes[i]->loaded) 
			{
				MeshLoadJob* job = new MeshLoadJob(meshes[i]);
				meshJobs.push_back(job);
				taskPool->AddTask(job);
			}
		}

		std::vector<TextureLoadJob*> textureJobs;
		for (int i = 0; i < textures.size(); ++i) {
			if (!textures[i]->loaded)
			{
				TextureLoadJob* job = new TextureLoadJob(textures[i]);
				textureJobs.push_back(job);
				taskPool->AddTask(job);
			}
		}

		while (meshJobs.size() + textureJobs.size() > 0)
		{
			for (int i = meshJobs.size() - 1; i >= 0; --i)
			{
				if (meshJobs[i]->done)
				{
					printf("Mesh %s loaded.\n", meshJobs[i]->mesh->name.c_str());
					delete meshJobs[i];
					meshJobs.erase(meshJobs.begin() + i);
				}
			}

			for (int i = textureJobs.size() - 1; i >= 0; --i)
			{
				if (textureJobs[i]->done)
				{
					printf("Texture %s loaded.\n", textureJobs[i]->texture->name.c_str());
					delete textureJobs[i];
					textureJobs.erase(textureJobs.begin() + i);
				}
			}
		}

		printf("Scene assets loaded.\n");
	}

	void Scene::CreateTLAS()
	{
		// Loop through all the mesh Instances and build a Top Level BVH
		std::vector<Bounds3D> bounds;
		bounds.resize(meshInstances.size());

		for (int i = 0; i < meshInstances.size(); i++)
		{
			Bounds3D bbox = meshes[meshInstances[i].meshID]->bvh->Bounds();
			Matrix4x4 matrix = meshInstances[i].transform;

			Vector3 minBound = bbox.min;
			Vector3 maxBound = bbox.max;

			Vector3 right       = Vector3(matrix.m[0][0], matrix.m[0][1], matrix.m[0][2]);
			Vector3 up          = Vector3(matrix.m[1][0], matrix.m[1][1], matrix.m[1][2]);
			Vector3 forward     = Vector3(matrix.m[2][0], matrix.m[2][1], matrix.m[2][2]);
			Vector3 translation = Vector3(matrix.m[3][0], matrix.m[3][1], matrix.m[3][2]);

			Vector3 xa = right * minBound.x;
			Vector3 xb = right * maxBound.x;

			Vector3 ya = up * minBound.y;
			Vector3 yb = up * maxBound.y;

			Vector3 za = forward * minBound.z;
			Vector3 zb = forward * maxBound.z;

            minBound = Vector3::Min(xa, xb) + Vector3::Min(ya, yb) + Vector3::Min(za, zb) + translation;
            maxBound = Vector3::Max(xa, xb) + Vector3::Max(ya, yb) + Vector3::Max(za, zb) + translation;

			Bounds3D bound;
			bound.min = minBound;
			bound.max = maxBound;

			bounds[i] = bound;
		}

		if (sceneBvh)
		{
			delete sceneBvh;
			sceneBvh = nullptr;
		}
		sceneBvh = new RadeonRays::Bvh(10.0f, 64, false);
		sceneBvh->Build(&bounds[0], bounds.size());

		sceneBounds = sceneBvh->Bounds();
	}

	void Scene::CreateBLAS()
	{
		class BuildBVHJob : public ThreadTask
		{
		public:
			BuildBVHJob(Mesh* inMesh)
				: mesh(inMesh)
				, done(false)
			{

			}

			virtual void DoThreadedWork() override
			{
				mesh->BuildBVH();
				done = true;
			}

			virtual void Abandon() override
			{

			}

            Mesh* mesh;
			bool done;
		};

		printf("Building bottom level bvh...\n");

		// Loop through all meshes and build BVHs
		std::vector<BuildBVHJob*> jobs(meshes.size());
		for (int i = 0; i < meshes.size(); i++) {
			jobs[i] = new BuildBVHJob(meshes[i]);
			taskPool->AddTask(jobs[i]);
		}

		while (jobs.size() > 0)
		{
			for (int i = jobs.size() - 1; i >= 0; --i)
			{
				if (jobs[i]->done) 
				{
					printf("Mesh %s bvh build complete.\n", jobs[i]->mesh->name.c_str());
					delete jobs[i];
					jobs.erase(jobs.begin() + i);
				}
			}
		}
	}

	void Scene::Update(float deltaTime)
	{
		camera->Perspective(camera->GetFov(), renderOptions.frameSize.x, renderOptions.frameSize.y, camera->GetNear(), camera->GetFar());
		camera->Update(deltaTime);
	}
	
	void Scene::Resize(int wWidth, int wHeight, int fWidth, int fHeight)
	{
		renderOptions.windowSize.x = wWidth;
		renderOptions.windowSize.y = wHeight;
        
        renderOptions.frameSize.x  = fWidth;
        renderOptions.frameSize.y  = fHeight;
	}
	
	void Scene::RebuildInstancesData()
	{
		if (sceneBvh)
		{
			delete sceneBvh;
			sceneBvh = nullptr;
		}
		
		CreateTLAS();

		bvhTranslator.UpdateTLAS(sceneBvh, meshInstances);
		
		// Copy transforms
		for (int i = 0; i < meshInstances.size(); i++) 
		{
			transforms[i] = meshInstances[i].transform;
		}
		
		instancesModified = true;
	}

	void Scene::ValidateTextures()
	{
		if (textures.size() == 0) {
			return;
		}

		int width  = textures[0]->width;
		int height = textures[0]->height;

		for (int i = 0; i < textures.size(); ++i)
		{
			if (textures[i]->comp != 3) 
			{
				textures[i]->SetChannel(3);
			}
			if (textures[i]->width != width || textures[i]->height != height) 
			{
				textures[i]->Resize(width, height);
			}
		}

		texWidth  = width;
		texHeight = height;
	}

	void Scene::CreateAccelerationStructures()
	{
		LoadAssets();

		ValidateTextures();

		CreateBLAS();

		printf("Building scene BVH\n");
		CreateTLAS();

		// Flatten BVH
		bvhTranslator.Process(sceneBvh, meshes, meshInstances);

		int verticesCnt = 0;

		// Copy mesh data
		for (int i = 0; i < meshes.size(); i++)
		{
			// Copy indices from BVH and not from Mesh
			int numIndices = meshes[i]->bvh->GetNumIndices();
			const int * triIndices = meshes[i]->bvh->GetIndices();

			for (int j = 0; j < numIndices; j++)
			{
				int index = triIndices[j];
				int v1 = (index * 3 + 0) + verticesCnt;
				int v2 = (index * 3 + 1) + verticesCnt;
				int v3 = (index * 3 + 2) + verticesCnt;

				vertIndices.push_back(Indices{ v1, v2, v3 });
			}

			verticesUVX.insert(verticesUVX.end(), meshes[i]->verticesUVX.begin(), meshes[i]->verticesUVX.end());
			normalsUVY.insert(normalsUVY.end(), meshes[i]->normalsUVY.begin(), meshes[i]->normalsUVY.end());

			verticesCnt += meshes[i]->verticesUVX.size();
		}

		// Resize to power of 2
		indicesTexWidth = (int)(sqrt(vertIndices.size()) + 1); 
		triDataTexWidth = (int)(sqrt(verticesUVX.size())+ 1); 

		vertIndices.resize(indicesTexWidth * indicesTexWidth);
		verticesUVX.resize(triDataTexWidth * triDataTexWidth);
		normalsUVY.resize(triDataTexWidth * triDataTexWidth);

		for (int i = 0; i < vertIndices.size(); i++)
		{
			vertIndices[i].x = ((vertIndices[i].x % triDataTexWidth) << 12) | (vertIndices[i].x / triDataTexWidth);
			vertIndices[i].y = ((vertIndices[i].y % triDataTexWidth) << 12) | (vertIndices[i].y / triDataTexWidth);
			vertIndices[i].z = ((vertIndices[i].z % triDataTexWidth) << 12) | (vertIndices[i].z / triDataTexWidth);
		}

		// Copy transforms
		transforms.resize(meshInstances.size());
		for (int i = 0; i < meshInstances.size(); i++) 
		{
			transforms[i] = meshInstances[i].transform;
		}
		
		// Copy Textures
		for (int i = 0; i < textures.size(); i++)
		{
			textureMapsArray.insert(textureMapsArray.end(), textures[i]->texData.begin(), textures[i]->texData.end());
		}
	}
}
