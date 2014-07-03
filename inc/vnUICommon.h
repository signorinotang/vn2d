//
//  vnUICommon.h
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnUICommon_h
#define vn2d_vnUICommon_h

#include "vnAllocatedObject.h"
#include "vnVector2.h"

_vn_begin

enum TextAlignment {
	kTA_Default = 0,
    kTA_Left = 1,
    kTA_Right = 2,
    kTA_HorizontalCenter = 3,
    kTA_Top = 4,
    kTA_Bottom = 8,
    kTA_VerticalCenter = 12,
    kTA_Center = 15
};

enum KeyboardType {
    kKT_None = 0,
    kKT_Default = 1,
    kKT_ASCIICapable = 2
};

struct UITouch : AllocatedObject {
	long touchId;
	vector2f position;
};

_vn_end

#endif
