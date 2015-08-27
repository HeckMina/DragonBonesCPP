﻿#ifndef DRAGONBONES_OBJECTS_TRANSFORM_TIMELINE_H
#define DRAGONBONES_OBJECTS_TRANSFORM_TIMELINE_H

#include "dragonbones/DragonBones.h"
#include "dragonbones/geoms/Point.h"
#include "dragonbones/geoms/Transform.h"
#include "Timeline.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class TransformTimeline : public Timeline
{    
public:
    TransformTimeline()
    {
        offset = 0.f;
        transformed = false;
    }
    TransformTimeline(const TransformTimeline &copyData)
    {
        operator=(copyData);
    }
    TransformTimeline& operator=(const TransformTimeline &copyData)
    {
        Timeline::operator=(copyData);
        //_dispose();
        transformed = copyData.transformed;
        offset = copyData.offset;
        name = copyData.name;
        originTransform = copyData.originTransform;
        originPivot = copyData.originPivot;
        return *this;
    }
    virtual ~TransformTimeline()
    {
        dispose();
    }
    
    virtual void dispose()
    {
        Timeline::dispose();
        //_dispose();
    }

public:
	bool transformed;
	float offset;

	std::string name;
	Transform originTransform;
	Point originPivot;
};
NAME_SPACE_DRAGON_BONES_END
#endif  // DRAGONBONES_OBJECTS_TRANSFORM_TIMELINE_H
