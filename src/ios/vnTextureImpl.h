//
//  vnTextureImpl.h
//  vn2d
//
//  Created by Wayne on 14-1-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __vn2d__vnTextureImpl__
#define __vn2d__vnTextureImpl__

#include "vnTexture.h"
#include <OpenGLES/ES2/gl.h>

_vn_begin

class TextureImpl : public Texture {
public:
    TextureImpl(const vector2i &_size);
    ~TextureImpl();
    
    virtual bool updatePixels(const vector2i &offset, const vector2i &size, const color_t *pixels);
    
    bool _init(const void *pixels);
    void _bind();
    
protected:
    GLuint m_texture;
};

_vn_end

#endif /* defined(__vn2d__vnTextureImpl__) */
