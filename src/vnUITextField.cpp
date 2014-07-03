//
//  vnUITextField.cpp
//  vn2d
//
//  Created by Wayne on 2/24/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnUITextField.h"
#include "vnUIRoot.h"
#include "vnUnicode.h"
#include "vnFontManager.h"

_vn_begin

UITextField::UITextField()
: m_textAlignment(0)
, m_textColor(color_t::kWhite)
, m_markedColor(color_t::kBlue)
, m_hasFocus(false)
, m_asciiOnly(true)
, m_caretTimer(0) {
    m_clipping = true;
}

UITextField * UITextField::clone() const {
    return vnnew UITextField(*this);
}


u32 UITextField::getTextAlignment() const {
    return m_textAlignment;
}

void UITextField::setTextAlignment(u32 alignment) {
    m_textAlignment = alignment;
}

void UITextField::setFont(const FontPtr &font) {
    m_font = font;
}

const FontPtr & UITextField::getFont() const {
    return m_font;
}

void UITextField::setText(const str8 &text) {
    str32 t;
    Unicode::convert(text.c_str(), text.length(), t);
    m_text.str(t);
}

void UITextField::setText(const str32 &text) {
    m_text.str(text);
}

const str32 & UITextField::getText() const {
    return m_text.str();
}

void UITextField::setTextColor(const color_t &color) {
    m_textColor = color;
}

const color_t & UITextField::getTextColor() const {
    return m_textColor;
}

void UITextField::setMarkedColor(const color_t &color) {
    m_markedColor = color;
}

const color_t & UITextField::getMarkedColor() const {
    return m_markedColor;
}

void UITextField::setASCIIOnly(bool asciiOnly) {
    m_asciiOnly = asciiOnly;
}

bool UITextField::isASCIIOnly() const {
    return m_asciiOnly;
}

// methods
void UITextField::init(const TreeDataObject *object) {
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
    
    m_markedColor.convertFromBGRA(object->queryAttributeInteger("marked_color", m_markedColor.convertToBGRA()));
    
    m_asciiOnly = object->queryAttributeBoolean("ascii_only", m_asciiOnly);
}

KeyboardType UITextField::keyboardType() const {
    if (m_asciiOnly) {
        return kKT_ASCIICapable;
    }
    return kKT_Default;
}

// events
bool UITextField::onTouchBegan(UIElement *target, const UITouch *touch) {
    UIRoot::instance().requireFocus(this);
    return true;
}

void UITextField::onInsertText(const str32 &text) {
#if VN_PLATFORM == VN_PLATFORM_WIN
	if (text[0] == '\r' || text[0] == '\n') {
#else
    if (text[0] == '\n') {
#endif
        _onNewLine();
        UIRoot::instance().releaseFocus(this);
        return ;
    }
    m_text.append(text);
}

void UITextField::onDeleteBackward() {
    m_text.pop_back();
}

void UITextField::onMarkText(const str32 &text) {
    m_markedText.str(text);
}

void UITextField::onUnmarkText() {
    m_markedText.clear();
}

void UITextField::onFocusIn() {
    m_hasFocus = true;
    m_caretTimer = 0;
    if (!m_fnFocusIn.null()) {
        m_fnFocusIn(this);
    }
}

void UITextField::onFocusOut() {
    m_hasFocus = false;
    if (!m_fnFocusOut.null()) {
        m_fnFocusOut(this);
    }
}

void UITextField::bindAction_NewLine(const function<void(UIElement *, const str32 &)> &func) {
    m_fnNewLine = func;
}

void UITextField::bindAction_FocusIn(const function<void(UIElement *)> &func) {
    m_fnFocusIn = func;
}

void UITextField::bindAction_FocusOut(const function<void(UIElement *)> &func) {
    m_fnFocusOut = func;
}

void UITextField::_onUpdate(f32 deltaTime) {
    m_text.update(m_font);
    m_markedText.update(m_font);
    f32 height = (m_font.null() ? 0 : m_font->height());
    f32 width = m_text.width() + m_markedText.width() + 1;
    
    u32 alignment = m_textAlignment;
    if (width > m_boundingBox.max_corner.x - m_boundingBox.min_corner.x) {
        alignment &= ~(u32)kTA_Left;
        alignment |= kTA_Right;
    }
    vector2f pen;

    switch (alignment & kTA_HorizontalCenter) {
        case kTA_HorizontalCenter:
            pen.x = (m_boundingBox.max_corner.x + m_boundingBox.min_corner.x - width) * 0.5f;
            break;
            
        case kTA_Right:
            pen.x = m_boundingBox.max_corner.x - width;
            break;
            
        default:
            pen.x = m_boundingBox.min_corner.x;
            break;
    }
    switch (alignment & kTA_VerticalCenter) {
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
    m_textPosition = pen;
    if (!m_markedText.empty()) {
        m_markedTextPosition = pen;
        m_markedTextPosition.x += m_text.width();
        m_markedRect.min_corner = m_markedTextPosition;
        m_markedRect.max_corner = m_markedTextPosition + vector2f(m_markedText.width(), height);
    }
    if (m_hasFocus) {
        m_caretTimer += deltaTime;
        if (m_caretTimer >= 1.f) {
            m_caretTimer -= 1.f;
        }
        m_caretRect.min_corner.set(pen.x + width - 1, pen.y);
        m_caretRect.max_corner = m_caretRect.min_corner + vector2f(1, height);
    }
}

void UITextField::_onRender(UIRenderer *renderer) {
    for (size_t i = 0, c = m_text.size(); i < c; ++i) {
        const FontString::Item *item = m_text[i];
        if (!item->glyph) {
            continue;
        }
        aabox2f rect = item->glyph->bounds;
        rect += m_textPosition;
        rect += vector2f(item->offset, 0);
        renderer->drawRect(rect, m_textColor, item->glyph->texture, item->glyph->texcoords);
    }
    if (!m_markedText.empty()) {
        renderer->drawRect(m_markedRect, m_markedColor);
        for (size_t i = 0, c = m_markedText.size(); i < c; ++i) {
            const FontString::Item *item = m_markedText[i];
            if (!item->glyph) {
                continue;
            }
            aabox2f rect = item->glyph->bounds;
            rect += m_markedTextPosition;
            rect += vector2f(item->offset, 0);
			renderer->drawRect(rect, m_textColor, item->glyph->texture, item->glyph->texcoords);
        }
    }
    if (m_hasFocus && m_caretTimer < 0.5f) {
        renderer->drawRect(m_caretRect, m_textColor);
    }
}

bool UITextField::_bindAction(const c8 *name, RefCounted *func_impl) {
    VN_UI_ELEMENT_BIND_ACTION(UITextField, "newline", m_fnNewLine, name, func_impl);
    VN_UI_ELEMENT_BIND_ACTION(UITextField, "focus_in", m_fnFocusIn, name, func_impl);
    VN_UI_ELEMENT_BIND_ACTION(UITextField, "focus_out", m_fnFocusOut, name, func_impl);
    return UIElement::_bindAction(name, func_impl);
}

void UITextField::_onNewLine() {
    if (!m_fnNewLine.null()) {
        m_fnNewLine(this, m_text.str());
    }
}

_vn_end
