//
//  vnTexture.cpp
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnTexture.h"

_vn_begin

Texture::Texture(const vector2i &_size)
: m_size(_size) {
    
}

const vector2i & Texture::size() const {
    return m_size;
}


_vn_end
