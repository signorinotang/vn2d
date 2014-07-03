//
//  vnDevice.h
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnDevice_h
#define vn2d_vnDevice_h

#include "vnQuad.h"
#include "vnTexture.h"
#include "vnDataStream.h"
#include "vnMatrix3.h"
#include "vnFont.h"

_vn_begin

class _vn_2d_api Device : public AllocatedObject {
public:
    enum BlendMode {
        kAlphaBlend = 0,
        kAlphaAdditive
    };
    
    static Device & instance();
    Device();
    ~Device();
    
    virtual const vector2f & scaleInPoint() const = 0;
    virtual const vector2f & scaleInPixel() const = 0;
    virtual const vector2f & size() const = 0;
    
    virtual bool begin() = 0;
    virtual void end() = 0;
    virtual void clear() = 0;
    
    virtual void setTexture(const TexturePtr &texture) = 0;
    virtual void setBlendMode(BlendMode mode) = 0;
    virtual void setViewTransform(const matrix3 &mat) = 0;
    
    virtual void drawTriangles(size_t count, const Vertex *vertices, size_t numVertices, u16 *indices) = 0;
    
    virtual TexturePtr createTexture(const vector2i &size) = 0;
    virtual TexturePtr loadTexture(DataStream &ds, vector2i *original = 0) = 0;
    
    virtual FontPtr createFont(u32 size) = 0;
    
    void drawQuad(const Quad &quad);
    
protected:
    static Device *ms_instance;
};

_vn_end


#endif
