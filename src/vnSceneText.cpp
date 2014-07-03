//
//  vnSceneText.cpp
//  vn2d
//
//  Created by Wayne on 14-2-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnSceneText.h"
#include "vnUnicode.h"
#include "vnUICommon.h"
#include "vnSceneNode.h"
#include "vnFontManager.h"


_vn_begin

SceneText::SceneText()
: m_alignment(0)
, m_blend(Device::kAlphaBlend)
, m_color(color_t::kWhite)
, m_font(FontManager::instance().getDefaultFont()) {
    
}

void SceneText::update() {
    m_string.update(m_font);
    f32 height = (m_font.null() ? 0 : m_font->height());
    vector2f &pen = m_offset;
    if (m_alignment) {
        switch (m_alignment & kTA_HorizontalCenter) {
            case kTA_HorizontalCenter:
                pen.x = (m_rect.max_corner.x + m_rect.min_corner.x - m_string.width()) * 0.5f;
                break;
                
            case kTA_Right:
                pen.x = m_rect.max_corner.x - m_string.width();
                break;
                
            default:
                pen.x = m_rect.min_corner.x;
                break;
        }
        switch (m_alignment & kTA_VerticalCenter) {
            case kTA_VerticalCenter:
                pen.y = (m_rect.max_corner.y + m_rect.min_corner.y - height) * 0.5f;
                break;
                
            case kTA_Bottom:
                pen.y = m_rect.max_corner.y - height;
                break;
                
            default:
                pen.y = m_rect.min_corner.y;
                break;
        }
    } else {
        pen = m_rect.min_corner;
    }
}

void SceneText::render() {
    Device &r = Device::instance();
    r.setBlendMode(m_blend);
    Quad quad;
    quad.setColor(m_color);
    if (m_sceneNode) {
        for (size_t i = 0, c = m_string.size(); i < c; ++i) {
            const FontString::Item *item = m_string[i];
            if (!item->glyph) {
                continue;
            }
            aabox2f rect = item->glyph->bounds;
            rect += m_offset;
            rect += vector2f(item->offset, 0);
            quad.setLocation(rect);
            quad.setTexcoords(item->glyph->texcoords);
            for (int i = 0; i < 4; ++i) {
                quad.vertices[i].position = m_sceneNode->convertToWorld(quad.vertices[i].position);
            }
            r.setTexture(item->glyph->texture);
            r.drawQuad(quad);
        }
    } else {
        for (size_t i = 0, c = m_string.size(); i < c; ++i) {
            const FontString::Item *item = m_string[i];
            if (!item->glyph) {
                continue;
            }
            aabox2f rect = item->glyph->bounds;
            rect += m_offset;
            rect += vector2f(item->offset, 0);
            quad.setLocation(rect);
            quad.setTexcoords(item->glyph->texcoords);
            r.setTexture(item->glyph->texture);
            r.drawQuad(quad);
        }
    }
}

SceneText & SceneText::operator <<(const FontPtr &font) {
    m_font = font;
    return *this;
}

SceneText & SceneText::operator <<(const aabox2f &rect) {
    m_rect = rect;
    return *this;
}

SceneText & SceneText::operator <<(const color_t &color) {
    m_color = color;
    return *this;
}

SceneText & SceneText::operator <<(Device::BlendMode blend) {
    m_blend = blend;
    return *this;
}

SceneText & SceneText::operator <<(const str8 &text) {
    str32 t;
    Unicode::convert(text.c_str(), text.length(), t);
    m_string.str(t);
    return *this;
}

SceneText & SceneText::operator <<(const str32 &text) {
    m_string.str(text);
    return *this;
}

SceneText & SceneText::operator <<(u32 alignment) {
    m_alignment = alignment;
    return *this;
}


_vn_end