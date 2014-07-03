//
//  vnSceneNodeAnimator.h
//  vn2d
//
//  Created by Wayne on 3/19/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnSceneNodeAnimator_h
#define vn2d_vnSceneNodeAnimator_h

#include "vnRefCounted.h"

_vn_begin

class SceneNode;

class _vn_2d_api SceneNodeAnimator : public RefCounted {
public:
    virtual void onAdded(SceneNode *node);
    virtual bool onUpdate(SceneNode *node, f32 deltaTime);
    virtual void onRemoved(SceneNode *node);
};

_vn_end

#endif
