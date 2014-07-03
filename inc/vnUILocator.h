//
//  vnUILocator.h
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnUILocator_h
#define vn2d_vnUILocator_h

#include "vnUIFactory.h"
#include "vnAABox2.h"

_vn_begin

class UIElement;

class _vn_2d_api UILocator : public AllocatedObject {
public:
	// ctor & dtor
	UILocator(UIElement *owner);
    UILocator(const UILocator &other, UIElement *owner);
	virtual UILocator * clone(UIElement *owner) const = 0;
	virtual ~UILocator();
	
	// methods
	void markDirty();
	void cleanDirty();
	bool isDirty() const;
	void update(f32 deltaTime);
	
    virtual void init(const TreeDataObject *object);
    
	virtual void _set(const aabox2f &location) = 0;
	
protected:
	virtual void _onUpdate(f32 deltaTime);
	virtual void _updateBoundingBox() = 0;
	void _setOwnerBoundingBox(const aabox2f &boundingBox);
	UIElement *m_owner;
	bool m_dirty;
};

_vn_end

#endif
