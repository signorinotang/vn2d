//
//  vnUIRoot.cpp
//  vn2d
//
//  Created by Wayne on 14-1-14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnUIRoot.h"
#include "vnUIElement.h"
#include "vnUILocator.h"
#include "vnApplication.h"

_vn_begin

UIRoot::TouchData::TouchData()
: hover(0), capture(0) {
	
}

UIRoot::TouchData::~TouchData() {
	VN_SAFE_DROP(hover);
	VN_SAFE_DROP(capture);
}

UIRoot::UIRoot()
: m_root(0)
, m_focus(0) {
	
}

UIRoot::~UIRoot() {
	VN_SAFE_DROP(m_focus);
	VN_SAFE_DROP(m_root);
}

void UIRoot::setViewBox(const aabox2f &rect) {
    m_viewBox = rect;
    if (m_root && m_root->locator()) {
        m_root->locator()->markDirty();
    }
}

const aabox2f & UIRoot::getViewBox() const {
	return m_viewBox;
}

void UIRoot::update(f32 deltaTime) {
	_updateTouches();
    _updateInput();
	if (m_root) {
		m_root->update(deltaTime);
	}
}

void UIRoot::render() {
    m_renderer._begin();
	if (m_root) {
		m_root->render(&m_renderer);
	}
    m_renderer._end();
}

void UIRoot::setRootElement(UIElement *root, bool grab) {
	if (m_root) {
		reset();
		m_root->drop();
	}
	if((m_root = root) && grab) {
		root->grab();
        if (root->locator()) {
            root->locator()->markDirty();
        }
	}
}

bool UIRoot::dispatchTouchBegan(long touchId, const vector2f &position) {
    TouchData &td = _newTouch(touchId, position);
    
    for (TouchFilters::iterator it = m_touchFilters.begin(); it != m_touchFilters.end(); ) {
        TouchFilter *filter = *it;
        ++it;
        if (filter->filter_onTouchBegan(&td.touch)) {
            return true;
        }
    }
    
    if (m_root && m_root->isVisible() && m_root->isEnabled()) {
        td.hover = m_root->pick(position);
        if (td.hover) {
            td.hover->grab();
            td.hover->onTouchEnter(&td.touch);
            return td.hover->sendTouchBegan(td.hover, &td.touch);
        }
    }
    return false;
}
			
bool UIRoot::dispatchTouchMoved(long touchId, const vector2f &position) {
    TouchData *p = 0;
    if (_updateTouch(touchId, position, p)) {
        for (TouchFilters::iterator it = m_touchFilters.begin(); it != m_touchFilters.end(); ) {
            TouchFilter *filter = *it;
            ++it;
            if (filter->filter_onTouchMoved(&p->touch)) {
                return true;
            }
        }
        if (p->capture) {
            return p->capture->sendTouchMoved(p->capture, &p->touch);
        }
        if (p->hover) {
            return p->hover->sendTouchMoved(p->hover, &p->touch);
        }
    }
    return false;
}
			
bool UIRoot::dispatchTouchEnded(long touchId, const vector2f &position) {
    bool ret = false;
    TouchDataMap::iterator it = m_touches.find(touchId);
    if (it != m_touches.end()) {
        TouchData *p = &it->second;
        p->touch.position = position;
        
        for (TouchFilters::iterator fit = m_touchFilters.begin(); fit != m_touchFilters.end(); ) {
            TouchFilter *filter = *fit;
            ++fit;
            if (filter->filter_onTouchEnded(&p->touch)) {
                return true;
            }
        }
        
        if (p->capture) {
            ret = p->capture->sendTouchEnded(p->capture, &p->touch);
        } else if (p->hover) {
            ret = p->hover->sendTouchEnded(p->hover, &p->touch);
        }
        m_touches.erase(it);
    }
    return ret;
}

bool UIRoot::dispatchTouchCancelled(long touchId, const vector2f &position) {
    bool ret = false;
    TouchDataMap::iterator it = m_touches.find(touchId);
    if (it != m_touches.end()) {
        TouchData *p = &it->second;
        p->touch.position = position;
        
        for (TouchFilters::iterator fit = m_touchFilters.begin(); fit != m_touchFilters.end(); ) {
            TouchFilter *filter = *fit;
            ++fit;
            if (filter->filter_onTouchCancelled(&p->touch)) {
                return true;
            }
        }
        
        if (p->capture) {
            ret = p->capture->sendTouchEnded(p->capture, &p->touch);
        } else if (p->hover) {
            ret = p->hover->sendTouchEnded(p->hover, &p->touch);
        }
        m_touches.erase(it);
    }
    return ret;
}

void UIRoot::dispatchInsertText(const str32 &text) {
    if (m_focus) {
        m_focus->onInsertText(text);
    }
}

void UIRoot::dispatchDeleteBackward() {
    if (m_focus) {
        m_focus->onDeleteBackward();
    }
}

void UIRoot::dispatchMarkText(const str32 &text) {
    if (m_focus) {
        m_focus->onMarkText(text);
    }
}

void UIRoot::dispatchUnmarkText() {
    if (m_focus) {
        m_focus->onUnmarkText();
    }
}

void UIRoot::reset() {
    _resetTouches();
    _resetFocus();
}

bool UIRoot::captureTouch(UIElement *element, long touchId) {
	vnassert(element != 0);
	TouchDataMap::iterator it = m_touches.find(touchId);
	if (it == m_touches.end()) {
		return false;
	}
	TouchData &td = it->second;
	if (td.capture) {
		return false;
	}
	td.capture = element;
	td.capture->grab();
	return true;
}

void UIRoot::releaseTouch(UIElement *element, long touchId) {
	vnassert(element != 0);
	TouchDataMap::iterator it = m_touches.find(touchId);
	if (it == m_touches.end()) {
		return ;
	}
	TouchData &td = it->second;
	if (td.capture != element) {
		return ;
	}
	td.capture->drop();
	td.capture = 0;
}

void UIRoot::requireFocus(UIElement *element) {
    vnassert(element != 0);
	if (m_focus == element) {
		return ;
	}
    Application &r = Application::instance();
    if (m_focus) {
        if (r.hasInputFocus()) {
            r.releaseInputFocus();
        }
        m_focus->onFocusOut();
        m_focus->drop();
    }
    m_focus = element;
    m_focus->grab();
    m_focus->onFocusIn();
    KeyboardType kbt = m_focus->keyboardType();
    if (kbt != kKT_None) {
        r.setInputAsciiOnly(kbt == kKT_ASCIICapable);
        r.requireInputFocus();
    }
}

void UIRoot::releaseFocus(UIElement *element) {
    vnassert(element != 0);
    if (m_focus == element) {
        Application &r = Application::instance();
        if (r.hasInputFocus()) {
            r.releaseInputFocus();
        }
        m_focus->onFocusOut();
        m_focus->drop();
        m_focus = 0;
    }
}

void UIRoot::addTouchFilter(TouchFilter *filter) {
    vnassert(filter);
    m_touchFilters.push_back(filter);
}

void UIRoot::removeTouchFilter(TouchFilter *filter) {
    for (TouchFilters::iterator it = m_touchFilters.begin(); it != m_touchFilters.end(); ++it) {
        if (*it == filter) {
            m_touchFilters.erase(it);
            break;
        }
    }
}

UIRoot::TouchData & UIRoot::_newTouch(long touchId, const vector2f &position) {
	TouchData &td = m_touches[touchId];
	td.touch.touchId = touchId;
	td.touch.position = position;
	vnassert(!td.hover && !td.capture);
	return td;
}

bool UIRoot::_updateTouch(long touchId, const vector2f &position, TouchData *&touch) {
	TouchDataMap::iterator it = m_touches.find(touchId);
	if (it == m_touches.end()) {
		return false;
	}
	it->second.touch.position = position;
	touch = &it->second;
	return true;
}

void UIRoot::_updateTouches() {
	for (TouchDataMap::iterator it = m_touches.begin(); it != m_touches.end(); ++it) {
		TouchData &td = it->second;
		UIElement *p = 0;
		if (m_root && m_root->isVisible() && m_root->isEnabled()) {
			p = m_root->pick(td.touch.position);
		}
		if (td.capture && td.capture != p) {
			p = 0;
		}
        if (p != td.hover) {
            if (td.hover) {
                td.hover->onTouchLeave(&td.touch);
                td.hover->drop();
            }
            if ((td.hover = p)) {
                p->grab();
                p->onTouchEnter(&td.touch);
            }
        }
		
	}
}

void UIRoot::_updateInput() {
    
}

void UIRoot::_resetTouches() {
	for (TouchDataMap::iterator it = m_touches.begin(); it != m_touches.end(); ++it) {
		TouchData &td = it->second;
		if (td.capture) {
			td.capture->sendTouchCancelled(td.capture, &td.touch);
		} else if (td.hover) {
			td.hover->sendTouchCancelled(td.hover, &td.touch);
		}
	}
	m_touches.clear();
}

void UIRoot::_resetFocus() {
    if (m_focus) {
        Application &r = Application::instance();
        if (r.hasInputFocus()) {
            r.releaseInputFocus();
        }
        m_focus->onFocusOut();
        m_focus->drop();
        m_focus = 0;
    }
}


_vn_end
