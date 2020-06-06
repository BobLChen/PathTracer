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
#include <vector>
#include <algorithm>
#include <limits>
#include <numeric>

#include "Bvh.h"
#include "math/Vector3.h"

namespace RadeonRays
{
    static const int kMaxPrimitivesPerLeaf = 1;

    static bool IsNaN(float v)
    {
        return v != v;
    }

    void Bvh::Build(const Bounds3D* bounds, int numbounds)
    {
        for (int i = 0; i < numbounds; ++i)
        {
            // Calc bbox
            m_Bounds.Expand(bounds[i]);
        }

        BuildImpl(bounds, numbounds);
    }

    void Bvh::InitNodeAllocator(size_t maxnum)
    {
        m_Nodecnt = 0;
        m_Nodes.resize(maxnum);
    }

    Bvh::Node* Bvh::AllocateNode()
    {
        return &m_Nodes[m_Nodecnt++];
    }

    void Bvh::BuildNode(const SplitRequest& req, const Bounds3D* bounds, const Vector3* centroids, int* primindices)
    {
        m_Height = std::max(m_Height, req.level);

        Node* node   = AllocateNode();
        node->bounds = req.bounds;
        node->index  = req.index;

        // Create leaf node if we have enough prims
        if (req.numprims < 2)
        {
            node->type = kLeaf;
            node->startidx = static_cast<int>(m_PackedIndices.size());
            node->numprims = req.numprims;

            for (auto i = 0; i < req.numprims; ++i)
            {
				m_PackedIndices.push_back(primindices[req.startidx + i]);
            }
        }
        else
        {
            // Choose the maximum extent
            int axis     = req.centroidBounds.Maxdim();
            float border = req.centroidBounds.Center()[axis];

            if (m_Usesah)
            {
                SahSplit ss = FindSahSplit(req, bounds, centroids, primindices);

                if (!IsNaN(ss.split))
                {
                    axis   = ss.dim;
                    border = ss.split;

                    if (req.numprims < ss.sah && req.numprims < kMaxPrimitivesPerLeaf)
                    {
                        node->type     = kLeaf;
                        node->startidx = static_cast<int>(m_PackedIndices.size());
                        node->numprims = req.numprims;

                        for (auto i = 0; i < req.numprims; ++i)
                        {
							m_PackedIndices.push_back(primindices[req.startidx + i]);
                        }

						if (req.ptr) {
							*req.ptr = node;
						}
                        return;
                    }
                }
            }

            node->type = kInternal;

            // Start partitioning and updating extents for children at the same time
			Bounds3D leftbounds;
			Bounds3D rightbounds;
			Bounds3D leftCentroidBounds;
			Bounds3D rightCentroidBounds;

            int splitidx  = req.startidx;
            bool near2far = (req.numprims + req.startidx) & 0x1;

            if (req.centroidBounds.Extents()[axis] > 0.f)
            {
                auto first = req.startidx;
                auto last  = req.startidx + req.numprims;

                if (near2far)
                {
                    while (true)
                    {
                        while ((first != last) && centroids[primindices[first]][axis] < border)
                        {
                            leftbounds.Expand(bounds[primindices[first]]);
                            leftCentroidBounds.Expand(centroids[primindices[first]]);
                            ++first;
                        }

						if (first == last--) {
							break;
						}

                        rightbounds.Expand(bounds[primindices[first]]);
                        rightCentroidBounds.Expand(centroids[primindices[first]]);

                        while ((first != last) && centroids[primindices[last]][axis] >= border)
                        {
                            rightbounds.Expand(bounds[primindices[last]]);
                            rightCentroidBounds.Expand(centroids[primindices[last]]);
                            --last;
                        }

						if (first == last) {
							break;
						}

                        leftbounds.Expand(bounds[primindices[last]]);
                        leftCentroidBounds.Expand(centroids[primindices[last]]);

                        std::swap(primindices[first++], primindices[last]);
                    }
                }
                else
                {
                    while (true)
                    {
                        while ((first != last) && centroids[primindices[first]][axis] >= border)
                        {
                            leftbounds.Expand(bounds[primindices[first]]);
                            leftCentroidBounds.Expand(centroids[primindices[first]]);
                            ++first;
                        }

						if (first == last--) {
							break;
						}

                        rightbounds.Expand(bounds[primindices[first]]);
                        rightCentroidBounds.Expand(centroids[primindices[first]]);

                        while ((first != last) && centroids[primindices[last]][axis] < border)
                        {
                            rightbounds.Expand(bounds[primindices[last]]);
                            rightCentroidBounds.Expand(centroids[primindices[last]]);
                            --last;
                        }

						if (first == last) {
							break;
						}

                        leftbounds.Expand(bounds[primindices[last]]);
                        leftCentroidBounds.Expand(centroids[primindices[last]]);

                        std::swap(primindices[first++], primindices[last]);
                    }
                }

                splitidx = first;
            }

            if (splitidx == req.startidx || splitidx == req.startidx + req.numprims)
            {
                splitidx = req.startidx + (req.numprims >> 1);

                for (int i = req.startidx; i < splitidx; ++i)
                {
                    leftbounds.Expand(bounds[primindices[i]]);
                    leftCentroidBounds.Expand(centroids[primindices[i]]);
                }

                for (int i = splitidx; i < req.startidx + req.numprims; ++i)
                {
                    rightbounds.Expand(bounds[primindices[i]]);
                    rightCentroidBounds.Expand(centroids[primindices[i]]);
                }
            }

            // Left request
            SplitRequest leftrequest  = { req.startidx, splitidx - req.startidx, &node->lc, leftbounds, leftCentroidBounds, req.level + 1, (req.index << 1) };
            
			// Right request
            SplitRequest rightrequest = { splitidx, req.numprims - (splitidx - req.startidx), &node->rc, rightbounds, rightCentroidBounds, req.level + 1, (req.index << 1) + 1 };

			// Put those to stack
			BuildNode(leftrequest, bounds, centroids, primindices);
			BuildNode(rightrequest, bounds, centroids, primindices);
        }

        // Set parent ptr if any
		if (req.ptr) {
			*req.ptr = node;
		}
    }

    Bvh::SahSplit Bvh::FindSahSplit(const SplitRequest& req, const Bounds3D* bounds, const Vector3* centroids, int* primindices) const
    {
        // SAH implementation
        // calc centroids histogram
        // int const kNumBins = 128;
        // moving split bin index
        int splitidx = -1;
        // Set SAH to maximum float value as a start
        float sah = std::numeric_limits<float>::max();

        SahSplit split;
        split.dim   = 0;
        split.split = std::numeric_limits<float>::quiet_NaN();

        // if we cannot apply histogram algorithm
        // put NAN sentinel as split border
        // PerformObjectSplit simply splits in half
        // in this case
        Vector3 centroidExtents = req.centroidBounds.Extents();
        if (Vector3::DotProduct(centroidExtents, centroidExtents) == 0.f)
        {
            return split;
        }

        // Bin has bbox and occurence count
        struct Bin
        {
			Bounds3D bounds;
            int count;
        };

        // Keep bins for each dimension
        std::vector<Bin> bins[3];
        bins[0].resize(m_NumBins);
        bins[1].resize(m_NumBins);
        bins[2].resize(m_NumBins);

        // Precompute inverse parent area
        float invarea = 1.f / req.bounds.Area();
        // Precompute min point
        Vector3 rootmin = req.centroidBounds.min;

        // Evaluate all dimensions
        for (int axis = 0; axis < 3; ++axis)
        {
            float rootminc = rootmin[axis];
            // Range for histogram
            float centroidRNG = centroidExtents[axis];
            float invcentroidRNG = 1.f / centroidRNG;

            // If the box is degenerate in that dimension skip it
			if (centroidRNG == 0.f) {
				continue;
			}

            // Initialize bins
            for (int i = 0; i < m_NumBins; ++i)
            {
                bins[axis][i].count = 0;
                bins[axis][i].bounds = Bounds3D();
            }

            // Calc primitive refs histogram
            for (int i = req.startidx; i < req.startidx + req.numprims; ++i)
            {
                int idx = primindices[i];
                int binidx = (int)std::min<float>(static_cast<float>(m_NumBins) * ((centroids[idx][axis] - rootminc) * invcentroidRNG), static_cast<float>(m_NumBins - 1));

                ++bins[axis][binidx].count;
                bins[axis][binidx].bounds.Expand(bounds[idx]);
            }

            std::vector<Bounds3D> rightbounds(m_NumBins - 1);

            // Start with 1-bin right box
			Bounds3D rightbox;
            for (int i = m_NumBins - 1; i > 0; --i)
            {
                rightbox.Expand(bins[axis][i].bounds);
                rightbounds[i - 1] = rightbox;
            }

			Bounds3D leftbox;
            int leftcount = 0;
            int rightcount = req.numprims;

            // Start best SAH search
            // i is current split candidate (split between i and i + 1)
            float sahtmp = 0.f;
            for (int i = 0; i < m_NumBins - 1; ++i)
            {
                leftbox.Expand(bins[axis][i].bounds);
                leftcount  += bins[axis][i].count;
                rightcount -= bins[axis][i].count;

                // Compute SAH
                sahtmp = m_TraversalCost + (leftcount * leftbox.Area() + rightcount * rightbounds[i].Area()) * invarea;

                // Check if it is better than what we found so far
                if (sahtmp < sah)
                {
                    split.dim = axis;
                    splitidx = i;
                    split.sah = sah = sahtmp;
                }
            }
        }

        // Choose split plane
        if (splitidx != -1)
        {
            split.split = rootmin[split.dim] + (splitidx + 1) * (centroidExtents[split.dim] / m_NumBins);
        }

        return split;
    }

    void Bvh::BuildImpl(const Bounds3D* bounds, int numbounds)
    {
        // Structure describing split request
        InitNodeAllocator(2 * numbounds - 1);

        // Cache some stuff to have faster partitioning
        std::vector<Vector3> centroids(numbounds);
        m_Indices.resize(numbounds);
        std::iota(m_Indices.begin(), m_Indices.end(), 0);

        // Calc bbox
		Bounds3D centroidBounds;
        for (size_t i = 0; i < static_cast<size_t>(numbounds); ++i)
        {
            Vector3 c = bounds[i].Center();
            centroidBounds.Expand(c);
            centroids[i] = c;
        }

        SplitRequest init = { 0, numbounds, nullptr, m_Bounds, centroidBounds, 0, 1 };

        BuildNode(init, bounds, &centroids[0], &m_Indices[0]);

        // Set root_ pointer
        m_Root = &m_Nodes[0];
    }

}
