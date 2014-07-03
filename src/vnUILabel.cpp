//
//  vnUILabel.cpp
//  vn2d
//
//  Created by Wayne on 14-1-14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnUILabel.h"
#include "vnUnicode.h"
#include "vnFontManager.h"
#include "vnUIRenderer.h"

_vn_begin

UILabel::UILabel()
: m_textAlignment(0)
, m_strokeEnabled(false)
, m_textColor(color_t::kWhite) {
    m_enabled = false;
}

UILabel * UILabel::clone() const {
    return vnnew UILabel(*this);
}

u32 UILabel::getTextAlignment() const {
    return m_textAlignment;
}

void UILabel::setTextAlignment(u32 alignment) {
    m_textAlignment = alignment;
}

void UILabel::setFont(const FontPtr &font) {
    m_font = font;
}

const FontPtr & UILabel::getFont() const {
    return m_font;
}

void UILabel::setText(const str8 &text) {
    str32 utf32;
    Unicode::convert(text.c_str(), text.length(), utf32);
    m_string.str(utf32);
}

void UILabel::setText(const str32 &text) {
    m_string.str(text);
}

void UILabel::setTextColor(const color_t &color) {
    m_textColor = color;
}

const color_t & UILabel::getTextColor() const {
    return m_textColor;
}

void UILabel::setFontStroke(bool enabled, color_t color) {
	if (enabled) {
		m_strokeEnabled = true;
		m_strokeColor = color;
	} else {
		m_strokeEnabled = false;
	}
}

FontString & UILabel::_string() {
    return m_string;
}

const FontString & UILabel::_string() const {
    return m_string;
}

void UILabel::init(const TreeDataObject *object) {
    UIElement::init(object);
    const TreeDataString *tdstr = object->queryAttributeString("font");
    if (tdstr) {
        m_font = FontManager::instance().getFont(tdstr->value());
    } else {
        m_font = FontManager::instance().getDefaultFont();
    }
    tdstr = object->queryAttributeString("text");
    if (tdstr) {
        setText(tdstr->value());
    }
    m_textColor.convertFromBGRA(object->queryAttributeInteger("color", m_textColor.convertToBGRA()));
    m_textAlignment = object->queryAttributeInteger("alignment");
}

void UILabel::_onUpdate(f32 deltaTime) {
    m_string.update(m_font);
    f32 height = (m_font.null() ? 0 : m_font->height());
    vector2f pen;
    if (m_textAlignment) {
        switch (m_textAlignment & kTA_HorizontalCenter) {
            case kTA_HorizontalCenter:
                pen.x = (m_boundingBox.max_corner.x + m_boundingBox.min_corner.x - m_string.width()) * 0.5f;
                break;
                
            case kTA_Right:
                pen.x = m_boundingBox.max_corner.x - m_string.width();
                break;
                
            default:
                pen.x = m_boundingBox.min_corner.x;
                break;
        }
        switch (m_textAlignment & kTA_VerticalCenter) {
            case kTA_VerticalCenter:
                pen.y = (m_boundingBox.max_corner.y + m_boundingBox.min_corner.y - height) * 0.5f;
                break;
                
            case kTA_Bottom:
                pen.y = m_boundingBox.max_corner.y - height;
                break;
                
            default:
                pen.y = m_boundingBox.min_corner.y;
                break;
        }
    } else {
        pen = m_boundingBox.min_corner;
    }
    m_textPosition = pen;
}

void UILabel::_onRender(UIRenderer *renderer) {
	if (m_strokeEnabled) {
		vector2f offset[5] = {
			{ 0.5f, 0 },
			{ 0, 0.5f },
			{ 0, -0.5f },
			{ -0.5f, 0 },
			{ 0, 1.f }
		};
		for (int k = 0; k < 5; ++k) {
			for (size_t i = 0, c = m_string.size(); i < c; ++i) {
				const FontString::Item *item = m_string[i];
				if (!item->glyph) {
					continue;
				}
				aabox2f rect = item->glyph->bounds;
				rect += m_textPosition;
				rect += vector2f(item->offset, 0) + offset[k];
				renderer->drawRect(rect, m_strokeColor, item->glyph->texture, item->glyph->texcoords);
			}
		}
	}
    for (size_t i = 0, c = m_string.size(); i < c; ++i) {
        const FontString::Item *item = m_string[i];
        if (!item->glyph) {
            continue;
        }
        aabox2f rect = item->glyph->bounds;
        rect += m_textPosition;
        rect += vector2f(item->offset, 0);
		renderer->drawRect(rect, m_textColor, item->glyph->texture, item->glyph->texcoords);
    }
}



_vn_end
