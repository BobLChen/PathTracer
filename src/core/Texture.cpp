#include <array>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "parser/stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "parser/stb_image_resize.h"

#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "parser/stb_image_write.h"

namespace GLSLPT
{
	Texture::~Texture() 
	{

	}

	bool Texture::LoadTexture(const std::string& filename)
	{
		uint8 *data = stbi_load(filename.c_str(), &width, &height, &comp, 3);
		if (data == nullptr) 
		{
			return false;
		}

		name = filename;
		comp = 3;
		
		texData.resize(width * height * comp);
		std::copy(data, data + texData.size(), texData.begin());

		loaded = true;
		return true;
	}

	void Texture::Resize(int nWidth, int nHeight)
	{
		if (width == nWidth && height == nHeight)
		{
			return;
		}
		
		std::vector<uint8> temp(nWidth * nHeight * comp);
		
		stbir_resize_uint8(texData.data(), width, height, 0, temp.data(), nWidth, nHeight, 0, comp);

		texData.resize(nWidth * nHeight * comp);
		std::copy(temp.begin(), temp.end(), texData.begin());

		width  = nWidth;
		height = nHeight;
	}

	void Texture::SetChannel(int channel)
	{
		if (comp == channel) 
		{
			return;
		}

		std::vector<uint8> temp(width * height * channel);

		for (int i = 0; i < width * height; ++i)
		{
			int oIndex = i * comp;
			int nIndex = i * channel;

			for (int c = 0; c < comp && c < channel; ++c)
			{
				temp[nIndex + c] = texData[oIndex + c];
			}

			int size = channel - comp;
			for (int c = 0; c < size; ++c)
			{
				temp[nIndex + comp + c] = 255;
			}
		}

		texData.resize(width * height * channel);
		std::copy(temp.begin(), temp.end(), texData.begin());

		comp = channel;
	}
}