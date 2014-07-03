//
//  vnFontString.cpp
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnFontString.h"

_vn_begin

FontString::Item::Item()
: glyph(0)
, offset(0) {
    
}

FontString::Item::Item(const Item &other)
: code(other.code)
, glyph(0)
, offset(0) {
    
}

FontString::Item * FontString::Item::clone() const {
    return vnnew Item(*this);
}

FontString::Item::~Item() {
    
}

FontString::FontString() {
    
}

FontString::FontString(const FontString &other)
: m_string(other.m_string)
, m_items(other.m_items) {
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        *it = (*it)->clone();
    }
}

FontString::~FontString() {
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        delete *it;
    }
}

f32 FontString::width() const {
    return m_width;
}

bool FontString::empty() const {
    return m_items.empty();
}

size_t FontString::size() const {
    return m_items.size();
}

const FontString::Item * FontString::at(size_t index) const {
    return m_items[index];
}

const FontString::Item * FontString::operator [](size_t index) const {
    return m_items[index];
}

const str32 & FontString::str() const {
    return m_string;
}

void FontString::str(const str32 &_str) {
    clear();
    m_string = _str;
    _append(_str.c_str());
}

void FontString::append(const str32 &_str) {
    m_string += _str;
    _append(_str.c_str());
}

void FontString::pop_back() {
    if (m_items.empty()) {
        return ;
    }
    size_t len = m_items.back()->code.length();
    m_items.pop_back();
    m_string.erase(m_string.end() - len, m_string.end());
}

void FontString::clear() {
    m_string.clear();
    for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
        delete *it;
    }
    m_items.clear();
    m_width = 0;
}

void FontString::update(const FontPtr &font) {
    m_width = 0;
    if (font.null()) {
        for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
            (*it)->glyph = 0;
        }
    } else {
        for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
            Item *item = *it;
            item->glyph = font->build(item->code);
            item->offset = m_width;
            m_width += (item->glyph ? item->glyph->size.x : 0);
        }
    }
}

FontString::Item * FontString::_createItem(size_t index) {
    return vnnew Item();
}

void FontString::_append(const c32 *sz) {
    while (*sz) {
        size_t len = 1;
        if (sz[0] == 0x1F1E8 && sz[1] == 0x1F1F3) {
            len = 2;
        } else if (sz[0] == 0x1F1E9 && sz[1] == 0x1F1EA) {
            len = 2;
        } else if (sz[0] == 0x1F1EA && sz[1] == 0x1F1F8) {
            len = 2;
        } else if (sz[0] == 0x1F1EB && sz[1] == 0x1F1F7) {
            len = 2;
        } else if (sz[0] == 0x1F1EC && sz[1] == 0x1F1E7) {
            len = 2;
        } else if (sz[0] == 0x1F1EE && sz[1] == 0x1F1F9) {
            len = 2;
        } else if (sz[0] == 0x1F1EF && sz[1] == 0x1F1F5) {
            len = 2;
        } else if (sz[0] == 0x1F1F0 && sz[1] == 0x1F1F7) {
            len = 2;
        } else if (sz[0] == 0x1F1F7 && sz[1] == 0x1F1FA) {
            len = 2;
        } else if (sz[0] == 0x1F1FA && sz[1] == 0x1F1F8) {
            len = 2;
        } else if (sz[1] == 0x20E3) {
            len = 2;
        }
        if (sz[len] == 0xFE0F) {
            ++len;
        }
        
        Item *item = _createItem(m_items.size());
        item->code.assign(sz, len);
        m_items.push_back(item);
        sz += len;
    }
}

///////////////////////////////////////////////////

FontStringEx::Item::Item()
: top(0) {
    
}

FontStringEx::Item * FontStringEx::Item::clone() const {
    return vnnew Item(*this);
}

const FontStringEx::Item * FontStringEx::at(size_t index) const {
    return (Item *)m_items[index];
}

const FontStringEx::Item * FontStringEx::operator [](size_t index) const {
    return (Item *)m_items[index];
}

void FontStringEx::update(const FontPtr &font) {
    m_width = 0;
    if (font.null()) {
        for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
            (*it)->glyph = 0;
        }
    } else {
        for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
            Item *item = (Item *)*it;
            item->glyph = font->build(item->code);
            item->offset = m_width;
            item->top = 0;
            m_width += (item->glyph ? item->glyph->size.x : 0);
        }
    }
}

void FontStringEx::update(const FontPtr &font, const vector2f &start, const aabox2f &rect, f32 line_space, vector2f *end) {
    m_width = 0;
    if (font.null()) {
        for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
            (*it)->glyph = 0;
        }
        if (end) {
            *end = start;
        }
    } else {
        vector2f pen = start;
        float height = font->height() + line_space;
        for (Items::iterator it = m_items.begin(); it != m_items.end(); ++it) {
            Item *item = (Item *)*it;
            if (*item->code.c_str() == '\n') {
                pen.x = rect.min_corner.x;
                pen.y += height;
                continue;
            }
            item->glyph = font->build(item->code);
            if (item->glyph) {
                m_width += item->glyph->size.x;
                f32 right = pen.x + item->glyph->size.x;
                if (right >= rect.max_corner.x) {
                    pen.x = rect.min_corner.x;
                    pen.y += height;
                }
                item->offset = pen.x;
                item->top = pen.y;
                pen.x += item->glyph->size.x;
            }
        }
        if (end) {
            *end = pen;
        }
    }
}

FontStringEx::Item * FontStringEx::_createItem(size_t index) {
    return vnnew Item();
}

_vn_end
