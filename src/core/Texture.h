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
		{

		}

		~Texture();

		bool LoadTexture(const std::string& filename);

		void SetChannel(int channel);

		void Resize(int width, int height);
		
		int width;
		int height;
		int comp;
		std::string name;

		bool loaded;

		std::vector<uint8> texData;
	};
}
