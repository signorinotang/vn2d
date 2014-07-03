//
//  vnFontImpl.h
//  vn2d
//
//  Created by Wayne on 14-1-10.
//  Copyright (c) 2014年 viichi.com. All rights reserved.
//

#ifndef __vn2d__vnFontImpl__
#define __vn2d__vnFontImpl__

#include "vnCellBasedFont.h"
#import <UIKit/UIKit.h>

_vn_begin

class FontImpl : public CellBasedFont {
public:
    FontImpl();
    virtual ~FontImpl();
    
    virtual f32 height() const;
    
    bool _create(f32 size, f32 scale);
    
protected:
    enum { kTexSize = 1024 };
    virtual void _build(const str32 &code, GlyphEx *glyph);
    
    UIFont *m_font;
    f32 m_height;
    f32 m_scale;
    u32 m_heightInPixel;
};

_vn_end

#endif /* defined(__vn2d__vnFontImpl__) */
