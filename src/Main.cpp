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

using namespace GLSLPT;

float mouseSensitivity = 1.0f;
int sampleSceneIndex = -1;
int selectedInstance = 0;
double lastTime = 0;

Scene* scene = nullptr;
Renderer* renderer = nullptr;
GLFWwindow* glfwWindow = NULL;
RenderOptions renderOptions;

void LoadSampleScene(int index)
{
	std::string path(EMBED_RES_PATH);
	if (scene) {
		delete scene;
		scene = nullptr;
	}
	scene = new Scene();

    switch (index)
    {
	case 0:
		LoadSceneFromFile(path + "assets/hyperion.scene", scene, renderOptions);
		break;
	case 1:
		LoadSceneFromFile(path + "assets/cornell_box.scene", scene, renderOptions);
		break;
	case 2:
		LoadBoyTestScene(path, scene, renderOptions);
		break;
    }

	// LoadSceneFromGLTF(path + "assets/diorama.glb", scene, renderOptions);

	selectedInstance = 0;
    scene->renderOptions = renderOptions;
}

bool InitRenderer()
{
	std::string path(EMBED_RES_PATH);

	if (renderer) {
		delete renderer;
		renderer = nullptr;
	}
    
    renderer = new TiledRenderer(scene, path + "shaders/");
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
	ImGuiIO& io = ImGui::GetIO();

	if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) && (ImGui::IsMouseDown(2) || ImGui::IsMouseDown(1)) && !ImGuizmo::IsOver())
	{
		if (ImGui::IsMouseDown(2) && !io.KeyShift)
		{
			ImVec2 mouseDelta = ImGui::GetMouseDragDelta(2, 0);
			scene->camera->OffsetOrientation(mouseSensitivity * mouseDelta.x, mouseSensitivity * mouseDelta.y);
			ImGui::ResetMouseDragDelta(2);
		}
		else if (ImGui::IsMouseDown(2))
		{
			ImVec2 mouseDelta = ImGui::GetMouseDragDelta(2, 0);
			scene->camera->Strafe(0.01 * mouseDelta.x, 0.01 * mouseDelta.y);
			ImGui::ResetMouseDragDelta(2);
		}
		else if (ImGui::IsMouseDown(1))
		{
			ImVec2 mouseDelta = ImGui::GetMouseDragDelta(1, 0);
			scene->camera->ChangeRadius(mouseSensitivity * 0.01 * mouseDelta.y);
			ImGui::ResetMouseDragDelta(1);
		}
		scene->camera->isMoving = true;
	}

	renderer->Update(deltaTime);
}

void EditTransform(const float* view, const float* projection, float* matrix)
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE      mCurrentGizmoMode(ImGuizmo::WORLD);

	if (ImGui::IsKeyPressed(90)) {
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	}
	if (ImGui::IsKeyPressed(69)) {
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	}
	if (ImGui::IsKeyPressed(82)) {
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	}
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE)) {
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE)) {
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE)) {
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	}

	float matrixTranslation[3];
	float matrixRotation[3];
	float matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Tr", matrixTranslation);
	ImGui::InputFloat3("Rt", matrixRotation);
	ImGui::InputFloat3("Sc", matrixScale);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL)) {
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD)) {
			mCurrentGizmoMode = ImGuizmo::WORLD;
		}
	}
	
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	ImGuizmo::Manipulate(view, projection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, NULL);
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

	ImGui::BulletText("RMB + drag to zoom in/out");
	ImGui::BulletText("MMB + drag to rotate");
	ImGui::BulletText("SHIFT + MMB + drag to pan");

	if (ImGui::Combo("Scene", &sampleSceneIndex, "Hyperion\0Cornell Box\0Boy\0"))
	{
		LoadSampleScene(sampleSceneIndex);
		InitRenderer();
	}

	bool optionsChanged = false;
	if (ImGui::CollapsingHeader("Render Settings"))
	{
		optionsChanged |= ImGui::SliderInt("Max Depth", &renderOptions.maxDepth, 1, 10);
		optionsChanged |= ImGui::Checkbox("Use envmap", &renderOptions.useEnvMap);
		optionsChanged |= ImGui::SliderFloat("HDR multiplier", &renderOptions.intensity, 0.1, 10);
	}

	if (ImGui::CollapsingHeader("Camera"))
	{
        float fov = MMath::RadiansToDegrees(scene->camera->fov);
		float aperture = scene->camera->aperture * 1000.0f;

		optionsChanged |= ImGui::SliderFloat("Fov", &fov, 10, 90);
		scene->camera->SetFov(fov);

		optionsChanged |= ImGui::SliderFloat("Aperture", &aperture, 0.0f, 10.8f);
		scene->camera->aperture = aperture / 1000.0f;

		optionsChanged |= ImGui::SliderFloat("Focal Distance", &scene->camera->focalDist, 0.01, 50.0);
	}

	scene->camera->isMoving = false;

	if (optionsChanged)
	{
		scene->renderOptions = renderOptions;
		scene->camera->isMoving = true;
	}

	if (ImGui::CollapsingHeader("Objects"))
	{
		bool objectPropChanged = false;

		std::vector<std::string> listboxItems;
		for (int i = 0; i < scene->meshInstances.size(); i++) {
			listboxItems.push_back(scene->meshInstances[i].name);
		}
		
		ImGui::ListBoxHeader("Instances");
		for (int i = 0; i < scene->meshInstances.size(); i++)
		{
			bool isSelected = selectedInstance == i;
			if (ImGui::Selectable(listboxItems[i].c_str(), isSelected)) {
				selectedInstance = i;
			}
		}
		ImGui::ListBoxFooter();

		ImGui::Separator();
		ImGui::Text("Materials");

		Vector3* albedo   = &scene->materials[scene->meshInstances[selectedInstance].materialID].albedo;
		Vector3* emission = &scene->materials[scene->meshInstances[selectedInstance].materialID].emission;

		objectPropChanged |= ImGui::ColorEdit3("Albedo", (float*)albedo, 0);
		objectPropChanged |= ImGui::ColorEdit3("Emission", (float*)emission, 0);
		objectPropChanged |= ImGui::SliderFloat("Metallic", &scene->materials[scene->meshInstances[selectedInstance].materialID].metallic, 0.001, 1.0);
		objectPropChanged |= ImGui::SliderFloat("Roughness", &scene->materials[scene->meshInstances[selectedInstance].materialID].roughness, 0.001, 1.0);

		ImGui::Separator();
		ImGui::Text("Transforms");
		{
			float viewMatrix[16];
			float projectionMatrix[16];
			auto io = ImGui::GetIO();
			
			scene->camera->ComputeViewProjectionMatrix(viewMatrix, projectionMatrix, io.DisplaySize.x / io.DisplaySize.y);
			Matrix4x4 tmpMat = scene->meshInstances[selectedInstance].transform;

			EditTransform(viewMatrix, projectionMatrix, (float*)&tmpMat.m);

			if (memcmp(&tmpMat, &scene->meshInstances[selectedInstance].transform, sizeof(float) * 16))
			{
				scene->meshInstances[selectedInstance].transform = tmpMat;
				objectPropChanged = true;
			}
		}

		if (objectPropChanged) {
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

static void OnGLFWResizeCallback(GLFWwindow* glfwWindow, int width, int height)
{
	glViewport(0, 0, width, height);
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

	glfwWindow = glfwCreateWindow(renderOptions.resolution.x, renderOptions.resolution.y, "PathTracer", NULL, NULL);
	if (glfwWindow == NULL) {
		return false;
	}

	glfwMakeContextCurrent(glfwWindow);
	glfwSetFramebufferSizeCallback(glfwWindow, OnGLFWResizeCallback);

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

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

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

std::string GetSceneFile(int argc, char** argv)
{
	std::string filename;

	const int argc_check = argc - 1;

	for (int i = 1; i < argc; ++i) 
	{
		if (!strcmp("-h", argv[i]) || !strcmp("-?", argv[i])) 
		{
			Usage();
			exit(0);
			return filename;
		}
		else if (!strcmp("-i", argv[i]) && i < argc_check) 
		{
			filename = argv[++i];
		}
	}

	return filename;
}

bool InitScene(const std::string& scenefile)
{
	if (scenefile.empty())
	{
		sampleSceneIndex = 0;
		LoadSampleScene(sampleSceneIndex);
	}
	else
	{
		scene = new Scene();
		if (!LoadSceneFromFile(scenefile, scene, renderOptions)) {
			return 1;
		}
		scene->renderOptions = renderOptions;
		printf("Scene %s Loaded\n", scenefile.c_str());
	}

	return true;
}

int main(int argc, char** argv)
{
	if (!InitScene(GetSceneFile(argc, argv))) {
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
