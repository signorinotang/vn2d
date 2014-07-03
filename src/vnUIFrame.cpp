//
//  vnUIFrame.cpp
//  vn2d
//
//  Created by Wayne on 14-1-14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnUIFrame.h"

_vn_begin

UIFrame * UIFrame::clone() const {
    return vnnew UIFrame(*this);
}

bool UIFrame::contains(const vector2f &point) const {
    return false;
}

_vn_end
