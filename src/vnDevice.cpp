//
//  vnDevice.cpp
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnDevice.h"

_vn_begin

Device * Device::ms_instance = 0;

Device::Device() {
    ms_instance = this;
}

Device::~Device() {
    ms_instance = 0;
}

Device & Device::instance() {
    vnassert(ms_instance);
    return *ms_instance;
}

void Device::drawQuad(const Quad &quad) {
    drawTriangles(2, quad.vertices, 4, Quad::kIndices);
}


_vn_end
