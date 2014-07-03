//
//  vnTexture.h
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnTexture_h
#define vn2d_vnTexture_h

#include "vnRefCounted.h"
#include "vnVector2.h"
#include "vnColor.h"

_vn_begin

class _vn_2d_api Texture : public RefCounted {
public:
    Texture(const vector2i &_size);
    const vector2i & size() const;
    
    virtual bool updatePixels(const vector2i &offset, const vector2i &size, const color_t *pixels) = 0;
    
protected:
    vector2i m_size;
};

typedef RefCountedPtr<Texture> TexturePtr;

_vn_end

#endif
