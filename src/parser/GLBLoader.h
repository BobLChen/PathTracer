#pragma once

#include <string>

#include "core/Scene.h"
#include "core/Renderer.h"

namespace GLSLPT
{
	bool LoadSceneFromGLTF(const std::string& filename, Scene* scene, RenderOptions& renderOptions);
}