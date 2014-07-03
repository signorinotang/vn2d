//
//  vnQuad.cpp
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnQuad.h"

_vn_begin

u16 Quad::kIndices[6] = {0, 1, 2, 0, 2, 3};

Quad & Quad::setLocation(const aabox2f &rect) {
    vertices[0].position = rect.corner_0();
    vertices[1].position = rect.corner_1();
    vertices[2].position = rect.corner_2();
    vertices[3].position = rect.corner_3();
    return *this;
}

Quad & Quad::setColor(const color_t &color) {
    vertices[0].color = color;
    vertices[1].color = color;
    vertices[2].color = color;
    vertices[3].color = color;
    return *this;
}

Quad & Quad::setTexcoords(const aabox2f &rect) {
    vertices[0].texcoord = rect.corner_0();
    vertices[1].texcoord = rect.corner_1();
    vertices[2].texcoord = rect.corner_2();
    vertices[3].texcoord = rect.corner_3();
    return *this;
}

Quad & Quad::setTexcoords(const vector2f texcoords[4]) {
	vertices[0].texcoord = texcoords[0];
	vertices[1].texcoord = texcoords[1];
	vertices[2].texcoord = texcoords[2];
	vertices[3].texcoord = texcoords[3];
	return *this;
}

_vn_end
