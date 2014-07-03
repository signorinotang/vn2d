//
//  vnUIFrame.h
//  vn2d
//
//  Created by Wayne on 14-1-14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnUIFrame_h
#define vn2d_vnUIFrame_h

#include "vnUIElement.h"

_vn_begin

class _vn_gfx_api UIFrame : public UIElement {
    VN_DECL_UI_ELEMENT()
public:
    virtual UIFrame * clone() const;
    virtual bool contains(const vector2f &point) const;
};

_vn_end


#endif
