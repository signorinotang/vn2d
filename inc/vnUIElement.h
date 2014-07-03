//
//  vnUIElement.h
//  vn2d
//
//  Created by Wayne on 14-1-13.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnUIElement_h
#define vn2d_vnUIElement_h

#include "vnUIFactory.h"
#include "vnAABox2.h"
#include "vnFunction.h"
#include "vnUIRenderEffect.h"
#include "vnLog.h"

_vn_begin

class UIRenderer;

class _vn_2d_api UIElement : public RefCounted {
	VN_DECL_UI_ELEMENT()
	friend class UILocator;
public:
	// ctor & dtor
	UIElement();
	UIElement(const UIElement &other);
	virtual ~UIElement();
	virtual UIElement * clone() const;
    
	// events
	virtual bool onTouchBegan(UIElement *target, const UITouch *touch);
    virtual bool onTouchMoved(UIElement *target, const UITouch *touch);
    virtual bool onTouchEnded(UIElement *target, const UITouch *touch);
    virtual bool onTouchCancelled(UIElement *target, const UITouch *touch);
    
	virtual void onTouchEnter(const UITouch *touch);
	virtual void onTouchLeave(const UITouch *touch);
	
	virtual void onInsertText(const str32 &text);
	virtual void onDeleteBackward();
	virtual void onMarkText(const str32 &text);
    virtual void onUnmarkText();
    
	virtual void onFocusIn();
	virtual void onFocusOut();
	
	
	// methods
	void setLocation(const aabox2f &location);
	void update(f32 deltaTime);
	void render(UIRenderer *renderer);
	void bringToTop();
	u32 addChild(UIElement *child, bool grab = true);
	void removeChild(UIElement *child);
	void removeChildByIndex(u32 index);
    void removeAllChildren();
	void bringChildToTop(u32 index);
	void bringChildToTop(UIElement *child);
	u32 getChildIndex(UIElement *child);
	UIElement * getChildByIndex(u32 index);
	UIElement * getChildByTag(u32 tag);
	u32 getNumChildren() const;
	
    bool sendTouchBegan(UIElement *target, const UITouch *touch);
    bool sendTouchMoved(UIElement *target, const UITouch *touch);
    bool sendTouchEnded(UIElement *target, const UITouch *touch);
    bool sendTouchCancelled(UIElement *target, const UITouch *touch);
    
	virtual bool contains(const vector2f &point) const;
	virtual UIElement * pick(const vector2f &point);
	virtual KeyboardType keyboardType() const;
	
	
	virtual void init(const TreeDataObject *object);
    virtual void buildElementMap(UIFactory::ElementMap &namedElements);
	template <typename T>
    bool bindAction(const c8 *name, const function<T> &func) {
        if (!this) {
            return false;
        }
        return _bindAction(name, func.impl_ptr());
    }
    
	// properties
	
	UIElement * parent() const;
	const aabox2f & boundingBox() const;
	UILocator * locator() const;
	
	virtual void setVisible(bool visible);
	bool isVisible() const;
	
	virtual void setEnabled(bool enabled);
	bool isEnabled() const;
	
	bool isClipping() const;
	void setClipping(bool clipping);
	
    // hook children touch event.
	bool isHookTouchEvent() const;
	void setHookTouchEvent(bool hook);
	
	void setTag(u32 tag);
	u32 getTag() const;
	
    void setRenderEffect(UIRenderEffect *effect, bool grab = true);
    UIRenderEffect * getRenderEffect() const;
    
protected:
	virtual void _onBoundingBoxUpdated();
	virtual void _onUpdate(f32 deltaTime);
	virtual void _onRender(UIRenderer *renderer);
	virtual bool _bindAction(const c8 *name, RefCounted *func_impl);
    
    str8 m_name;
	bool m_visible;
	bool m_enabled;
	bool m_clipping;
	bool m_hookTouchEvent; // hook children touch event.
	u32 m_tag;
	
	
	aabox2f m_boundingBox;
	UIElement *m_parent;
	UILocator *m_locator;
	RefCountedPtr<UIRenderEffect> m_effect;
    
	typedef std::vector<UIElement *> Elements;
	Elements m_children;
};

_vn_end

#define VN_UI_ELEMENT_BIND_ACTION(cls, name, dest, src_name, src_ptr) \
if (!strcmp(name, src_name)) {\
dest.set(src_ptr);\
if (src_ptr && dest.null()) {\
VN_LOG_ERROR(#cls << ": bind action \"" << name << "\" failed.");\
return false;\
}\
return true;\
}

#endif
