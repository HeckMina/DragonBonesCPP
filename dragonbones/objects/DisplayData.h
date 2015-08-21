﻿#ifndef OBJECTS_DISPLAY_DATA_H
#define OBJECTS_DISPLAY_DATA_H

#include "../DragonBones.h"
#include "../geoms/Point.h"
#include "../geoms/Transform.h"
#include "../objects/TextData.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DisplayData
{
public:
    //
    //bool scalingGrid;
    //int scalingGridLeft;
    //int scalingGridRight;
    //int scalingGridTop;
    //int scalingGridBottom;
	//TextData *textData;

    std::string name;
	std::string slotName;
    DisplayType type;
    Transform transform;
	Point pivot;

public:
    DisplayData():
        //scalingGrid(false),
        //scalingGridLeft(0),
        //scalingGridRight(0),
        //scalingGridTop(0),
        //scalingGridBottom(0),
		//,textData(nullptr)

        name("")
		,slotName("")
        ,type(DisplayType::DT_IMAGE)
    {}
    DisplayData(const DisplayData &copyData)
    {
        operator=(copyData);
    }
    DisplayData& operator=(const DisplayData &copyData)
    {
        dispose();

        //scalingGrid = copyData.scalingGrid;
        //scalingGridLeft = copyData.scalingGridLeft;
        //scalingGridRight = copyData.scalingGridRight;
        //scalingGridTop = copyData.scalingGridTop;
        //scalingGridBottom = copyData.scalingGridBottom;

        name = copyData.name;
        type = copyData.type;
        transform = copyData.transform;
        
		pivot = copyData.pivot;
		//if (copyData.textData)
		//{
		//	textData = new TextData(); 
		//	*textData = *(copyData.textData);
		//}

        return *this;
    }
    virtual ~DisplayData() 
    {
        dispose();
    }

    void dispose()
    {
        //scalingGrid = false;
        //scalingGridLeft = 0;
        //scalingGridRight = 0;
        //scalingGridTop = 0;
        //scalingGridBottom = 0;

        name.clear();
        type = DisplayType::DT_IMAGE;
  //      pivot.clear();
		//transform.clear();

        //if (textData)
        //{
        //    delete textData;
        //    textData = nullptr;
        //}
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // OBJECTS_DISPLAY_DATA_H
