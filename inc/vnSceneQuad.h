//
//  vnSceneQuad.h
//  vn2d
//
//  Created by Wayne on 14-1-15.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnSceneQuad_h
#define vn2d_vnSceneQuad_h

#include "vnSceneObject.h"
#include "vnDevice.h"
#include "vnPicture.h"

_vn_begin

class _vn_2d_api SceneQuad : public SceneObject {
public:
    SceneQuad();
    
	virtual void update();
    virtual void render();
    
    SceneQuad & operator <<(const vector2f *corners);
    SceneQuad & operator <<(const aabox2f &rect);
    SceneQuad & operator <<(const color_t &color);
    SceneQuad & operator <<(const Picture *picture);
	SceneQuad & operator <<(Device::BlendMode blend);
	
protected:
	virtual void _onTransformUpdated();
	vector2f m_corners[4];
    Quad m_quad;
    TexturePtr m_texture;
    Device::BlendMode m_blend;
	bool m_dirty;
};

_vn_end

#endif
