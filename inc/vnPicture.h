//
//  vnPicture.h
//  vngfx
//
//  Created by Wayne on 13-12-10.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnPicture_h
#define vngfx_vnPicture_h

#include "vnTexture.h"
#include "vnAABox2.h"

_vn_begin

struct _vn_gfx_api Picture : RefCounted {
    TexturePtr texture;
    vector2f texcoords[4];
	vector2f size;
    
    Picture & buildTexcoord(const aabox2f &rect);
    Picture & buildTexcoord(const vector2f &size, const aabox2f &rect);
    Picture & flip_horizontal();
    Picture & flip_vertical();
    Picture & rotate_90();
    Picture & rotate_180();
    Picture & rotate_270();
};

typedef RefCountedPtr<Picture> PicturePtr;

_vn_end

#endif
