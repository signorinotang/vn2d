//
//  vnSceneNodeContainer.h
//  vn2d
//
//  Created by Wayne on 3/11/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnSceneNodeContainer_h
#define vn2d_vnSceneNodeContainer_h

#include "vnAllocatedObject.h"
#include "vnAABox2.h"

#include <vector>

_vn_begin

class SceneNode;

class _vn_2d_api SceneNodeContainer : public AllocatedObject {
public:
    class Visitor {
    public:
        virtual bool visit(SceneNode *) = 0;
    };
    
    virtual ~SceneNodeContainer();
    
    virtual void add(SceneNode *node, bool grab = true) = 0;
    virtual void remove(SceneNode *node) = 0;
    virtual void clear() = 0;
    virtual size_t size() const = 0;
    virtual void queryVisible(const aabox2f &view, std::vector<SceneNode *> &nodes) const = 0;
    virtual bool accept(Visitor *visitor) const = 0;
    
    virtual void notifyPositionUpdated(SceneNode *node) = 0;
};

class _vn_2d_api SceneNodeContainerDefault : public SceneNodeContainer {
public:
    virtual ~SceneNodeContainerDefault();
    virtual void add(SceneNode *node, bool grab);
    virtual void remove(SceneNode *node);
    virtual void clear();
    virtual size_t size() const;
    virtual void queryVisible(const aabox2f &view, std::vector<SceneNode *> &nodes) const;
    virtual bool accept(Visitor *visitor) const;
    
    virtual void notifyPositionUpdated(SceneNode *node);
    
protected:
    typedef std::vector<SceneNode *> Nodes;
    Nodes m_nodes;
};

_vn_end

#endif
