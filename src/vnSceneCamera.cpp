//
//  vnSceneCamera.cpp
//  vn2d
//
//  Created by Wayne on 14-1-15.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnSceneCamera.h"

_vn_begin

SceneCamera::SceneCamera()
: m_viewMatrix(matrix3::c_identity)
, m_zoom(1)
, m_dirty(false)
, m_shakeRange(0)
, m_shakeTimer(0)
, m_shakeCount(0) {
    
}

void SceneCamera::update(f32 deltaTime) {
    bool updateViewMat = false;
    if (m_dirty) {
        m_dirty = false;
        updateViewMat = true;
    }
    if (m_shakeTimer) {
        updateViewMat = true;
        m_shakeTimer -= deltaTime;
        if (m_shakeTimer <= 0.f) {
            m_shakeTimer = 0.f;
            m_shakeOffset = vector2f::kZero;
        } else {
            _updateShakeOffset();
            m_shakeCount = (m_shakeCount + 1) % 16;
        }
    }
    
    if (updateViewMat) {
        _updateViewMatrix();
        m_boundingBox.min_corner = convertFromView(vector2f::kZero);
        m_boundingBox.max_corner = convertFromView(m_viewSize);
    }
}

void SceneCamera::move(const vector2f &delta) {
    m_position += delta / m_zoom;
    m_position = vector2i(m_position);
    m_dirty = true;
}

void SceneCamera::setPosition(const vector2f &pos) {
    m_position = vector2i(pos);
    m_dirty = true;
}

const vector2f & SceneCamera::getPosition() const {
    return m_position;
}

void SceneCamera::setZoom(f32 zoom) {
    vnassert(zoom);
    m_zoom = zoom;
    m_dirty = true;
}

f32 SceneCamera::getZoom() const {
    return m_zoom;
}

void SceneCamera::setViewSize(const vector2f &size) {
    m_viewSize = size;
    if (m_dirty) {
        m_dirty = false;
        _updateViewMatrix();
    }
    m_boundingBox.max_corner = convertFromView(size);
}

const matrix3 & SceneCamera::viewMatrix() const {
    return m_viewMatrix;
}

const aabox2f & SceneCamera::boundingBox() const {
    return m_boundingBox;
}

vector2f SceneCamera::convertToView(const vector2f &point) {
    return point * m_zoom + vector2f(m_viewMatrix.m31, m_viewMatrix.m32);
}

vector2f SceneCamera::convertFromView(const vector2f &point) {
    return (point - vector2f(m_viewMatrix.m31, m_viewMatrix.m32)) / m_zoom;
}

void SceneCamera::shake(f32 time, f32 range) {
    if (time > m_shakeTimer) {
        m_shakeTimer = time;
    }
    m_shakeRange = range;
}

void SceneCamera::cleanDirty() {
    if (m_dirty) {
        m_dirty = false;
        _updateViewMatrix();
        m_boundingBox.min_corner = convertFromView(vector2f::kZero);
        m_boundingBox.max_corner = convertFromView(m_viewSize);
    }
}

void SceneCamera::_updateViewMatrix() {
    m_viewMatrix.m11 = m_zoom;
    m_viewMatrix.m22 = m_zoom;
    vector2f off = m_viewSize / 2;
    vector2f t = m_position + off;
    t *= m_zoom;
    t -= off;
    m_viewMatrix.m31 = -t.x + m_shakeOffset.x;
    m_viewMatrix.m32 = -t.y + m_shakeOffset.y;
}

void SceneCamera::_updateShakeOffset() {
    switch (m_shakeCount) {
        case 0:
            m_shakeOffset.set(-m_shakeRange, -m_shakeRange);
            break;
            
        case 1:
            m_shakeOffset.set(m_shakeRange, m_shakeRange);
            break;
            
        case 2:
            m_shakeOffset.set(0, -m_shakeRange);
            break;
            
        case 3:
            m_shakeOffset.set(0, m_shakeRange);
            break;
            
        case 4:
            m_shakeOffset.set(m_shakeRange, m_shakeRange);
            break;
            
        case 5:
            m_shakeOffset.set(-m_shakeRange, m_shakeRange);
            break;
            
        case 6:
            m_shakeOffset.set(m_shakeRange, 0);
            break;
            
        case 7:
            m_shakeOffset.set(-m_shakeRange, 0);
            break;
            
        case 8:
            m_shakeOffset.set(m_shakeRange, m_shakeRange);
            break;
            
        case 9:
            m_shakeOffset.set(-m_shakeRange, -m_shakeRange);
            break;
            
        case 10:
            m_shakeOffset.set(0, m_shakeRange);
            break;
            
        case 11:
            m_shakeOffset.set(0, -m_shakeRange);
            break;
            
        case 12:
            m_shakeOffset.set(-m_shakeRange, m_shakeRange);
            break;
            
        case 13:
            m_shakeOffset.set(m_shakeRange, -m_shakeRange);
            break;
            
        case 14:
            m_shakeOffset.set(-m_shakeRange, 0);
            break;
            
        case 15:
            m_shakeOffset.set(m_shakeRange, 0);
            break;
    }
}

_vn_end
