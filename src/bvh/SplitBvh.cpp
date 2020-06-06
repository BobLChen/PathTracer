#include <algorithm>
#include <cassert>
#include <cmath>

#include "SplitBvh.h"

namespace RadeonRays
{
    void SplitBvh::BuildImpl(const Bounds3D* bounds, int numbounds)
    {
        // Initialize prim refs structures
        PrimRefArray primrefs(numbounds);

        // Keep centroids to speed up partitioning
        std::vector<Vector3> centroids(numbounds);
		Bounds3D centroidBounds;

        for (auto i = 0; i < numbounds; ++i)
        {
            primrefs[i] = PrimRef { bounds[i], bounds[i].Center(), i };

            auto c = bounds[i].Center();
            centroidBounds.Expand(c);
        }

        m_NumNodesForRegular = (2 * numbounds - 1);
        m_NumNodesRequired   = (int)(m_NumNodesForRegular * (1.f + m_ExtraRefsBudget));

        InitNodeAllocator(m_NumNodesRequired);

        SplitRequest init = { 0, numbounds, nullptr, m_Bounds, centroidBounds, 0 };

        // Start from the top
        BuildNode(init, primrefs);
    }

    void SplitBvh::BuildNode(SplitRequest& req, PrimRefArray& primrefs)
    {
        // Update current height
        m_Height = std::max(m_Height, req.level);

        // Allocate new node
        Node* node   = AllocateNode();
        node->bounds = req.bounds;

        // Create leaf node if we have enough prims
        if (req.numprims < 2)
        {
            node->type     = kLeaf;
            node->startidx = (int)m_PackedIndices.size();
            node->numprims = req.numprims;

            for (int i = req.startidx; i < req.startidx + req.numprims; ++i)
            {
                m_PackedIndices.push_back(primrefs[i].idx);
            }
        }
        else
        {
            node->type = kInternal;

            // Choose the maximum extent
            int axis = req.centroidBounds.Maxdim();
            float border = req.centroidBounds.Center()[axis];

            SahSplit os = FindObjectSahSplit(req, primrefs);
            SahSplit ss;
            auto splitType = SplitType::kObject;

            // Only use split if
            // 1. Maximum depth is not exceeded
            // 2. We found spatial split
            // 3. It is better than object split
            // 4. Object split is not good enought (too much overlap)
            // 5. Our node budget still allows us to split references
            if (req.level < m_MaxSplitDepth && m_Nodecnt < m_NumNodesRequired && os.overlap > m_MinOverlap)
            {
                ss = FindSpatialSahSplit(req, primrefs);

                if (!std::isnan(ss.split) && ss.sah < os.sah)
                {
                    splitType = SplitType::kSpatial;
                }
            }

            if (splitType == SplitType::kSpatial)
            {
                // First we need maximum 2x numprims elements allocated
                size_t elems = req.startidx + req.numprims * 2;
                if (primrefs.size() < elems)
                {
                    primrefs.resize(elems);
                }

                // Split prim refs and add extra refs to request
                int extra_refs = 0;
                SplitPrimRefs(ss, req, primrefs, extra_refs);
                req.numprims += extra_refs;
                border = ss.split;
                axis = ss.dim;
            }
            else
            {
                border = !std::isnan(os.split) ? os.split : border;
                axis   = !std::isnan(os.split) ? os.dim   : axis;
            }
            
            // Start partitioning and updating extents for children at the same time
			Bounds3D leftbounds;
			Bounds3D rightbounds;
			Bounds3D leftcentroidBounds;
			Bounds3D rightcentroidBounds;

            int splitidx  = req.startidx;
            bool near2far = (req.numprims + req.startidx) & 0x1;

            bool(*cmpl)(float, float)  = [](float a, float b) -> bool { return a < b; };
            bool(*cmpge)(float, float) = [](float a, float b) -> bool { return a >= b; };

            auto cmp1 = near2far ? cmpl  : cmpge;
            auto cmp2 = near2far ? cmpge : cmpl;

            if (req.centroidBounds.Extents()[axis] > 0.f)
            {
                auto first = req.startidx;
                auto last = req.startidx + req.numprims;

                while (true)
                {
                    while ((first != last) && cmp1(primrefs[first].center[axis], border))
                    {
                        leftbounds.Expand(primrefs[first].bounds);
                        leftcentroidBounds.Expand(primrefs[first].center);
                        ++first;
                    }

					if (first == last--) {
						break;
					}

                    rightbounds.Expand(primrefs[first].bounds);
                    rightcentroidBounds.Expand(primrefs[first].center);

                    while ((first != last) && cmp2(primrefs[last].center[axis], border))
                    {
                        rightbounds.Expand(primrefs[last].bounds);
                        rightcentroidBounds.Expand(primrefs[last].center);
                        --last;
                    }

					if (first == last) {
						break;
					}

                    leftbounds.Expand(primrefs[last].bounds);
                    leftcentroidBounds.Expand(primrefs[last].center);

                    std::swap(primrefs[first++], primrefs[last]);
                }

                splitidx = first;
            }

            if (splitidx == req.startidx || splitidx == req.startidx + req.numprims)
            {
                splitidx = req.startidx + (req.numprims >> 1);

                for (int i = req.startidx; i < splitidx; ++i)
                {
                    leftbounds.Expand(primrefs[i].bounds);
                    leftcentroidBounds.Expand(primrefs[i].center);
                }

                for (int i = splitidx; i < req.startidx + req.numprims; ++i)
                {
                    rightbounds.Expand(primrefs[i].bounds);
                    rightcentroidBounds.Expand(primrefs[i].center);
                }
            }

            // Left request
            SplitRequest leftrequest  = { req.startidx, splitidx - req.startidx, &node->lc, leftbounds, leftcentroidBounds, req.level + 1 };
            // Right request
            SplitRequest rightrequest = { splitidx, req.numprims - (splitidx - req.startidx), &node->rc, rightbounds, rightcentroidBounds, req.level + 1 };

            // The order is very important here since right node uses the space at the end of the array to partition
			BuildNode(rightrequest, primrefs);

			// Put those to stack
			BuildNode(leftrequest, primrefs);
        }

        // Set parent ptr if any
		if (req.ptr) {
			*req.ptr = node;
		}
    }

    SplitBvh::SahSplit SplitBvh::FindObjectSahSplit(const SplitRequest& req, const PrimRefArray& refs) const
    {
        // SAH implementation
        // calc centroids histogram
        // moving split bin index
        int splitidx = -1;
        // Set SAH to maximum float value as a start
        auto sah = std::numeric_limits<float>::max();

        SahSplit split;
        split.dim   = 0;
        split.split = std::numeric_limits<float>::quiet_NaN();
        split.sah   = sah;

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
        auto invarea = 1.f / req.bounds.Area();
        // Precompute min point
        auto rootmin = req.centroidBounds.min;

        // Evaluate all dimensions
        for (int axis = 0; axis < 3; ++axis)
        {
            float rootminc = rootmin[axis];
            // Range for histogram
            auto centroidRNG = centroidExtents[axis];
            auto invcentroidRNG = 1.f / centroidRNG;

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
                auto idx = i;
                auto binidx = (int)std::min<float>(static_cast<float>(m_NumBins) * ((refs[idx].center[axis] - rootminc) * invcentroidRNG), static_cast<float>(m_NumBins - 1));

                ++bins[axis][binidx].count;
                bins[axis][binidx].bounds.Expand(refs[idx].bounds);
            }

            std::vector<Bounds3D> rightbounds(m_NumBins - 1);

            // Start with 1-bin right box
			Bounds3D rightbox;
            for (int i = m_NumBins - 1; i > 0; --i)
            {
                rightbox.Expand(bins[axis][i].bounds);
                rightbounds[i - 1] = rightbox;
            }

			Bounds3D leftbox ;
            int leftcount = 0;
            int rightcount = req.numprims;

            // Start best SAH search
            // i is current split candidate (split between i and i + 1)
            float sahtmp = 0.f;
            for (int i = 0; i < m_NumBins - 1; ++i)
            {
                leftbox.Expand(bins[axis][i].bounds);
                leftcount += bins[axis][i].count;
                rightcount -= bins[axis][i].count;

                // Compute SAH
                sahtmp = m_TraversalCost + (leftcount * leftbox.Area() + rightcount * rightbounds[i].Area()) * invarea;

                // Check if it is better than what we found so far
                if (sahtmp < sah)
                {
                    split.dim = axis;
                    splitidx = i;
                    sah = sahtmp;

                    // Calculate percentage of overlap 
                    split.overlap = Bounds3D::Intersection(leftbox, rightbounds[i]).Area() * invarea;
                }
            }
        }

        // Choose split plane
        if (splitidx != -1)
        {
            split.split = rootmin[split.dim] + (splitidx + 1) * (centroidExtents[split.dim] / m_NumBins);
            split.sah   = sah;
        }

        return split;
    }

    SplitBvh::SahSplit SplitBvh::FindSpatialSahSplit(const SplitRequest& req, const PrimRefArray& refs) const
    {
        // SAH implementation
        // calc centroids histogram
        int const kNumBins = 128;
        // Set SAH to maximum float value as a start
        auto sah = std::numeric_limits<float>::max();

        SahSplit split;
        split.dim   = 0;
        split.split = std::numeric_limits<float>::quiet_NaN();
        split.sah   = sah;

        // Extents
        Vector3 extents = req.bounds.Extents();
        auto invarea = 1.f / req.bounds.Area();

        // If there are too few primitives don't split them
        
        if (Vector3::DotProduct(extents, extents) == 0.f)
        {
            return split;
        }

        // Bin has start and exit counts + bounds
        struct Bin
        {
			Bounds3D bounds;
            int enter;
            int exit;
        };

        Bin bins[3][kNumBins];

        // Prepcompute some useful stuff
        Vector3 origin     = req.bounds.min;
        Vector3 binsize    = req.bounds.Extents() * (1.f / kNumBins);
        Vector3 invbinsize = Vector3(1.f / binsize.x, 1.f / binsize.y, 1.f / binsize.z);

        // Initialize bins
        for (int axis = 0; axis < 3; ++axis)
        {
            for (int i = 0; i < kNumBins; ++i)
            {
                bins[axis][i].bounds = Bounds3D();
                bins[axis][i].enter = 0;
                bins[axis][i].exit = 0;
            }
        }

        // Iterate thru all primitive refs
        for (int i = req.startidx; i < req.startidx + req.numprims; ++i)
        {
            PrimRef const& primref(refs[i]);
            // Determine starting bin for this primitive
            Vector3 firstbin = Vector3::Clamp((primref.bounds.min - origin) * invbinsize, Vector3(0, 0, 0), Vector3(kNumBins - 1, kNumBins - 1, kNumBins - 1));
            // Determine finishing bin
            Vector3 lastbin  = Vector3::Clamp((primref.bounds.max - origin) * invbinsize, firstbin, Vector3(kNumBins - 1, kNumBins - 1, kNumBins - 1));
            // Iterate over axis
            for (int axis = 0; axis < 3; ++axis)
            {
                // Skip in case of a degenerate dimension
				if (extents[axis] == 0.f) {
					continue;
				}

                // Break the prim into bins
                auto tempref = primref;

                for (int j = (int)firstbin[axis]; j < (int)lastbin[axis]; ++j)
                {
                    PrimRef leftref, rightref;
                    // Split primitive ref into left and right
                    float splitval = origin[axis] + binsize[axis] * (j + 1);
                    if (SplitPrimRef(tempref, axis, splitval, leftref, rightref))
                    {
                        // Add left one
                        bins[axis][j].bounds.Expand(leftref.bounds);
                        // Save right to add part of it into the next bin
                        tempref = rightref;
                    }
                }

                // Add the last piece into the last bin
                bins[axis][(int)lastbin[axis]].bounds.Expand(tempref.bounds);
                // Adjust enter & exit counters
                bins[axis][(int)firstbin[axis]].enter++;
                bins[axis][(int)lastbin[axis]].exit++;
            }
        }

        // Prepare moving window data
		Bounds3D rightbounds[kNumBins - 1];
        split.sah = std::numeric_limits<float>::max();

        // Iterate over axis
        for (int axis = 0; axis < 3; ++axis)
        {
            // Skip if the extent is degenerate in that direction
			if (extents[axis] == 0.f) {
				continue;
			}
            
            // Start with 1-bin right box
			Bounds3D rightbox = Bounds3D();
            for (int i = kNumBins - 1; i > 0; --i)
            {
                rightbox = Bounds3D::Union(rightbox, bins[axis][i].bounds);
                rightbounds[i - 1] = rightbox;
            }

			Bounds3D leftbox = Bounds3D();
            int leftcount = 0;
            int rightcount = req.numprims;

            // Start moving border to the right
            for (int i = 1; i < kNumBins; ++i)
            {
                // New left box
                leftbox.Expand(bins[axis][i - 1].bounds);
                // New left box count
                leftcount += bins[axis][i - 1].enter;
                // Adjust right box
                rightcount -= bins[axis][i - 1].exit;
                // Calc SAH
                float sah = m_TraversalCost + (leftbox.Area() * rightbounds[i - 1].Area() * rightcount)  * invarea;

                // Update SAH if it is needed
                if (sah < split.sah)
                {
                    split.sah = sah;
                    split.dim = axis;
                    split.split = origin[axis] + binsize[axis] * (float)i;

                    // For spatial split overlap is zero by design
                    split.overlap = 0.f;
                }
            }
        }

        return split;
    }

    bool SplitBvh::SplitPrimRef(PrimRef const& ref, int axis, float split, PrimRef& leftref, PrimRef& rightref) const
    {
        // Start with left and right refs equal to original ref
        leftref.idx    = rightref.idx = ref.idx;
        leftref.bounds = rightref.bounds = ref.bounds;

        // Only split if split value is within our bounds range
        if (split > ref.bounds.min[axis] && split < ref.bounds.max[axis])
        {
            // Trim left box on the right
            leftref.bounds.max[axis]  = split;
            // Trim right box on the left
            rightref.bounds.min[axis] = split;
            return true;
        }

        return false;
    }

    void SplitBvh::SplitPrimRefs(SahSplit const& split, SplitRequest const& req, PrimRefArray& refs, int& extra_refs)
    {
        // We are going to append new primitives at the end of the array
        int appendprims = req.numprims;

        // Split refs if any of them require to be split
        for (int i = req.startidx; i < req.startidx + req.numprims; ++i)
        {
            PrimRef leftref, rightref;
            if (SplitPrimRef(refs[i], split.dim, split.split, leftref, rightref))
            {
                // Copy left ref instead of original
                refs[i] = leftref;
                // Append right one at the end
                refs[req.startidx + appendprims++] = rightref;
            }
        }

        // Return number of primitives after this operation
        extra_refs = appendprims - req.numprims;
    }

    SplitBvh::Node* SplitBvh::AllocateNode()
    {
        if (m_Nodecnt - m_NumNodesArchived >= m_NumNodesForRegular)
        {
			m_NodeArchive.push_back(std::move(m_Nodes));
            m_NumNodesArchived += m_NumNodesForRegular;
			m_Nodes = std::vector<Node>(m_NumNodesForRegular);
        }

        return &m_Nodes[m_Nodecnt++ - m_NumNodesArchived];
    }

    void SplitBvh::InitNodeAllocator(size_t maxnum)
    {
        m_NodeArchive.clear();
        m_Nodecnt = 0;
		m_Nodes.resize(maxnum);

        // Set root_ pointer
        m_Root = &m_Nodes[0];
    }

}

