#ifndef OBJECTS_SLOT_FRAME_H
#define OBJECTS_SLOT_FRAME_H

#include "Frame.h"
#include "../geoms/ColorTransform.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class SlotFrame : public Frame
{
public:
	float tweenEasing;
	int displayIndex;
	bool visible;
	float zOrder;
	ColorTransform *color;

public:
	SlotFrame() :
		tweenEasing(10)
		,displayIndex(0)
		,visible(true)
		,zOrder(0.f)
		,color(nullptr)
	{}

	virtual ~SlotFrame()
	{
		dispose();
	}

	virtual void dispose()
	{
		Frame::dispose();
		_dispose();
	}

private:
	void _dispose()
	{
		if (color)
		{
			delete color;
			color = nullptr;
		}
	}
};
NAME_SPACE_DRAGON_BONES_END
#endif OBJECTS_SLOT_FRAME_H
