//
//  vnTextureImpl.cpp
//  vn2d
//
//  Created by Wayne on 14-1-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnTextureImpl.h"

_vn_begin

TextureImpl::TextureImpl(const vector2i &_size)
: Texture(_size)
, m_texture(0) {
    
}

TextureImpl::~TextureImpl() {
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
}

bool TextureImpl::updatePixels(const vector2i &offset, const vector2i &size, const color_t *pixels) {
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexSubImage2D (GL_TEXTURE_2D, 0, offset.x, offset.y, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    return true;
}

bool TextureImpl::_init(const void *pixels) {
    if (m_size.x <= 0 || m_size.y <= 0) {
        return false;
    }
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    return true;
}

void TextureImpl::_bind() {
    glBindTexture(GL_TEXTURE_2D, m_texture);
}


_vn_end