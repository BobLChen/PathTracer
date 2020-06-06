#pragma once

#include <vector>

#include "math/Math.h"

namespace GLSLPT
{
	class Texture
	{
	public:
		Texture() 
			: width(-1) 
			, height(-1)
			, comp(3)
			, loaded(false)
			, texData(nullptr)
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
		int comp;
		std::string name;

		bool loaded;

		uint8* texData;
	};
}
