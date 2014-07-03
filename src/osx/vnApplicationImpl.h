//
//  vnApplicationImpl.h
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __vn2d__vnApplicationImpl__
#define __vn2d__vnApplicationImpl__

#include "vnApplication.h"
#include "vnAllocatedObject.h"

_vn_begin

class ApplicationImpl : public AllocatedObject {
public:
    ApplicationImpl();
    ~ApplicationImpl();
    
    int main(Application::Delegate *delegate);
    
    Application::Type type();
    f32 scale();
    const vector2i & viewSize();
    
    Device * createDevice(const vector2f &size);
    
protected:
    
    Application::Type m_type;
    f32 m_scale;
    vector2i m_viewSize;
    
};

_vn_end

#endif /* defined(__vn2d__vnApplicationImpl__) */
