//
//  vnSceneNode.cpp
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnSceneNode.h"
#include "vnSceneObject.h"
#include "vnSceneNodeContainer.h"
#include "vnSceneNodeAnimator.h"

_vn_begin

SceneNode::SceneNode()
: m_parent(0)
, m_orientation(1, 0)
, m_rotation(0)
, m_scale(1, 1)
, m_visible(true)
, m_dirty(0)
, m_localTransformM(matrix2::c_identity)
, m_worldTransformM(matrix2::c_identity)
, m_inheritTransformM(true)
, m_inheritTransformT(true)
, m_children(vnnew SceneNodeContainerDefault())
, m_containerData(0) {
    
}

SceneNode::SceneNode(SceneNodeContainer *container)
: m_parent(0)
, m_orientation(1, 0)
, m_rotation(0)
, m_scale(1, 1)
, m_visible(true)
, m_dirty(0)
, m_localTransformM(matrix2::c_identity)
, m_worldTransformM(matrix2::c_identity)
, m_inheritTransformM(true)
, m_inheritTransformT(true)
, m_children(container)
, m_containerData(0) {
    if (!m_children) {
        m_children = vnnew SceneNodeContainerDefault();
    }
}

SceneNode::~SceneNode() {
//    for (Animators::iterator it = m_animators.begin(); it != m_animators.end(); ++it) {
//        it->first->onRemoved(this);
//    }
    clearObjects();
    struct Visitor : public SceneNodeContainer::Visitor {
        virtual bool visit(SceneNode *node) {
            node->m_parent = 0;
            return true;
        }
    } visitor;
    m_children->accept(&visitor);
    delete m_children;
}

void SceneNode::update(f32 deltaTime) {
//    for (Animators::iterator it = m_animators.begin(); it != m_animators.end(); ) {
//        if (!it->second || !it->first->onUpdate(this, deltaTime)) {
//            it->first->onRemoved(this);
//            it = m_animators.erase(it);
//        } else {
//            ++it;
//        }
//    }
    _onUpdate(deltaTime);
    cleanDirty();
	struct Visitor : public SceneNodeContainer::Visitor {
        f32 deltaTime;
        virtual bool visit(SceneNode *node) {
            node->update(deltaTime);
            return true;
        }
        Visitor(f32 dt) : deltaTime(dt) {}
    } visitor(deltaTime);
    m_children->accept(&visitor);
}

void SceneNode::setPosition(const vector2f &pos) {
	m_position = pos;
	m_dirty |= kDirty_Local;
    if (m_parent) {
        m_parent->m_children->notifyPositionUpdated(this);
    }
}

const vector2f & SceneNode::getPosition() const {
	return m_position;
}

void SceneNode::setRotation(real radians) {
	m_rotation = radians;
	math::sincos(radians, m_orientation.y, m_orientation.x);
	m_dirty |= kDirty_Local;
}

void SceneNode::setRotation(const vector2f &_oritentation) {
    m_orientation = _oritentation;
    m_rotation = math::atan2(_oritentation.y, _oritentation.x);
    m_dirty |= kDirty_Local;
}

real SceneNode::getRotation() const {
	return m_rotation;
}

void SceneNode::setScale(const vector2f &scale) {
	m_scale = scale;
	m_dirty |= kDirty_Local;
}

const vector2f & SceneNode::getScale() const {
	return m_scale;
}

void SceneNode::setVisible(bool visible) {
	m_visible = visible;
}

bool SceneNode::isVisible() const {
	return m_visible;
}

void SceneNode::setLocalBoundingBox(const aabox2f &rect) {
	m_localBoundingBox = rect;
	m_dirty |= kDirty_BoundingBox;
}

const aabox2f & SceneNode::getLocalBoundingBox() const {
	return m_localBoundingBox;
}

void SceneNode::setInheritTransformM(bool inherit) {
	m_inheritTransformM = inherit;
	m_dirty |= kDirty_World;
}

bool SceneNode::isInheritTransformM() const {
	return m_inheritTransformM;
}

void SceneNode::setInheritTransformT(bool inherit) {
	m_inheritTransformT = inherit;
	m_dirty |= kDirty_World;
}

bool SceneNode::isInheritTransformT() const {
	return m_inheritTransformT;
}

void SceneNode::addObject(SceneObject *object, bool grab) {
    vnassert(object);
    if (grab) {
        object->grab();
    }
	if (object->m_sceneNode) {
		object->m_sceneNode->removeObject(object);
	}
	object->m_sceneNode = this;
    m_objects.push_back(object);
	object->_onTransformUpdated();
}

bool SceneNode::removeObject(SceneObject *object) {
    for (Objects::iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        if (*it == object) {
            m_objects.erase(it);
			object->m_sceneNode = 0;
			object->_onTransformUpdated();
			object->drop();
            return true;
        }
    }
    return false;
}

void SceneNode::clearObjects() {
    for (Objects::iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        (*it)->drop();
    }
    m_objects.clear();
}

SceneNode * SceneNode::parent() const {
	return m_parent;
}

void SceneNode::addChild(SceneNode *child, bool grab) {
	vnassert(child);
    child->grab();
    if (child->m_parent) {
        child->m_parent->removeChild(child);
    }
    m_children->add(child, grab);
    child->drop();
	child->m_parent = this;
	child->m_dirty |= kDirty_World;
}

void SceneNode::removeChild(SceneNode *child) {
	vnassert(child && child->m_parent == this);
    child->m_parent = 0;
	m_children->remove(child);
}

void SceneNode::clearChildren() {
    struct Visitor : public SceneNodeContainer::Visitor {
        virtual bool visit(SceneNode *node) {
            node->m_parent = 0;
            return true;
        }
    } visitor;
    m_children->accept(&visitor);
	m_children->clear();
}

const vector2f & SceneNode::oritentation() const {
	return m_orientation;
}

const matrix2 & SceneNode::localTransformM() const {
	return m_localTransformM;
}

const matrix2 & SceneNode::worldTransformM() const {
    return m_worldTransformM;
}

const vector2f & SceneNode::worldTransformT() const {
	return m_worldTransformT;
}

const aabox2f & SceneNode::worldBoundingBox() const {
	return m_worldBoundingBox;
}

vector2f SceneNode::convertToWorld(const vector2f &point) {
	return point * m_worldTransformM + m_worldTransformT;
}

void SceneNode::cleanDirty() {
	if (!m_dirty) {
		return ;
	}
    if (m_dirty & kDirty_Local) {
        _updateLocalTransform();
    }
    if (m_dirty & kDirty_World) {
        _updateWorldTransform();
    }
    if (m_dirty & kDirty_BoundingBox) {
        _updateWorldBoundingBox();
    }
    m_dirty = 0;
    struct Visitor : public SceneNodeContainer::Visitor {
        virtual bool visit(SceneNode *node) {
            node->m_dirty |= kDirty_World;
            return true;
        }
    } visitor;
    m_children->accept(&visitor);
}

void SceneNode::queryVisibleNodes(const aabox2f &view, std::vector<SceneNode *> &nodes) {
    if (!m_visible) {
        return ;
    }
	if (view.intersects(m_worldBoundingBox)) {
		nodes.push_back(this);
	}
	queryVisibleChildren(view, nodes);
}

void SceneNode::queryVisibleChildren(const aabox2f &view, std::vector<SceneNode *> &nodes) {
    m_children->queryVisible(view, nodes);
}

void SceneNode::updateObjects() {
	for (Objects::iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        (*it)->update();
    }
    
}

void SceneNode::renderObjects() {
	for (Objects::iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        (*it)->render();
    }
}

void SceneNode::setContainerData(void *data) {
    m_containerData = data;
}

void * SceneNode::getContainerData() const {
    return m_containerData;
}


//void SceneNode::addAnimator(SceneNodeAnimator *animator, bool grab) {
//    vnassert(animator);
//    if (grab) {
//        animator->grab();
//    }
//    m_animators.push_back(std::make_pair(animator, true));
//    animator->onAdded(this);
//}
//
//void SceneNode::removeAnimator(SceneNodeAnimator *animator) {
//    for (Animators::iterator it = m_animators.begin(); it != m_animators.end(); ++it) {
//        if (it->first.ptr() == animator) {
//            it->second = false;
//            break;
//        }
//    }
//}
//
//void SceneNode::clearAnimators() {
//    for (Animators::iterator it = m_animators.begin(); it != m_animators.end(); ++it) {
//        it->second = false;
//    }
//}

void SceneNode::_onUpdate(f32 deltaTime) {
    
}

void SceneNode::_updateLocalTransform() {
    m_localTransformM.m11 = m_orientation.x * m_scale.x;
	m_localTransformM.m12 = m_orientation.y * m_scale.x;
	m_localTransformM.m21 = -m_orientation.y * m_scale.y;
	m_localTransformM.m22 = m_orientation.x * m_scale.y;
    m_dirty |= kDirty_World;
}

void SceneNode::_updateWorldTransform() {
	if (m_parent) {
        m_worldTransformT = m_position;
		if (m_inheritTransformM) {
			m_worldTransformM.set_by_product(m_localTransformM, m_parent->m_worldTransformM);
			m_worldTransformT *= m_parent->m_worldTransformM;
		}
		else {
			m_worldTransformM = m_localTransformM;
		}
		if (m_inheritTransformT) {
			m_worldTransformT += m_parent->m_worldTransformT;
		}
	} else {
        m_worldTransformM = m_localTransformM;
        m_worldTransformT = m_position;
    }
    m_dirty |= kDirty_BoundingBox;
	for (Objects::iterator it = m_objects.begin(); it != m_objects.end(); it++) {
        (*it)->_onTransformUpdated();
    }
}

void SceneNode::_updateWorldBoundingBox() {
	m_worldBoundingBox.reset(convertToWorld(m_localBoundingBox.corner_0()));
	m_worldBoundingBox.add_point(convertToWorld(m_localBoundingBox.corner_1()));
	m_worldBoundingBox.add_point(convertToWorld(m_localBoundingBox.corner_2()));
	m_worldBoundingBox.add_point(convertToWorld(m_localBoundingBox.corner_3()));
}

_vn_end
