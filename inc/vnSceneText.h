//
//  vnSceneText.h
//  vn2d
//
//  Created by Wayne on 14-2-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnSceneText_h
#define vn2d_vnSceneText_h

#include "vnSceneObject.h"
#include "vnFontString.h"
#include "vnDevice.h"

_vn_begin

class _vn_2d_api SceneText : public SceneObject {
public:
    SceneText();
    
    virtual void update();
    virtual void render();
    
    SceneText & operator <<(const FontPtr &font);
    SceneText & operator <<(const aabox2f &rect);
    SceneText & operator <<(const color_t &color);
	SceneText & operator <<(Device::BlendMode blend);
    SceneText & operator <<(const str8 &text);
    SceneText & operator <<(const str32 &text);
    SceneText & operator <<(u32 alignment);
    
protected:
    FontPtr m_font;
    FontString m_string;
    aabox2f m_rect;
    color_t m_color;
    Device::BlendMode m_blend;
    u32 m_alignment;
    vector2f m_offset;
};

_vn_end

#endif
