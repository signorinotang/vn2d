//
//  vnCellBasedFont.h
//  vn2d
//
//  Created by Wayne on 14-1-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnFont_TTF_h
#define vn2d_vnFont_TTF_h

#include "vnFont.h"

#include <map>
#include <list>

_vn_begin

class _vn_2d_api CellBasedFont : public Font {
public:
    CellBasedFont();
    virtual ~CellBasedFont();
    
    virtual const Glyph * build(const str32 &code);
    virtual void discard();
    
protected:
    
    struct GlyphEx;
    typedef std::map<str32, GlyphEx *> GlyphMap;
	typedef std::list<GlyphEx *> GlyphList;
    
    struct GlyphEx : Glyph, AllocatedObject {
        vector2i pos; // in texture.
        u32 pass;
		GlyphList::iterator lstIt;
		GlyphMap::iterator mapIt;
    };
    
	u32 m_currentPass;
    TexturePtr m_texture;
    
	GlyphMap m_glyphMap;
	GlyphList m_glyphList;
    
    void _initTextureAndCells(u32 cell_size, u32 texture_size);
    virtual void _build(const str32 &code, GlyphEx *glyph) = 0;
};

_vn_end


#endif
