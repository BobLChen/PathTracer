#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "math/Math.h"

namespace GLSLPT
{	
	class Texture
	{
	public:
		Texture() 
			: texData(nullptr) 
		{

		}

		~Texture() 
		{ 
			if (texData)
			{
				delete texData;
				texData = nullptr;
			}
		}

		bool LoadTexture(const std::string& filename);
		
		int width;
		int height;
		std::string name;

		uint8* texData;
	};
}