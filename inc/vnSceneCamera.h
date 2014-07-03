//
//  vnSceneCamera.h
//  vn2d
//
//  Created by Wayne on 14-1-15.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnSceneCamera_h
#define vn2d_vnSceneCamera_h

#include "vnRefCounted.h"
#include "vnMatrix3.h"
#include "vnAABox2.h"

_vn_begin

class _vn_2d_api SceneCamera : public RefCounted {
public:
    SceneCamera();
    
    void update(f32 deltaTime);
    void move(const vector2f &delta);
    
    void setPosition(const vector2f &pos);
    const vector2f & getPosition() const;
    void setZoom(f32 zoom);
    vn::f32 getZoom() const;
    
    void setViewSize(const vector2f &size);
    
    const vn::matrix3 & viewMatrix() const;
    const vn::aabox2f & boundingBox() const;
    
    vn::vector2f convertToView(const vector2f &point);
    vn::vector2f convertFromView(const vector2f &point);
    
    void shake(f32 time, f32 range = 1.f);
    void cleanDirty();
protected:
    vn::matrix3 m_viewMatrix;
    vn::vector2f m_position;
    vn::f32 m_zoom;
    vn::vector2f m_viewSize;
    vn::aabox2f m_boundingBox;
    bool m_dirty;
    
    void _updateViewMatrix();
    void _updateBoundingBox();
    
    s32 m_shakeCount;
    f32 m_shakeRange;
    f32 m_shakeTimer;
    vector2f m_shakeOffset;
    
    void _updateShakeOffset();
};

_vn_end

#endif
