//
//  vnSceneQuad.cpp
//  vn2d
//
//  Created by Wayne on 14-1-15.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnSceneQuad.h"
#include "vnSceneNode.h"

_vn_begin

SceneQuad::SceneQuad()
: m_dirty(false)
, m_blend(Device::kAlphaBlend) {
    m_quad.setColor(color_t::kWhite);
}

void SceneQuad::update() {
	if (m_dirty) {
		m_dirty = false;
		if (m_sceneNode) {
			m_quad.vertices[0].position = m_sceneNode->convertToWorld(m_corners[0]);
			m_quad.vertices[1].position = m_sceneNode->convertToWorld(m_corners[1]);
			m_quad.vertices[2].position = m_sceneNode->convertToWorld(m_corners[2]);
			m_quad.vertices[3].position = m_sceneNode->convertToWorld(m_corners[3]);
		} else {
			m_quad.vertices[0].position = m_corners[0];
			m_quad.vertices[1].position = m_corners[1];
			m_quad.vertices[2].position = m_corners[2];
			m_quad.vertices[3].position = m_corners[3];
		}
	}
}

void SceneQuad::render() {
    Device &r = Device::instance();
    r.setBlendMode(m_blend);
    r.setTexture(m_texture);
    r.drawQuad(m_quad);
}

SceneQuad & SceneQuad::operator<<(const vector2f *corners) {
    m_corners[0] = corners[0];
    m_corners[1] = corners[1];
    m_corners[2] = corners[2];
    m_corners[3] = corners[3];
    m_dirty = true;
    return *this;
}

SceneQuad & SceneQuad::operator <<(const aabox2f &rect) {
    rect.build_corners(m_corners);
	m_dirty = true;
	return *this;
}

SceneQuad & SceneQuad::operator <<(const color_t &color) {
    m_quad.setColor(color);
	return *this;
}

SceneQuad & SceneQuad::operator <<(const Picture *picture) {
    if (picture) {
        m_texture = picture->texture;
        m_quad.vertices[0].texcoord = picture->texcoords[0];
        m_quad.vertices[1].texcoord = picture->texcoords[1];
        m_quad.vertices[2].texcoord = picture->texcoords[2];
        m_quad.vertices[3].texcoord = picture->texcoords[3];
    } else {
        m_texture.release();
    }
	return *this;
}

SceneQuad & SceneQuad::operator <<(Device::BlendMode blend) {
	m_blend = blend;
	return *this;
}

void SceneQuad::_onTransformUpdated() {
	m_dirty = true;
}

_vn_end
