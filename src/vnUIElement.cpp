//
//  vnUIElement.cpp
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnUIElement.h"
#include "vnUILocator.h"
#include "vnUIRenderer.h"
#include "vnUIRenderEffect.h"
#include "vnLog.h"

_vn_begin

UIElement::UIElement()
: m_parent(0)
, m_enabled(true)
, m_visible(true)
, m_clipping(false)
, m_hookTouchEvent(false)
, m_tag(0)
, m_locator(0) {
	
}

UIElement::UIElement(const UIElement &other)
: m_parent(0)
, m_name(other.m_name)
, m_enabled(other.m_enabled)
, m_visible(other.m_visible)
, m_clipping(other.m_clipping)
, m_hookTouchEvent(other.m_hookTouchEvent)
, m_tag(other.m_tag)
, m_locator(0) {
	if (other.m_locator) {
		other.m_locator->clone(this);
	}
}

UIElement::~UIElement() {
	removeAllChildren();
	if (m_locator) {
		delete m_locator;
	}
}

UIElement * UIElement::clone() const {
	return vnnew UIElement(*this);
}

void UIElement::onTouchEnter(const UITouch *touch) {
	
}

void UIElement::onTouchLeave(const UITouch *touch) {
	
}

bool UIElement::onTouchBegan(UIElement *target, const UITouch *touch) {
    return false;
}

bool UIElement::onTouchMoved(UIElement *target, const UITouch *touch) {
    return false;
}

bool UIElement::onTouchEnded(UIElement *target, const UITouch *touch) {
    return false;
}

bool UIElement::onTouchCancelled(UIElement *target, const UITouch *touch) {
    return false;
}

void UIElement::onInsertText(const str32 &text) {
	
}

void UIElement::onDeleteBackward() {
	
}

void UIElement::onMarkText(const str32 &text) {
	
}

void UIElement::onUnmarkText() {
    
}

void UIElement::onFocusIn() {
	
}

void UIElement::onFocusOut() {
	
}

void UIElement::setLocation(const aabox2f &location) {
	if (m_locator) {
		m_locator->_set(location);
	}
	m_boundingBox = location;
	_onBoundingBoxUpdated();
}

void UIElement::update(f32 deltaTime) {
	if (m_locator) {
		m_locator->update(deltaTime);
	}
    if (!m_effect.null()) {
        if (m_effect->valid()) {
            m_effect->update(this, deltaTime);
        } else {
            m_effect.release();
        }
    }
	_onUpdate(deltaTime);
	for (Elements::iterator it = m_children.begin(); it != m_children.end(); ++it) {
		(*it)->update(deltaTime);
	}
}

void UIElement::render(UIRenderer *renderer) {
    if (!m_effect.null()) {
        m_effect->begin(renderer, this);
    }
	if (m_clipping) {
		renderer->pushClippingBox(m_boundingBox);
		_onRender(renderer);
		for (Elements::iterator it = m_children.begin(); it != m_children.end(); ++it) {
			UIElement *p = *it;
			if (p->m_visible) {
				p->render(renderer);
			}
		}
		renderer->popClippingBox();
	} else {
		_onRender(renderer);
		for (Elements::iterator it = m_children.begin(); it != m_children.end(); ++it) {
			UIElement *p = *it;
			if (p->m_visible) {
				p->render(renderer);
			}
		}
	}
    if (!m_effect.null()) {
        m_effect->end(renderer, this);
    }
}

void UIElement::bringToTop() {
	if (m_parent) {
		m_parent->bringChildToTop(this);
	}
}

u32 UIElement::addChild(UIElement *child, bool grab) {
	vnassert(child != 0);
	if (grab) {
		child->grab();
	}
	if (child->m_parent) {
		child->m_parent->removeChild(child);
	}
	child->m_parent = this;
	u32 idx = (u32)m_children.size();
	m_children.push_back(child);
	if (child->m_locator) {
		child->m_locator->markDirty();
	}
	return idx;
}

void UIElement::removeChild(UIElement *child) {
	for (Elements::iterator it = m_children.begin(); it != m_children.end(); ++it) {
		if (*it == child) {
			m_children.erase(it);
			child->m_parent = 0;
			if (child->m_locator) {
				child->m_locator->markDirty();
			}
			child->drop();
			break;
		}
	}
}

void UIElement::removeChildByIndex(u32 index) {
	if (index < m_children.size()) {
		UIElement *child = m_children[index];
		m_children.erase(m_children.begin() + index);
		child->m_parent = 0;
		if (child->m_locator) {
			child->m_locator->markDirty();
		}
		child->drop();
	}
}

void UIElement::removeAllChildren() {
	for (Elements::iterator it = m_children.begin(); it != m_children.end(); ++it) {
		UIElement *child = *it;
		child->m_parent = 0;
		if (child->m_locator) {
			child->m_locator->markDirty();
		}
		child->drop();
	}
	m_children.clear();
}

void UIElement::bringChildToTop(u32 index) {
	if (index >= m_children.size()) {
		return ;
	}
	UIElement *child = m_children[index];
	m_children.erase(m_children.begin() + index);
	m_children.push_back(child);
}

void UIElement::bringChildToTop(UIElement *child) {
	for (Elements::iterator it = m_children.begin(); it != m_children.end(); ++it) {
		if (*it == child) {
			m_children.erase(it);
			m_children.push_back(child);
			break;
		}
	}
}

u32 UIElement::getChildIndex(UIElement *child) {
	vnassert(child->m_parent == this);
	u32 index = 0;
	for (Elements::iterator it = m_children.begin(); it != m_children.end(); ++it, ++index) {
		if (*it == child) {
			break;
		}
	}
	return index;
}

UIElement * UIElement::getChildByIndex(u32 index) {
	vnassert(index < m_children.size());
	return m_children[index];
}

UIElement * UIElement::getChildByTag(u32 tag) {
	for (Elements::iterator it = m_children.begin(); it != m_children.end(); ++it) {
		UIElement *p = *it;
		if (p->m_tag == tag) {
			return p;
		}
	}
	return 0;
}

u32 UIElement::getNumChildren() const {
	return (u32)m_children.size();
}

bool UIElement::contains(const vector2f &point) const {
	return point >= m_boundingBox.min_corner && point < m_boundingBox.max_corner;
}

bool UIElement::sendTouchBegan(UIElement *target, const UITouch *touch) {
	if (m_parent) {
		if (m_parent->sendTouchBegan(target, touch)) {
			return true;
		}
	}
	if (target == this) {
		return onTouchBegan(target, touch);
	} else if (m_hookTouchEvent) {
		if (onTouchBegan(target, touch)) {
			return true;
		}
	}
	return false;
}

bool UIElement::sendTouchMoved(UIElement *target, const UITouch *touch) {
	if (m_parent) {
		if (m_parent->sendTouchMoved(target, touch)) {
			return true;
		}
	}
	if (target == this) {
		return onTouchMoved(target, touch);
	} else if (m_hookTouchEvent) {
		if (onTouchMoved(target, touch)) {
			return true;
		}
	}
	return false;
}

bool UIElement::sendTouchEnded(UIElement *target, const UITouch *touch) {
	if (m_parent) {
		if (m_parent->sendTouchEnded(target, touch)) {
			return true;
		}
	}
	if (target == this) {
		return onTouchEnded(target, touch);
	} else if (m_hookTouchEvent) {
		if (onTouchEnded(target, touch)) {
			return true;
		}
	}
	return false;
}

bool UIElement::sendTouchCancelled(UIElement *target, const UITouch *touch) {
	if (m_parent) {
		if (m_parent->sendTouchCancelled(target, touch)) {
			return true;
		}
	}
	if (target == this) {
		return onTouchCancelled(target, touch);
	} else if (m_hookTouchEvent) {
		if (onTouchCancelled(target, touch)) {
			return true;
		}
	}
	return false;
}

UIElement * UIElement::pick(const vector2f &point) {
	if (m_clipping && (point < m_boundingBox.min_corner || point >= m_boundingBox.max_corner)) {
		return 0;
	}
	for (Elements::reverse_iterator it = m_children.rbegin(); it != m_children.rend(); ++it) {
		UIElement *p = *it;
		if (!p->m_visible || !p->m_enabled) {
			continue;
		}
		UIElement *ret = p->pick(point);
		if (ret) {
			return ret;
		}
	}
	if (contains(point)) {
		return this;
	}
	return 0;
}

KeyboardType UIElement::keyboardType() const {
	return kKT_None;
}

//void UIElement::load(XmlElement *xml, UIFactory::ElementMap &namedElements) {
//	setVisible(xml->queryAttributeBoolean("visible", true));
//	setEnabled(xml->queryAttributeBoolean("enabled", m_enabled));
//	setClipping(xml->queryAttributeBoolean("clipping", m_clipping));
//	setHookTouchEvent(xml->queryAttributeBoolean("hookTouchEvent", m_hookTouchEvent));
//	m_tag = xml->queryAttributeInt("tag", m_tag);
//}

void UIElement::init(const TreeDataObject *object) {
    vnassert(object);
    
    const TreeDataString *tdstr = object->queryAttributeString("id");
    if (tdstr) {
        m_name = tdstr->value();
    }
    
    setVisible(object->queryAttributeBoolean("visible", m_visible));
    setEnabled(object->queryAttributeBoolean("enabled", m_enabled));
    setClipping(object->queryAttributeBoolean("clipping", m_clipping));
    setHookTouchEvent(object->queryAttributeBoolean("hook_touch", m_hookTouchEvent));
    m_tag = object->queryAttributeInteger("tag", m_tag);
    
    const TreeDataObject *objLocator = object->queryAttributeObject("locator");
    if (objLocator) {
        if (UIFactory::instance().createLocator(objLocator->name(), this)) {
            m_locator->init(objLocator);
        } else {
            VN_LOG_ERROR("UIElement: unknown locator \"" << objLocator->name() << "\"");
        }
    }
}

void UIElement::buildElementMap(UIFactory::ElementMap &namedElements) {
    if (!m_name.empty()) {
        namedElements[m_name] = this;
    }
    for (Elements::iterator it = m_children.begin(); it != m_children.end(); ++it) {
        (*it)->buildElementMap(namedElements);
    }
}

UIElement * UIElement::parent() const {
	return m_parent;
}

const aabox2f & UIElement::boundingBox() const {
	return m_boundingBox;
}

UILocator * UIElement::locator() const {
	return m_locator;
}

void UIElement::setVisible(bool visible) {
	m_visible = visible;
}

bool UIElement::isVisible() const {
	return m_visible;
}

void UIElement::setEnabled(bool enabled) {
	m_enabled = enabled;
}

bool UIElement::isEnabled() const {
	return m_enabled;
}

bool UIElement::isClipping() const {
	return m_clipping;
}

void UIElement::setClipping(bool clipping) {
	m_clipping = clipping;
}

bool UIElement::isHookTouchEvent() const {
	return m_hookTouchEvent;
}

void UIElement::setHookTouchEvent(bool hook) {
	m_hookTouchEvent = hook;
}

void UIElement::setTag(u32 tag) {
	m_tag = tag;
}

u32 UIElement::getTag() const {
	return m_tag;
}

void UIElement::setRenderEffect(UIRenderEffect *effect, bool grab) {
    if (effect && grab) {
        effect->grab();
    }
    m_effect = effect;
}

UIRenderEffect * UIElement::getRenderEffect() const {
    return m_effect.ptr();
}

void UIElement::_onBoundingBoxUpdated() {
	for (Elements::iterator it = m_children.begin(); it != m_children.end(); ++it) {
		UIElement *p = *it;
		if (p->m_locator) {
			p->m_locator->markDirty();
		}
	}
}

void UIElement::_onUpdate(f32 deltaTime) {
	
}

void UIElement::_onRender(UIRenderer *renderer) {
	
}

bool UIElement::_bindAction(const c8 *name, RefCounted *func_impl) {
    VN_LOG_ERROR("UIElement: unknown action \"" << name << "\"");
    return false;
}

_vn_end