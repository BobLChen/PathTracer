#pragma once

#include <string>

#include "core/Scene.h"

namespace GLSLPT
{
    bool LoadSceneFromFile(const std::string& filename, Scene* scene, RenderOptions& renderOptions);
}