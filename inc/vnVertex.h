//
//  vnVertex.h
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnVertex_h
#define vn2d_vnVertex_h

#include "vnAllocatedObject.h"
#include "vnVector2.h"
#include "vnColor.h"

_vn_begin

struct Vertex : AllocatedObject {
    vector2f position;
    color_t color;
    vector2f texcoord;
};

_vn_end


#endif
