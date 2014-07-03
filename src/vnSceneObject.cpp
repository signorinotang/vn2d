//
//  vnSceneObject.cpp
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnSceneObject.h"

_vn_begin

SceneObject::SceneObject()
: m_sceneNode(0) {
    
}

void SceneObject::update() {
    
}

SceneNode * SceneObject::sceneNode() const {
    return m_sceneNode;
}

void SceneObject::_onTransformUpdated() {
    
}



_vn_end
