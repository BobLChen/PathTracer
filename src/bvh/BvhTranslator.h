/**********************************************************************
Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
********************************************************************/

/* 
	Code is not the original code from RadeonRays. It is modfied slightly for this project 
	Please refer to https://github.com/GPUOpen-LibrariesAndSDKs/RadeonRays_SDK for the original code
*/

#pragma once

#ifndef BVH_TRANSLATOR_H
#define BVH_TRANSLATOR_H

#include <map>

#include "Bvh.h"
#include "core/Mesh.h"

namespace RadeonRays
{
    /// This class translates pointer based BVH representation into
    /// index based one suitable for feeding to GPU or any other accelerator
    //
    class BvhTranslator
    {
    public:

        // Constructor
        BvhTranslator() = default;

		struct Node
		{
			int leftIndex;
			int rightIndex;
			int leaf;
		};

		void ProcessBLAS();
		void ProcessTLAS();
		void UpdateTLAS(const Bvh* topLevelBvh, const std::vector<GLSLPT::MeshInstance>& instances);
		void Process(const Bvh* topLevelBvh, const std::vector<GLSLPT::Mesh*>& meshes, const std::vector<GLSLPT::MeshInstance>& instances);
		
	private:
		int ProcessBLASNodes(const Bvh::Node* root);
		int ProcessTLASNodes(const Bvh::Node* root);

	public:
		std::vector<glm::vec3> bboxmin;
		std::vector<glm::vec3> bboxmax;
		std::vector<Node> nodes;
		int nodeTexWidth;
		int topLevelIndexPackedXY = 0;
		int topLevelIndex = 0;

    private:
		int curNode = 0;
		int curTriIndex = 0;
		const Bvh* TLBvh;
		std::vector<int> bvhRootStartIndices;
		std::vector<GLSLPT::MeshInstance> meshInstances;
		std::vector<GLSLPT::Mesh*> meshes;
    };
}

#endif // BVH_TRANSLATOR_H
