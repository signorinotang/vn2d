//
//  vnCellBasedFont.cpp
//  vn2d
//
//  Created by Wayne on 14-1-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnCellBasedFont.h"
#include "vnDevice.h"

_vn_begin

CellBasedFont::CellBasedFont()
: m_currentPass(1) {
    
}

CellBasedFont::~CellBasedFont() {
    for (GlyphList::iterator it = m_glyphList.begin(); it != m_glyphList.end(); ++it) {
		delete *it;
	}
}

const Font::Glyph * CellBasedFont::build(const str32 &code) {
    GlyphMap::iterator it = m_glyphMap.insert(std::make_pair(code, (GlyphEx *)0)).first;
	if (it->second) {
		it->second->pass = m_currentPass;
	} else {
        GlyphEx *glyph = m_glyphList.back();
        if (glyph->pass == m_currentPass) {
            return 0;
        }
        if (glyph->mapIt != m_glyphMap.end()) {
            m_glyphMap.erase(glyph->mapIt);
        }
        glyph->pass = m_currentPass;
        m_glyphList.erase(glyph->lstIt);
        m_glyphList.push_front( glyph );
        glyph->lstIt = m_glyphList.begin();
        glyph->mapIt = it;
        it->second = glyph;
        _build(code, glyph);
    }
    return it->second;
}

void CellBasedFont::discard() {
    ++m_currentPass;
}

void CellBasedFont::_initTextureAndCells(u32 texture_size, u32 cell_size) {
    m_texture = Device::instance().createTexture(vector2i(texture_size, texture_size));
    s32 cnt = texture_size / cell_size;
    for (s32 y = 0; y < cnt; ++y) {
        for (s32 x = 0; x < cnt; ++x) {
            GlyphEx *p = vnnew GlyphEx();
            p->valid = false;
            p->texture = m_texture;
            p->pos.set(x * cell_size, y * cell_size);
            p->texcoords[0] = vector2f(p->pos) / (real)texture_size;
			p->texcoords[1].y = p->texcoords[0].y;
			p->texcoords[3].x = p->texcoords[0].x;
            p->pass = 0;
            m_glyphList.push_front(p);
            p->lstIt = m_glyphList.begin();
            p->mapIt = m_glyphMap.end();
        }
    }
}


_vn_end
