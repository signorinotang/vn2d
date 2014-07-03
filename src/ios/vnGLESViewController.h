//
//  vnGLESViewController.h
//  vn2d
//
//  Created by Wayne on 14-1-10.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef __vn2d__vnGLESViewController__
#define __vn2d__vnGLESViewController__

#import <GLKit/GLKit.h>
#include "vn2dDef.h"

_vn_begin
class ApplicationImpl;
_vn_end

@interface GLESViewController : GLKViewController<GLKViewControllerDelegate> {
    _vn_ns::ApplicationImpl *m_impl;
}

- (id)initWithAppImpl:(_vn_ns::ApplicationImpl *)impl;

@end

#endif /* defined(__vn2d__vnGLESViewController__) */
