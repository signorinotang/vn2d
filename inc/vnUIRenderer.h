//
//  vnUIRenderer.h
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnUIRenderer_h
#define vn2d_vnUIRenderer_h

#include "vnPicture.h"
#include "vnColor.h"
#include "vnMatrix3.h"

#include <stack>

_vn_begin

class _vn_2d_api UIRenderer : public AllocatedObject {
public:
    void pushClippingBox(const aabox2f &clippingBox);
	void popClippingBox();
    
    void pushTransform(const matrix3 &mat);
    void popTransform();
    
    void pushBlendColor(const color_t &color);
    void popBlendColor();
    
    void drawRect(const aabox2f &rect, const color_t &color);
    void drawRect(const aabox2f &rect, const color_t &color, const TexturePtr &texture, const vector2f *texcoords);
    void drawRect(const aabox2f &rect, const color_t &color, Picture *picture);
    
    void _begin();
    void _end();
private:
    typedef aabox2f rect_type;
    std::stack<aabox2f> m_clippingBoxes;
    std::stack<matrix3> m_transforms;
    std::stack<color_t> m_blendColors;
    
	bool _getClippingBox(const aabox2f *&clippingBox);
    color_t _blend(const color_t &color);
	void _draw(const rect_type &rect, const color_t &color);
	void _draw(const rect_type &rect, const color_t &color, const TexturePtr &texture, const vector2f *texcoords);
	static bool _hasIntersection(const aabox2f &a, const aabox2f &b);
	static bool _clipRect(const aabox2f &clippingBox, aabox2f &rect);
	static bool _clipRect(const aabox2f &clippingBox, aabox2f &rect, const vector2f src_texcoords[4], vector2f dest_texcoords[4]);
	
	static aabox2f _modify_rect(const aabox2f &rect, const vector2f &scaleInPixel);
};

_vn_end

#endif
