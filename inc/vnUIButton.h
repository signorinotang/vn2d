//
//  vnUIButton.h
//  vngfx
//
//  Created by Wayne on 13-12-19.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vngfx_vnUIButton_h
#define vngfx_vnUIButton_h

#include "vnUIElement.h"

_vn_begin

class _vn_gfx_api UIButton : public UIElement {
    VN_DECL_UI_ELEMENT()
public:
    enum State {
        kNormal = 0,
        kDisabled,
        kPushed,
        kNormalPressed,
        kPushedPressed
    };
    
    // ctor & dtor
    UIButton();
    virtual UIButton * clone() const;
    
    // properties
    State state() const;
    bool isPushed() const;
    void setPushed(bool pushed);
    
    virtual void setEnabled(bool enabled);
    
    // methods
    virtual void init(const TreeDataObject *object);

    // named "clicked"
    void bindAction_Clicked(const function<void(UIElement *sender)> &func);
    
    // events
	virtual bool onTouchBegan(UIElement *target, const UITouch *touch);
    virtual bool onTouchMoved(UIElement *target, const UITouch *touch);
    virtual bool onTouchEnded(UIElement *target, const UITouch *touch);
    virtual bool onTouchCancelled(UIElement *target, const UITouch *touch);
    virtual void onTouchEnter(const UITouch *touch);
	virtual void onTouchLeave(const UITouch *touch);
    
    virtual void onClicked();
    
protected:
    virtual bool _bindAction(const c8 *name, RefCounted *func_impl);
    void _updateState();
    
    State m_state;
    bool m_pushed;
    const UITouch *m_touch;
    bool m_pressed;
    function<void(UIElement *sender)> m_fnClicked;
};

_vn_end

#endif
