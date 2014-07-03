//
//  vnApplicationImpl.cpp
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnApplicationImpl.h"

_vn_begin

ApplicationImpl::ApplicationImpl()
: m_type(Application::kType_iPad)
, m_scale(1)
, m_viewSize(1024, 768) {
    
}

ApplicationImpl::~ApplicationImpl() {
    
}

int ApplicationImpl::main(Application::Delegate *delegate) {
    
    return 0;
}

Application::Type ApplicationImpl::type() {
    return m_type;
}

f32 ApplicationImpl::scale() {
    return m_scale;
}

const vector2i & ApplicationImpl::viewSize() {
    return m_viewSize;
}

Device * ApplicationImpl::createDevice(const vector2f &size) {
    return 0;
}


_vn_end