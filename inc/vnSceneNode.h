//
//  vnSceneNode.h
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnSceneNode_h
#define vn2d_vnSceneNode_h

#include "vnRefCounted.h"

#include "vnMatrix2.h"
#include "vnAABox2.h"

#include <vector>
#include <list>

_vn_begin

class SceneObject;
class SceneNodeContainer;
class SceneNodeAnimator;

class _vn_2d_api SceneNode : public RefCounted {
public:
    SceneNode();
    SceneNode(SceneNodeContainer *container);
    virtual ~SceneNode();
    
    void update(f32 deltaTime);
    
	void setPosition(const vector2f &pos);
	const vector2f & getPosition() const;
	
	void setRotation(real radians);
    void setRotation(const vector2f &_oritentation);
	real getRotation() const;
	
	void setScale(const vector2f &scale);
	const vector2f & getScale() const;
	
	void setVisible(bool visible);
	bool isVisible() const;
	
	void setLocalBoundingBox(const aabox2f &rect);
	const aabox2f & getLocalBoundingBox() const;
	
	void setInheritTransformM(bool inherit);
	bool isInheritTransformM() const;
	
	void setInheritTransformT(bool inherit);
	bool isInheritTransformT() const;
	
	const vector2f & oritentation() const;
	const matrix2 & localTransformM() const;
    const matrix2 & worldTransformM() const;
	const vector2f & worldTransformT() const;
	const aabox2f & worldBoundingBox() const;
	
	void addObject(SceneObject *object, bool grab = true);
	bool removeObject(SceneObject *object);
	void clearObjects();
    
	SceneNode * parent() const;
	void addChild(SceneNode *child, bool grab = true);
	void removeChild(SceneNode *child);
	void clearChildren();
    
	vector2f convertToWorld(const vector2f &point);
    
	void cleanDirty();
	
	void updateObjects();
	void renderObjects();
	
	void queryVisibleNodes(const aabox2f &view, std::vector<SceneNode *> &nodes);
	void queryVisibleChildren(const aabox2f &view, std::vector<SceneNode *> &nodes);
    
    void setContainerData(void *data);
    void * getContainerData() const;
    
//    void addAnimator(SceneNodeAnimator *animator, bool grab = true);
//    void removeAnimator(SceneNodeAnimator *animator);
//    void clearAnimators();
    
protected:
    virtual void _onUpdate(f32 deltaTime);
    virtual void _updateLocalTransform();
    virtual void _updateWorldTransform();
	virtual void _updateWorldBoundingBox();
    
	SceneNode *m_parent;
	
	vector2f m_position;
	real m_rotation;
	vector2f m_orientation;
	vector2f m_scale;
	bool m_visible;
	
	matrix2 m_localTransformM;
    
    matrix2 m_worldTransformM;
	vector2f m_worldTransformT;
    
	bool m_inheritTransformM;
	bool m_inheritTransformT;
	
	SceneNodeContainer *m_children;
	
    typedef std::vector<SceneObject *> Objects;
    Objects m_objects;
	
	aabox2f m_localBoundingBox;
	aabox2f m_worldBoundingBox;
    
    enum {
        kDirty_Local = 1,
        kDirty_World = 2,
        kDirty_BoundingBox = 4
    };
    
	u32 m_dirty;
	
    void *m_containerData;
    
//    typedef std::list< std::pair<RefCountedPtr<SceneNodeAnimator>, bool> > Animators;
//    Animators m_animators;
};

typedef RefCountedPtr<SceneNode> SceneNodePtr;

_vn_end

#endif
