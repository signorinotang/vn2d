//
//  vnFont.h
//  vngfx
//
//  Created by Wayne on 13-12-16.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnFont_h
#define vngfx_vnFont_h

#include "vnTexture.h"
#include "vnAABox2.h"

_vn_begin

class _vn_gfx_api Font : public RefCounted {
public:
    struct Glyph {
        bool valid;
        aabox2f bounds;
        vector2f size;
        vector2f texcoords[4];
        TexturePtr texture;
    };
    
    virtual const Glyph * build(const str32 &code) = 0;
    virtual f32 height() const = 0;
    virtual void discard() = 0;
    
    const Glyph * build(c32 chr);
};

typedef RefCountedPtr<Font> FontPtr;

_vn_end

#endif
