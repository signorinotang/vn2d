//
//  vnUILocatorRect.cpp
//  vngfx
//
//  Created by Wayne on 13-12-18.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUILocatorRect.h"
#include "vnUIElement.h"
#include "vnUIRoot.h"

_vn_begin

UILocatorRect::UILocatorRect(UIElement *owner)
: UILocator(owner) {
	
}

UILocatorRect::UILocatorRect(const UILocatorRect &other, UIElement *owner)
: UILocator(other, owner)
, m_position(other.m_position)
, m_size(other.m_size) {
	
}

UILocatorRect * UILocatorRect::clone(UIElement *owner) const {
	return vnnew UILocatorRect(*this, owner);
}

void UILocatorRect::setPosition(const vector2f &position) {
	m_position = position;
	m_dirty = true;
}

const vector2f & UILocatorRect::getPosition() const {
	return m_position;
}

void UILocatorRect::setSize(const vector2f &size) {
	m_size = size;
	m_dirty = true;
}

const vector2f & UILocatorRect::getSize() const {
	return m_size;
}

void UILocatorRect::init(const TreeDataObject *object) {
    m_dirty = true;
    m_position.x = object->queryAttributeFloat("x");
    m_position.y = object->queryAttributeFloat("y");
    m_size.x = object->queryAttributeFloat("width");
    m_size.y = object->queryAttributeFloat("height");
}

void UILocatorRect::_set(const aabox2f &location) {
	m_position = location.min_corner;
	m_size = location.size();
	UIElement *parent = m_owner->parent();
	if (parent) {
		m_position -= parent->boundingBox().min_corner;
	}
	m_dirty = false;
}

void UILocatorRect::_updateBoundingBox() {
	UIElement *parent = m_owner->parent();
	aabox2f box;
	box.min_corner = m_position;
	if (parent) {
		box.min_corner += parent->boundingBox().min_corner;
	}
	box.max_corner = box.min_corner + m_size;
	_setOwnerBoundingBox(box);
}

//////////////////////////////////////////////////


UILocatorRectEx::UILocatorRectEx(UIElement *owner)
: UILocatorRect(owner)
, m_alignX(kLittle)
, m_alignY(kLittle)
, m_unitX(kValue)
, m_unitY(kValue)
, m_unitWidth(kValue)
, m_unitHeight(kValue) {
    
}

UILocatorRectEx::UILocatorRectEx(const UILocatorRectEx &other, UIElement *owner)
: UILocatorRect(other, owner)
, m_alignX(other.m_alignX)
, m_alignY(other.m_alignY)
, m_unitX(other.m_unitX)
, m_unitY(other.m_unitY)
, m_unitWidth(other.m_unitWidth)
, m_unitHeight(other.m_unitHeight) {
    
}

UILocatorRectEx * UILocatorRectEx::clone(UIElement *owner) const {
    return vnnew UILocatorRectEx(*this, owner);
}

UILocatorRectEx::Alignment UILocatorRectEx::getPositionXAlignment() const {
    return m_alignX;
}

void UILocatorRectEx::setPositionXAlignment(Alignment xAlignment) {
    if (m_alignX != xAlignment) {
        m_alignX = xAlignment;
        m_dirty = true;
    }
}

UILocatorRectEx::Alignment UILocatorRectEx::getPositionYAlignment() const {
    return m_alignY;
}

void UILocatorRectEx::setPositionYAlignment(Alignment yAlignment) {
    if (m_alignY != yAlignment) {
        m_alignY = yAlignment;
        m_dirty = true;
    }
}

UILocatorRectEx::Unit UILocatorRectEx::getPositionXUnit() const {
    return m_unitX;
}

void UILocatorRectEx::setPositionXUnit(Unit xUnit) {
    if (m_unitX != xUnit) {
        m_unitX = xUnit;
        m_dirty = true;
    }
}

UILocatorRectEx::Unit UILocatorRectEx::getPositionYUnit() const {
    return m_unitY;
}

void UILocatorRectEx::setPositionYUnit(Unit yUnit) {
    if (m_unitY != yUnit) {
        m_unitY = yUnit;
        m_dirty = true;
    }
}

UILocatorRectEx::Unit UILocatorRectEx::getWidthUnit() const {
    return m_unitWidth;
}

void UILocatorRectEx::setWidthUnit(Unit widthUnit) {
    if (m_unitWidth != widthUnit) {
        m_unitWidth = widthUnit;
        m_dirty = true;
    }
}

UILocatorRectEx::Unit UILocatorRectEx::getHeightUnit() const {
    return m_unitHeight;
}

void UILocatorRectEx::setHeightUnit(Unit heightUnit) {
    if (m_unitHeight != heightUnit) {
        m_unitHeight = heightUnit;
        m_dirty = true;
    }
}

void UILocatorRectEx::init(const TreeDataObject *object) {
    UILocatorRect::init(object);
    m_alignX = (Alignment)object->queryAttributeInteger("x_align");
    m_alignY = (Alignment)object->queryAttributeInteger("y_align");
    m_unitX = (Unit)object->queryAttributeInteger("x_unit");
    m_unitY = (Unit)object->queryAttributeInteger("y_unit");
    m_unitWidth = (Unit)object->queryAttributeInteger("width_unit");
    m_unitHeight = (Unit)object->queryAttributeInteger("height_unit");
}

void UILocatorRectEx::_set(const aabox2f &location) {
    UIElement *parent = m_owner->parent();
    aabox2f parentRect;
    if (parent) {
        parentRect = parent->boundingBox();
    } else {
        parentRect = UIRoot::instance().getViewBox();
    }
    vector2f parentSize = parentRect.size();
    vector2f size = location.size();
    if (m_unitWidth == kValue) {
        m_size.x = size.x;
    } else {
        m_size.x = size.x / parentSize.x;
    }
    if (m_unitHeight == kValue) {
        m_size.y = size.y;
    } else {
        m_size.y = size.y / parentSize.y;
    }
    
    vector2f pos;
    switch (m_alignX) {
        case kBig:
            pos.x = parentRect.max_corner.x - location.max_corner.x;
            break;
        case kHalf:
            pos.x = (location.min_corner.x + location.max_corner.x - parentRect.min_corner.x - parentRect.max_corner.x) * 0.5f;
            break;
        default:
            pos.x = location.min_corner.x - parentRect.min_corner.x;
            break;
    }
    switch (m_alignY) {
        case kBig:
            pos.y = parentRect.max_corner.y - location.max_corner.y;
            break;
        case kHalf:
            pos.y = (location.min_corner.y + location.max_corner.y - parentRect.min_corner.y - parentRect.max_corner.y) * 0.5f;
            break;
        default:
            pos.y = location.min_corner.y - parentRect.min_corner.y;
            break;
    }
    if (m_unitX == kValue) {
        m_position.x = pos.x;
    } else {
        m_position.x = pos.x / parentSize.x;
    }
    if (m_unitY == kValue) {
        m_position.y = pos.y;
    } else {
        m_position.y = pos.y / parentSize.y;
    }
}

void UILocatorRectEx::_updateBoundingBox() {
    UIElement *parent = m_owner->parent();
    aabox2f parentRect;
    if (parent) {
        parentRect = parent->boundingBox();
    } else {
        parentRect = UIRoot::instance().getViewBox();
    }
    vector2f parentSize = parentRect.size();
    vector2f size;
    if (m_unitWidth == kValue) {
        size.x = m_size.x;
    } else {
        size.x = parentSize.x * m_size.x;
    }
    if (m_unitHeight == kValue) {
        size.y = m_size.y;
    } else {
        size.y = parentSize.y * m_size.y;
    }
    vector2f offset;
    if (m_unitX == kValue) {
        offset.x = m_position.x;
    } else {
        offset.x = parentSize.x * m_position.x;
    }
    if (m_unitY == kValue) {
        offset.y = m_position.y;
    } else {
        offset.y = parentSize.y * m_position.y;
    }
    aabox2f rect;
    switch (m_alignX) {
        case kBig:
            rect.max_corner.x = parentRect.max_corner.x - offset.x;
            rect.min_corner.x = rect.max_corner.x - size.x;
            break;
        case kHalf:
            rect.min_corner.x = parentRect.min_corner.x + offset.x + (parentSize.x - size.x) * 0.5f;
            rect.max_corner.x = rect.min_corner.x + size.x;
            break;
        default:
            rect.min_corner.x = parentRect.min_corner.x + offset.x;
            rect.max_corner.x = rect.min_corner.x + size.x;
            break;
    }
    switch (m_alignY) {
        case kBig:
            rect.max_corner.y = parentRect.max_corner.y - offset.y;
            rect.min_corner.y = rect.max_corner.y - size.y;
            break;
        case kHalf:
            rect.min_corner.y = parentRect.min_corner.y + offset.y + (parentSize.y - size.y) * 0.5f;
            rect.max_corner.y = rect.min_corner.y + size.y;
            break;
        default:
            rect.min_corner.y = parentRect.min_corner.y + offset.y;
            rect.max_corner.y = rect.min_corner.y + size.y;
            break;
    }
    _setOwnerBoundingBox(rect);
}

_vn_end
