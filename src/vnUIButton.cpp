//
//  vnUIButton.cpp
//  vngfx
//
//  Created by Wayne on 13-12-19.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#include "vnUIButton.h"
#include "vnUIRoot.h"

_vn_begin

UIButton::UIButton()
: m_state(kNormal)
, m_pushed(false)
, m_pressed(false)
, m_touch(0) {
    
}

UIButton * UIButton::clone() const {
    return vnnew UIButton(*this);
}

UIButton::State UIButton::state() const {
    return m_state;
}

bool UIButton::isPushed() const {
    return m_pushed;
}

void UIButton::setPushed(bool pushed) {
    if (m_pushed != pushed) {
        m_pushed = pushed;
        _updateState();
    }
}

void UIButton::setEnabled(bool enabled) {
    if (m_enabled != enabled) {
        m_enabled = enabled;
        _updateState();
    }
}

void UIButton::init(const TreeDataObject *object) {
    UIElement::init(object);
    m_pushed = object->queryAttributeBoolean("pushed", m_pushed);
}

void UIButton::bindAction_Clicked(const function<void (vn::UIElement *)> &func) {
    m_fnClicked = func;
}

bool UIButton::onTouchBegan(UIElement *target, const UITouch *touch) {
    if (m_touch) {
        return false;
    }
    if (UIRoot::instance().captureTouch(this, touch->touchId)) {
        m_touch = touch;
        m_pressed = true;
        _updateState();
    }
    return true;
}

bool UIButton::onTouchMoved(UIElement *target, const UITouch *touch) {
    if (m_touch != touch) {
        return false;
    }
    return true;
}

bool UIButton::onTouchEnded(UIElement *target, const UITouch *touch) {
    if (m_touch != touch) {
        return false;
    }
    m_touch = 0;
    UIRoot::instance().releaseTouch(this, touch->touchId);
    if (m_pressed) {
        m_pressed = false;
        _updateState();
        onClicked();
    }
    return true;
}

bool UIButton::onTouchCancelled(UIElement *target, const UITouch *touch) {
    if (m_touch != touch) {
        return false;
    }
    m_touch = 0;
    m_pressed = false;
    return true;
}

void UIButton::onTouchEnter(const UITouch *touch) {
    if (m_touch == touch) {
        m_pressed = true;
        _updateState();
    }
}

void UIButton::onTouchLeave(const UITouch *touch) {
    if (m_touch == touch) {
        m_pressed = false;
        _updateState();
    }
}

void UIButton::onClicked() {
    if (!m_fnClicked.null()) {
        m_fnClicked(this);
    }
}

bool UIButton::_bindAction(const c8 *name, RefCounted *func_impl) {
    VN_UI_ELEMENT_BIND_ACTION(UIButton, "clicked", m_fnClicked, name, func_impl);
    return UIElement::_bindAction(name, func_impl);
}

void UIButton::_updateState() {
    if (!m_enabled) {
        m_state = kDisabled;
    } else if (m_pushed) {
        if (m_pressed) {
            m_state = kPushedPressed;
        } else {
            m_state = kPushed;
        }
    } else {
        if (m_pressed) {
            m_state = kNormalPressed;
        } else {
            m_state = kNormal;
        }
    }
}

_vn_end