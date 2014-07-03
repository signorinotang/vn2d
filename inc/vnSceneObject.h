//
//  vnSceneObject.h
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnSceneObject_h
#define vn2d_vnSceneObject_h

#include "vnRefCounted.h"

_vn_begin

class SceneNode;

class _vn_2d_api SceneObject : public RefCounted {
    friend class SceneNode;
public:
    SceneObject();
    virtual void update();
    virtual void render() = 0;
    
    SceneNode * sceneNode() const;
    
protected:
    virtual void _onTransformUpdated();
    SceneNode *m_sceneNode;
};

_vn_end

#endif
