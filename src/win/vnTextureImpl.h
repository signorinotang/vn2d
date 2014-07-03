//  vnTextureImpl.h
//  vn2d
//
//  Created by ts on 14-1-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __vn2d__vnTextureImpl__
#define __vn2d__vnTextureImpl__

#include "vnTexture.h"
#include <d3d9.h>

_vn_begin

class TextureImpl : public Texture {
public:
	TextureImpl(const vector2i& _size);
	~TextureImpl();

	virtual bool updatePixels(const vector2i &offset, const vector2i &size, const color_t *pixels);
	bool _init(IDirect3DDevice9* device, const void *pixels);

	
	bool createTexture(IDirect3DDevice9* device);

	u32* lock(u32 &pitch, bool readOnly = false);
	u32* lock(u32 left, u32 top, u32 right, u32 bottom, u32 &pitch, bool readOnly);
	void unlock();

	IDirect3DTexture9 *m_texture;
};



_vn_end

#endif
