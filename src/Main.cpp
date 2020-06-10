#include <time.h>
#include <math.h>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "core/Scene.h"
#include "core/Renderer.h"
#include "core/TiledRenderer.h"

#include "parser/SceneLoader.h"
#include "parser/GLBLoader.h"

#include "test/BoyTestScene.h"
#include "test/CornellTestScene.h"

#include "file/tinydir.h"

using namespace GLSLPT;

int				sampleSceneIndex = -1;
int				envMapIndex = 0;
int				selectedInstance = 0;
double			lastTime = 0;
std::string		shaderDir;
std::string		assetsDir;
std::string     hdrResDir;

Scene*			scene = nullptr;
Renderer*		renderer = nullptr;
GLFWwindow*		glfwWindow = NULL;
RenderOptions	renderOptions;

std::vector<std::string> sceneFiles;
std::vector<std::string> sceneNames;
std::vector<std::string> envFiles;
std::vector<std::string> envNames;

void LoadScene(const std::string& file)
{
	if (scene) 
	{
		delete scene;
		scene = nullptr;
	}
	scene = new Scene();
	
	std::string ext = file.substr(file.find_last_of(".") + 1);

	bool useGLB = false;
	if (ext == "glb")
	{
		useGLB = true;
		LoadSceneFromGLTF(file.c_str(), scene);
	}
	else if (ext == "scene")
	{
		LoadSceneFromFile(file.c_str(), scene, renderOptions);
	}

	if (scene->hdrData == nullptr)
	{
		scene->AddHDR(assetsDir + "HDR/vignaioli_night_1k.hdr");
		renderOptions.useEnvMap = useGLB;
	}

	envMapIndex = 0;
	for (int i = 0; i < envFiles.size(); ++i)
	{
		if (envFiles[i] == scene->hdrFile) 
		{
			envMapIndex = i;
			break;
		}
	}

	scene->renderOptions = renderOptions;
}

bool InitRenderer()
{
	if (renderer) 
	{
		delete renderer;
		renderer = nullptr;
	}
    
    renderer = new TiledRenderer(scene, shaderDir);
    renderer->Init();
    
    return true;
}

void Render(float deltaTime)
{
	auto io = ImGui::GetIO();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	renderer->Render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Update(float deltaTime)
{
	ImVec2 mousePos = ImGui::GetMousePos();
	scene->camera->OnMousePos(Vector2(mousePos.x, mousePos.y));

	if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) &&
		(ImGui::IsMouseDown(GLFW_MOUSE_BUTTON_RIGHT) || ImGui::IsMouseDown(GLFW_MOUSE_BUTTON_MIDDLE) || ImGui::GetIO().MouseWheel != 0.0f) && 
		!ImGuizmo::IsOver()
	)
	{
		scene->camera->OnRMouse(ImGui::IsMouseDown(GLFW_MOUSE_BUTTON_RIGHT));
		scene->camera->OnMMouse(ImGui::IsMouseDown(GLFW_MOUSE_BUTTON_MIDDLE));
		scene->camera->OnMouseWheel(ImGui::GetIO().MouseWheel);
	}
	else
	{
		scene->camera->OnRMouse(false);
		scene->camera->OnMMouse(false);
		scene->camera->OnMouseWheel(0.0f);
	}

	scene->Update(deltaTime);
	renderer->Update(deltaTime);
}

void EditTransform(const float* view, const float* proj, Matrix4x4& matrix)
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE      mCurrentGizmoMode(ImGuizmo::WORLD);

	if (ImGui::IsKeyPressed(GLFW_KEY_W)) 
	{
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_E)) 
	{
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_R)) 
	{
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	}

	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE)) 
	{
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	}

	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE)) 
	{
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	}

	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE)) 
	{
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	}

	Vector3 pos = matrix.GetOrigin();
	Vector3 rot = matrix.GetRotation();
	Vector3 sca = matrix.GetScale();

	ImGui::InputFloat3("Tr", (float*)&pos);
	ImGui::InputFloat3("Rt", (float*)&rot);
	ImGui::InputFloat3("Sc", (float*)&sca);

	matrix.SetPosition(pos);
	matrix.SetRotation(rot);
	matrix.SetScale(sca);

	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL)) 
		{
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		}

		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD)) 
		{
			mCurrentGizmoMode = ImGuizmo::WORLD;
		}
	}
	
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate(view, proj, mCurrentGizmoOperation, mCurrentGizmoMode, (float*)&matrix, NULL, NULL);
}

void OnGUI(float deltaTime)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::SetOrthographic(true);
	ImGuizmo::BeginFrame();

	ImGui::Begin("Settings");
	ImGui::Text("Samples: %d ", renderer->GetSampleCount());

	std::vector<const char*> sceneItems;
	for (int i = 0; i < sceneNames.size(); ++i) {
		sceneItems.push_back(sceneNames[i].c_str());
	}
	if (ImGui::Combo("Scene", &sampleSceneIndex, sceneItems.data(), sceneItems.size()))
	{
		LoadScene(sceneFiles[sampleSceneIndex]);
		glfwSetWindowSize(glfwWindow, scene->renderOptions.windowSize.x, scene->renderOptions.windowSize.y);
		InitRenderer();
	}

	std::vector<const char*> envItems;
	for (int i = 0; i < envNames.size(); ++i) {
		envItems.push_back(envNames[i].c_str());
	}

	if (ImGui::Combo("EnvMap", &envMapIndex, envItems.data(), envItems.size()))
	{
		scene->AddHDR(envFiles[envMapIndex]);
	}

	bool optionsChanged = false;
	if (ImGui::CollapsingHeader("Render Settings"))
	{
		optionsChanged |= ImGui::SliderInt("Max Depth", &renderOptions.maxDepth, 1, 10);
		optionsChanged |= ImGui::SliderInt("NumTilesX", &renderOptions.numTilesX, 1, 32);
		optionsChanged |= ImGui::SliderInt("NumTilesY", &renderOptions.numTilesY, 1, 32);
		optionsChanged |= ImGui::Checkbox("Use envmap", &renderOptions.useEnvMap);
		optionsChanged |= ImGui::SliderFloat("HDR multiplier", &renderOptions.intensity, 0.1, 10);
	}

	if (ImGui::CollapsingHeader("Camera"))
	{
        float fov = MMath::RadiansToDegrees(scene->camera->GetFov());
		optionsChanged |= ImGui::SliderFloat("Fov", &fov, 10, 90);
		scene->camera->SetFov(MMath::DegreesToRadians(fov));

		float aperture = scene->camera->aperture * 1000.0f;
		optionsChanged |= ImGui::SliderFloat("Aperture", &aperture, 0.0f, 50.0f);
		scene->camera->aperture = aperture / 1000.0f;

		optionsChanged |= ImGui::SliderFloat("Focal Distance", &scene->camera->focalDist, 0.01f, 50.0f);
	}

	if (optionsChanged)
	{
		scene->renderOptions = renderOptions;
		scene->camera->isMoving = true;
	}

	if (ImGui::CollapsingHeader("Objects"))
	{
		bool objectPropChanged = false;

		std::vector<std::string> listboxItems;
		for (int i = 0; i < scene->meshInstances.size(); i++) 
		{
			listboxItems.push_back(scene->meshInstances[i].name);
		}
		
		ImGui::ListBoxHeader("Instances");
		for (int i = 0; i < scene->meshInstances.size(); i++)
		{
			bool isSelected = selectedInstance == i;
			if (ImGui::Selectable(listboxItems[i].c_str(), isSelected)) 
			{
				selectedInstance = i;
			}
		}
		ImGui::ListBoxFooter();

		ImGui::Separator();
		ImGui::Text("Materials");

		Vector3* albedo   = &scene->materials[scene->meshInstances[selectedInstance].materialID].albedo;
		Vector3* emission = &scene->materials[scene->meshInstances[selectedInstance].materialID].emission;
		int materialType  = +scene->materials[scene->meshInstances[selectedInstance].materialID].type;

		objectPropChanged |= ImGui::ColorEdit3("Albedo", (float*)albedo, 0);
		objectPropChanged |= ImGui::SliderInt("Type", &materialType, 0, 1);
		scene->materials[scene->meshInstances[selectedInstance].materialID].type = materialType;

		objectPropChanged |= ImGui::ColorEdit3("Emission", (float*)emission, 0);
		objectPropChanged |= ImGui::SliderFloat("Metallic", &scene->materials[scene->meshInstances[selectedInstance].materialID].metallic, 0.001, 1.0);
		objectPropChanged |= ImGui::SliderFloat("Roughness", &scene->materials[scene->meshInstances[selectedInstance].materialID].roughness, 0.001, 1.0);
		objectPropChanged |= ImGui::SliderFloat("ior", &scene->materials[scene->meshInstances[selectedInstance].materialID].ior, 0.001, 5.0);
		objectPropChanged |= ImGui::SliderFloat("transmittance", &scene->materials[scene->meshInstances[selectedInstance].materialID].transmittance, 0.001, 5.0);

		ImGui::Separator();
		ImGui::Text("Transforms");
		{
			Matrix4x4 trans = scene->meshInstances[selectedInstance].transform;

			float gizmoProj[16];
			float gizmoView[16];
			scene->camera->GetGizmoViewProjection(gizmoView, gizmoProj);

			EditTransform(gizmoView, gizmoProj, trans);

			if (memcmp(&trans, &scene->meshInstances[selectedInstance].transform, sizeof(float) * 16))
			{
				scene->meshInstances[selectedInstance].transform = trans;
				objectPropChanged = true;
			}
		}

		if (objectPropChanged) 
		{
			scene->RebuildInstancesData();
		}
	}

	ImGui::End();
}

void MainLoop()
{
	glfwPollEvents();

	double currTime = glfwGetTime();
	double passTime = currTime - lastTime;
	lastTime = currTime;
	
	OnGUI((float)passTime);
	Update((float)passTime);
	Render((float)passTime);

	glfwMakeContextCurrent(glfwWindow);
	glfwSwapBuffers(glfwWindow);
}

static void OnGLFWErrorCallback(int error, const char* description)
{
	printf("GLFW Error %d: %s\n", error, description);
}

static void OnGLFWResizeCallback(GLFWwindow* window, int width, int height)
{
    int fWidth  = 0;
    int fHeight = 0;
    glfwGetFramebufferSize(window, &fWidth, &fHeight);
	scene->Resize(width, height, fWidth, fHeight);
}

void Usage() 
{
	printf("Usage: Pathtracer -i filepath\n");
	printf("\n");
	printf("Main options:\n");
	printf("  -h | -?               show help.\n");
	printf("  -i <input>            input file name.\n");
}

bool InitOpenGLResources()
{
	glfwSetErrorCallback(OnGLFWErrorCallback);

	if (!glfwInit()) {
		return false;
	}
    
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined(PLATFORM_MAC)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwWindow = glfwCreateWindow(renderOptions.windowSize.x, renderOptions.windowSize.y, "PathTracer", NULL, NULL);
	if (glfwWindow == NULL) {
		return false;
	}

	glfwMakeContextCurrent(glfwWindow);
	glfwSetFramebufferSizeCallback(glfwWindow, OnGLFWResizeCallback);
    
    int frameWidth;
    int frameHeight;
    glfwGetFramebufferSize(glfwWindow, &frameWidth, &frameHeight);
    scene->renderOptions.frameSize = Vector2(frameWidth, frameHeight);
    
	if (!gladLoadGL())
	{
		printf("Failed to initialize OpenGL loader!\n");
		return false;
	}
    
    lastTime = glfwGetTime();
    
	return true;
}

bool InitIMGUI()
{
#if defined(PLATFORM_MAC)
	const char* glslVersion = "#version 150";
#else
	const char* glslVersion = "#version 130";
#endif

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
	ImGui_ImplOpenGL3_Init(glslVersion);

	return true;
}

bool Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(glfwWindow);
	glfwTerminate();

	delete renderer;
	delete scene;

	return true;
}

bool InitScene()
{
	sampleSceneIndex = 0;
	LoadScene(sceneFiles[sampleSceneIndex]);

	return true;
}

void GetDirFiles(const std::string& path, std::vector<std::string>& files)
{
	tinydir_dir dir;
	tinydir_open(&dir, path.c_str());

	while (dir.has_next)
	{
		tinydir_file file;
		tinydir_readfile(&dir, &file);

		if (file.is_dir)
		{
			// 
		}
		else
		{
			files.push_back(file.path);
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);
}

bool InitSceneFiles()
{
	std::vector<std::string> files;
	GetDirFiles(assetsDir, files);
	for (int i = 0; i < files.size(); ++i)
	{
		std::string& file = files[i];
		std::string name  = file.substr(file.find_last_of("/\\") + 1);

		sceneFiles.push_back(file);
		sceneNames.push_back(name);
	}

	files.clear();
	GetDirFiles(hdrResDir, files);
	for (int i = 0; i < files.size(); ++i)
	{
		std::string& file = files[i];
		std::string name  = file.substr(file.find_last_of("/\\") + 1);

		envFiles.push_back(file);
		envNames.push_back(name);
	}

	return true;
}

int main(int argc, char** argv)
{
	std::string exePath = argv[0];
	std::string dirPath = exePath.substr(0, exePath.find_last_of("/\\")) + "/";
	for (int i = 0; i < dirPath.size(); ++i) {
		if (dirPath[i] == '\\') {
			dirPath[i] = '/';
		}
	}

	assetsDir = dirPath + "assets/";
	shaderDir = dirPath + "shaders/";
	hdrResDir = assetsDir + "HDR/";

	if (!InitSceneFiles()) {
		return 1;
	}

	if (!InitScene()) {
		return 1;
	}
    
	if (!InitOpenGLResources()) {
		return 1;
	}
    
	if (!InitIMGUI()) {
		return 1;
	}
    
	if (!InitRenderer()) {
		return 1;
	}
    
	while (!glfwWindowShouldClose(glfwWindow)) {
		MainLoop();
	}
    
	Cleanup();
    
    return 0;
}
