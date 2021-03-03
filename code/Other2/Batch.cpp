#include "stdafx.h"
#include "Profiler.h"
#include "Texture.h"
#include "Batch.h"
#include "Engine/DebugNew.h"

inline bool CompareBatchState(Batch& lhs, Batch& rhs)
{
    return lhs.sortKey < rhs.sortKey;
}

inline bool CompareBatchDistanceFrontToBack(Batch& lhs, Batch& rhs)
{
    return lhs.distance < rhs.distance;
}

inline bool CompareBatchDistanceBackToFront(Batch& lhs, Batch& rhs)
{
    return lhs.distance > rhs.distance;
}

void BatchQueue::Clear()
{
    batches.Clear();
    additiveBatches.Clear();
}

void BatchQueue::Sort(Vector<Matrix3x4>& instanceTransforms)
{
    switch (sort)
    {
    case SORT_STATE:
        ::Sort(batches.Begin(), batches.End(), CompareBatchState);
        ::Sort(additiveBatches.Begin(), additiveBatches.End(), CompareBatchState);
        break;

    case SORT_FRONT_TO_BACK:
        ::Sort(batches.Begin(), batches.End(), CompareBatchDistanceFrontToBack);
        // After drawing the base batches, the Z buffer has been prepared. Additive batches can be sorted per state now
        ::Sort(additiveBatches.Begin(), additiveBatches.End(), CompareBatchState);
        break;

    case SORT_BACK_TO_FRONT:
        ::Sort(batches.Begin(), batches.End(), CompareBatchDistanceBackToFront);
        ::Sort(additiveBatches.Begin(), additiveBatches.End(), CompareBatchDistanceBackToFront);
        break;

    default:
        break;
    }

    // Build instances where adjacent batches have same state
    BuildInstances(batches, instanceTransforms);
    BuildInstances(additiveBatches, instanceTransforms);
}

void BatchQueue::BuildInstances(Vector<Batch>& batches, Vector<Matrix3x4>& instanceTransforms)
{
    Batch* start = nullptr;
    for (auto it = batches.Begin(), end = batches.End(); it != end; ++it)
    {
        Batch* current = &*it;
        if (start && current->type == GEOM_STATIC && current->pass == start->pass && current->geometry == start->geometry &&
            current->lights == start->lights)
        {
            if (start->type == GEOM_INSTANCED)
            {
                instanceTransforms.Push(*current->worldMatrix);
                ++start->instanceCount;
            }
            else
            {
                // Begin new instanced batch
                start->type = GEOM_INSTANCED;
                size_t instanceStart = instanceTransforms.Size();
                instanceTransforms.Push(*start->worldMatrix);
                instanceTransforms.Push(*current->worldMatrix);
                start->instanceStart = instanceStart; // Overwrites non-instance world matrix
                start->instanceCount = 2; // Overwrites sort key / distance
            }
        }
        else
            start = (current->type == GEOM_STATIC) ? current : nullptr;
    }
}

ShadowMap::ShadowMap()
{
    // Construct texture but do not define its size yet
    texture = new Texture2();
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::Clear()
{
    allocator.Reset(texture->Width(), texture->Height(), 0, 0, false);
    shadowViews.Clear();
    used = false;
}

void ShadowView::Clear()
{
    shadowQueue.Clear();
}