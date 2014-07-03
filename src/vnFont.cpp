//
//  vnFont.cpp
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnFont.h"

_vn_begin

const Font::Glyph * Font::build(c32 chr) {
    return build(str32(&chr, 1));
}

_vn_end
