//
//  vnQuad.h
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnQuad_h
#define vn2d_vnQuad_h

#include "vnVertex.h"
#include "vnAABox2.h"

_vn_begin

struct _vn_2d_api Quad {
    Vertex vertices[4];
    
    Quad & setLocation(const aabox2f &rect);
    Quad & setColor(const color_t &color);
    Quad & setTexcoords(const aabox2f &rect);
	Quad & setTexcoords(const vector2f texcoords[4]);
    
    static u16 kIndices[6];
};

_vn_end


#endif
