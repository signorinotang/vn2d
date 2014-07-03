//
//  vnFontString.h
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnFontString_h
#define vn2d_vnFontString_h

#include "vnFont.h"
#include <vector>

_vn_begin

class _vn_2d_api FontString : public AllocatedObject {
public:
    struct _vn_2d_api Item : public AllocatedObject {
        Item();
        Item(const Item &other);
        virtual Item * clone() const;
        virtual ~Item();
        str32 code;
        const Font::Glyph *glyph;
        f32 offset;
    };
    
    FontString();
    FontString(const FontString &other);
    ~FontString();
    
    f32 width() const;
    bool empty() const;
    size_t size() const;
    const Item * at(size_t index) const;
    const Item * operator [](size_t index) const;
    
    const str32 & str() const;
    virtual void str(const str32 &_str);
    
    virtual void append(const str32 &_str);
    virtual void pop_back();
    virtual void clear();
    
    virtual void update(const FontPtr &font);
    
protected:
    str32 m_string;
    typedef std::vector<Item *> Items;
    Items m_items;
    f32 m_width;
    
    virtual Item * _createItem(size_t index);
    void _append(const c32 *sz);
};

class _vn_2d_api FontStringEx : public FontString {
public:
    struct _vn_2d_api Item : FontString::Item {
        Item();
        virtual Item * clone() const;
        f32 top;
    };
    const Item * at(size_t index) const;
    const Item * operator [](size_t index) const;
    
    virtual void update(const FontPtr &font);
    virtual void update(const FontPtr &font, const vector2f &start, const aabox2f &rect, f32 line_space = 0, vector2f *end = 0);
    
protected:
    virtual Item * _createItem(size_t index);
    
};

_vn_end

#endif
