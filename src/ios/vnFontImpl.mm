//
//  vnFontImpl.cpp
//  vn2d
//
//  Created by Wayne on 14-1-10.
//  Copyright (c) 2014年 viichi.com. All rights reserved.
//

#include "vnFontImpl.h"
#include "vnUnicode.h"

_vn_begin

FontImpl::FontImpl()
: m_font(0)
, m_height(0)
, m_scale(1)
, m_heightInPixel(0) {
    
}

FontImpl::~FontImpl() {
    
}

f32 FontImpl::height() const {
    return m_height;
}

bool FontImpl::_create(f32 size, f32 scale) {
    m_scale = scale;
	m_font = [UIFont boldSystemFontOfSize:(size * scale)];
    //m_font = [UIFont fontWithName:@"HiraKakuProN-W6" size:size * scale];
    if (!m_font) {
        return false;
    }
    m_heightInPixel = (u32)ceilf(m_font.lineHeight);
    m_height = m_heightInPixel / m_scale;
    _initTextureAndCells(kTexSize, m_heightInPixel);
    for (GlyphList::iterator it = m_glyphList.begin(); it != m_glyphList.end(); ++it) {
        GlyphEx *glyph = *it;
        glyph->size.y = m_height;
        glyph->bounds.min_corner = vector2f::kZero;
    }
    return true;
}

void FontImpl::_build(const str32 &code, vn::CellBasedFont::GlyphEx *glyph) {
    str8 utf8;
    Unicode::convert(code.c_str(), code.length(), utf8);
    NSString *str = [NSString stringWithUTF8String:utf8.c_str()];
    NSDictionary *attrib = [NSDictionary dictionaryWithObjectsAndKeys:
                            m_font, NSFontAttributeName,
                            [UIColor whiteColor], NSForegroundColorAttributeName,
                            nil];
    CGSize size = [str sizeWithAttributes:attrib];
    vector2i sizei(ceilf(size.width), floor(size.height));
    if (sizei.x > m_heightInPixel) {
        sizei.x = m_heightInPixel;
    }
    if (sizei.y > m_heightInPixel) {
        sizei.y = m_heightInPixel;
    }
    u32 numPixels = (u32)sizei.area();
	color_t *pixels = vnmalloc(color_t, numPixels);
	for (u32 i = 0; i < numPixels; ++i) {
		pixels[i] = 0x00FFFFFF;
	}
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	CGContextRef context = CGBitmapContextCreate(pixels, sizei.x, sizei.y, 8, sizei.x * sizeof(color_t), colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
	CGColorSpaceRelease(colorSpace);
	if (!context) {
		vnfree(pixels);
		return ;
	}
	//CGContextSetRGBFillColor(context, 1, 1, 1, 1);
	CGContextTranslateCTM(context, 0.0f, size.height);
	CGContextScaleCTM(context, 1.0f, -1.0f);
	UIGraphicsPushContext(context);
	CGPoint pt = {};
	[str drawAtPoint:pt withAttributes:attrib];
	UIGraphicsPopContext();
	CGContextRelease(context);
    
    m_texture->updatePixels(glyph->pos, sizei, pixels);
	vnfree(pixels);
    
    glyph->size.x = glyph->bounds.max_corner.x = (f32)sizei.x / m_scale;
    glyph->bounds.max_corner.y = (f32)sizei.y / m_scale;
    glyph->texcoords[2] = vector2f(glyph->pos + sizei) / kTexSize;
    glyph->texcoords[1].x = glyph->texcoords[2].x;
    glyph->texcoords[3].y = glyph->texcoords[2].y;
}

_vn_end