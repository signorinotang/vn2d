//
//  vnApplicationImpl.h
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014年 viichi.com. All rights reserved.
//

#ifndef __vn2d__vnApplicationImpl__
#define __vn2d__vnApplicationImpl__

#include "vnAllocatedObject.h"
#include "vnApplication.h"
#include "vnDeviceImpl.h"

_vn_begin

class ApplicationImpl : public AllocatedObject {
public:
    ApplicationImpl();
    ~ApplicationImpl();
    
    int main(Application::Delegate *delegate, bool horizontal);
    
    Application::Type type();
    f32 scale();
    const vector2i & viewSize();
    
    Device * createDevice(const vector2f &size);
    Link * createLink(LinkHandler *handler);
    
    const str8 & resourcePath();
    const str8 & cachePath();
    const str8 & tempPath();
    const str8 & documentPath();
    
    void requireInputFocus();
    bool hasInputFocus();
    void releaseInputFocus();
    
    void setInputAsciiOnly(bool flag);
    bool isInputAsciiOnly();
    
    void _view_update(vn::f32 deltaTime);
    void _view_render();
    
    void _view_suspend();
    void _view_resume();
    
    void _view_touchBegan(long tid, const vector2f &pt);
    void _view_touchMoved(long tid, const vector2f &pt);
    void _view_touchEnded(long tid, const vector2f &pt);
    void _view_touchCancelled(long tid, const vector2f &pt);
    
    void _defaultFrameBuffer_bind();
    
    void _text_insert(const str32 &text);
    void _text_mark(const str32 &text);
    void _text_unmark();
    void _text_deleteBackward();
    
    void _keyboard_willShow();
	void _keyboard_didShow();
	void _keyboard_willHide();
	void _keyboard_didHide();
    
protected:
    Application::Type m_type;
    f32 m_scale;
    vector2i m_viewSize;
    str8 m_resourcePath;
    str8 m_cachePath;
    str8 m_tempPath;
    str8 m_documentPath;
    DeviceImpl *m_device;
    Application::Delegate *m_delegate;
    
    
    
    void _init(bool horizontal);
    void _createView();
    
};

_vn_end

#endif /* defined(__vn2d__vnApplicationImpl__) */
