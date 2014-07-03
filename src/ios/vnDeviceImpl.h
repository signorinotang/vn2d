//
//  vnDeviceImpl.h
//  vn2d
//
//  Created by Wayne on 14-1-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __vn2d__vnDeviceImpl__
#define __vn2d__vnDeviceImpl__

#include "vnDevice.h"
#include "vnTextureImpl.h"

_vn_begin

class ApplicationImpl;

class DeviceImpl : public Device {
public:
    DeviceImpl(ApplicationImpl *appImpl);
    ~DeviceImpl();
    
    virtual const vector2f & scaleInPoint() const;
    virtual const vector2f & scaleInPixel() const;
    virtual const vector2f & size() const;
    
    virtual bool begin();
    virtual void end();
    virtual void clear();
    
    virtual void setTexture(const TexturePtr &texture);
    virtual void setBlendMode(BlendMode mode);
    virtual void setViewTransform(const matrix3 &mat);
    
    virtual void drawTriangles(size_t count, const Vertex *vertices, size_t numVertices, u16 *indices);
    
    virtual TexturePtr createTexture(const vector2i &size);
    virtual TexturePtr loadTexture(DataStream &ds, vector2i *original);
    
    virtual FontPtr createFont(u32 size);
    
    bool _init(const vector2i &size, const vector2i &sizeInPoint, const vector2i &sizeInPixel);
    
protected:
    enum {
        kMaxNumVertices = 256,
        kMaxNumIndices = 384,
    };
    
    ApplicationImpl *m_appImpl;
	GLuint m_program;
	GLuint m_vertexBuffer;
	GLuint m_indexBuffer;
	GLint m_projMatLocation;
	GLint m_viewMatLocation;
	Vertex *m_vertices;
	u16 *m_indices;
    
	size_t m_numVertices;
	size_t m_numIndices;
    
	RefCountedPtr<TextureImpl> m_currentTexture, m_nullTexture;
	BlendMode m_blendMode;
    
    vector2f m_size;
    vector2f m_scaleInPoint;
    vector2f m_scaleInPixel;
	
    void _setupGLES();
	void _flush();
	void _applyBlendMode();
};

_vn_end

#endif /* defined(__vn2d__vnDeviceImpl__) */
