#include <iostream>

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
	bool Texture::LoadTexture(const std::string& filename)
	{
		name    = filename;
		texData = stbi_load(filename.c_str(), &width, &height, &comp, 3);
		comp    = 3;
		loaded  = true;
		return texData != nullptr;
	}
}