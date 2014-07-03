//
//  vnUILocatorBox.cpp
//  vngfx
//
//  Created by Wayne on 13-12-25.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUILocatorBox.h"
#include "vnUIRoot.h"
#include "vnUIElement.h"

_vn_begin

UILocatorBox::UILocatorBox(UIElement *owner)
: UILocator(owner) {
	
}

UILocatorBox::UILocatorBox(const UILocatorBox &other, UIElement *owner)
: UILocator(other, owner)
, m_box(other.m_box) {
	
}

UILocatorBox * UILocatorBox::clone(UIElement *owner) const {
	return vnnew UILocatorBox(*this);
}


void UILocatorBox::init(const TreeDataObject *object) {
	m_box.min_corner.x = object->queryAttributeFloat("left");
	m_box.min_corner.y = object->queryAttributeFloat("top");
	m_box.max_corner.x = object->queryAttributeFloat("right");
	m_box.max_corner.y = object->queryAttributeFloat("bottom");
	
	m_dirty = true;
}

void UILocatorBox::_set(const aabox2f &location) {
	m_box = location;
	m_dirty = false;
}

void UILocatorBox::setBox(const aabox2f &box) {
	m_box = box;
	m_dirty = true;
}

const aabox2f & UILocatorBox::getBox() const {
	return m_box;
}

void UILocatorBox::_updateBoundingBox() {
	_setOwnerBoundingBox(m_box);
}

//////////////////////////////////////////////////////////

UILocatorBoxEx::UILocatorBoxEx(UIElement *owner)
: UILocatorBox(owner)
, m_unitLeft(kValue)
, m_unitTop(kValue)
, m_unitRight(kValue)
, m_unitBottom(kValue)
, m_alignmentLeft(kLittle)
, m_alignmentTop(kLittle)
, m_alignmentRight(kLittle)
, m_alignmentBottom(kLittle) {
	
}

UILocatorBoxEx::UILocatorBoxEx(const UILocatorBoxEx &other, UIElement *owner)
: UILocatorBox(other, owner)
, m_unitLeft(other.m_unitLeft)
, m_unitTop(other.m_unitTop)
, m_unitRight(other.m_unitRight)
, m_unitBottom(other.m_unitBottom)
, m_alignmentLeft(other.m_alignmentLeft)
, m_alignmentTop(other.m_alignmentTop)
, m_alignmentRight(other.m_alignmentRight)
, m_alignmentBottom(other.m_alignmentBottom) {
	
}

UILocatorBoxEx * UILocatorBoxEx::clone(UIElement *owner) const {
	return vnnew UILocatorBoxEx(*this, owner);
}


void UILocatorBoxEx::init(const TreeDataObject *object) {
	UILocatorBox::init(object);
	m_unitLeft = (Unit)object->queryAttributeInteger("left_unit");
	m_unitTop = (Unit)object->queryAttributeInteger("top_unit");
	m_unitRight = (Unit)object->queryAttributeInteger("right_unit");
	m_unitBottom = (Unit)object->queryAttributeInteger("bottom_unit");
	
	m_alignmentLeft = (Alignment)object->queryAttributeInteger("left_align");
	m_alignmentTop = (Alignment)object->queryAttributeInteger("top_align");
	m_alignmentRight = (Alignment)object->queryAttributeInteger("right_align");
	m_alignmentBottom = (Alignment)object->queryAttributeInteger("bottom_align");
}

void UILocatorBoxEx::_set(const aabox2f &location) {
	m_dirty = false;
	UIElement *parent = m_owner->parent();
    aabox2f parentRect;
    if (parent) {
        parentRect = parent->boundingBox();
    } else {
        parentRect = UIRoot::instance().getViewBox();
    }
	m_box.min_corner.x = _convertFrom(location.min_corner.x, m_alignmentLeft, m_unitLeft, parentRect.min_corner.x, parentRect.max_corner.x);
	m_box.min_corner.y = _convertFrom(location.min_corner.y, m_alignmentTop, m_unitTop, parentRect.min_corner.y, parentRect.max_corner.y);
	m_box.max_corner.x = _convertFrom(location.max_corner.x, m_alignmentRight, m_unitRight, parentRect.min_corner.x, parentRect.max_corner.x);
	m_box.max_corner.y = _convertFrom(location.max_corner.y, m_alignmentBottom, m_unitBottom, parentRect.min_corner.y, parentRect.max_corner.y);
	
}


void UILocatorBoxEx::_updateBoundingBox() {
	UIElement *parent = m_owner->parent();
    aabox2f parentRect;
    if (parent) {
        parentRect = parent->boundingBox();
    } else {
        parentRect = UIRoot::instance().getViewBox();
    }
	aabox2f box;
	box.min_corner.x = _convertTo(m_box.min_corner.x, m_alignmentLeft, m_unitLeft, parentRect.min_corner.x, parentRect.max_corner.x);
	box.min_corner.y = _convertTo(m_box.min_corner.y, m_alignmentTop, m_unitTop, parentRect.min_corner.y, parentRect.max_corner.y);
	box.max_corner.x = _convertTo(m_box.max_corner.x, m_alignmentRight, m_unitRight, parentRect.min_corner.x, parentRect.max_corner.x);
	box.max_corner.y = _convertTo(m_box.max_corner.y, m_alignmentBottom, m_unitBottom, parentRect.min_corner.y, parentRect.max_corner.y);
	_setOwnerBoundingBox(box);
}

f32 UILocatorBoxEx::_convertFrom(f32 x, Alignment align, Unit unit, f32 little, f32 big) {
	f32 ret;
	switch (align) {
		case kLittle:
			ret = x - little;
			break;
			
		case kBig:
			ret = big - x;
			break;
			
		case kHalf:
			ret = x - (little + big) * 0.5f;
			break;
	}
	if (unit == kPercent) {
		f32 len = big - little;
		if (len) {
			ret /= len;
		} else {
			ret = 1;
		}
	}
	return ret;
}

f32 UILocatorBoxEx::_convertTo(f32 x, Alignment align, Unit unit, f32 little, f32 big) {
	f32 ret = x;
	if (unit == kPercent) {
		ret *= big - little;
	}
	switch (align) {
		case kLittle:
			ret += little;
			break;
			
		case kBig:
			ret = big - ret;
			break;
			
		case kHalf:
			ret += (little + big) * 0.5f;
			break;
	}
	return ret;
}
_vn_end