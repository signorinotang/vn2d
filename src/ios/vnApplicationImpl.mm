//
//  vnApplicationImpl.cpp
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014年 viichi.com. All rights reserved.
//

#include "vnApplicationImpl.h"
#include "vnGLESView.h"
#include "vnGLESViewController.h"
#include "vnLinkImpl.h"

#import <UIKit/UIKit.h>

static UIWindow *g_window = 0;
static GLESView *g_view = 0;
static GLESViewController *g_controller = 0;

_vn_begin

ApplicationImpl::ApplicationImpl()
: m_type(Application::kType_iPad)
, m_scale(1)
, m_viewSize(1024, 768)
, m_delegate(0)
, m_device(0) {
    
}

ApplicationImpl::~ApplicationImpl() {
    VN_SAFE_DELETE(m_device);
}

int ApplicationImpl::main(Application::Delegate *delegate, bool horizontal) {
    vnassert(delegate);
    m_delegate = delegate;
    _init(horizontal);
    _createView();

    m_resourcePath = [[[NSBundle mainBundle] resourcePath] UTF8String];
    m_resourcePath.push_back('/');
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    
	m_cachePath = [[paths objectAtIndex:0] UTF8String];
    m_cachePath.push_back('/');
	
	paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	m_documentPath = [[paths objectAtIndex:0] UTF8String];
	
	//paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
	//m_libraryPath = [[paths objectAtIndex:0] UTF8String];
    //m_libraryPath.push_back('/');
	
	m_tempPath = [NSTemporaryDirectory() UTF8String];
    m_tempPath.push_back('/');
    
    
    m_delegate->onInitialize();
    m_delegate->onUpdate(0);
    return 0;
}

Application::Type ApplicationImpl::type() {
    return m_type;
}

f32 ApplicationImpl::scale() {
    return m_scale;
}

const vector2i & ApplicationImpl::viewSize() {
    return m_viewSize;
}

Device * ApplicationImpl::createDevice(const vector2f &size) {
    m_device = vnnew DeviceImpl(this);
    if (!m_device->_init(size, m_viewSize, m_viewSize * m_scale/*vector2i(g_view.drawableWidth, g_view.drawableHeight)*/)) {
        delete m_device;
        m_device = 0;
    }
    return m_device;
}

Link * ApplicationImpl::createLink(LinkHandler *handler) {
    return vnnew LinkImpl(handler);
}

const str8 & ApplicationImpl::resourcePath() {
    return m_resourcePath;
}

const str8 & ApplicationImpl::cachePath() {
    return m_cachePath;
}

const str8 & ApplicationImpl::tempPath() {
    return m_tempPath;
}

const str8 & ApplicationImpl::documentPath() {
    return m_documentPath;
}

void ApplicationImpl::requireInputFocus() {
    [g_view becomeFirstResponder];
}

bool ApplicationImpl::hasInputFocus() {
    return [g_view isFirstResponder] == YES;
}

void ApplicationImpl::releaseInputFocus() {
    [g_view resignFirstResponder];
}

void ApplicationImpl::setInputAsciiOnly(bool flag) {
    [g_view setInputAsciiOnly:flag];
}

bool ApplicationImpl::isInputAsciiOnly() {
    return [g_view isInputAsciiOnly];
}

void ApplicationImpl::_init(bool horizontal) {
    CGRect rect = [[UIScreen mainScreen] bounds];
	f32 aspect = (f32)rect.size.height / (f32)rect.size.width;
    if (aspect == 4.f / 3.f) {
		m_type = Application::kType_iPad;
	} else if (aspect == 3.f / 2.f) {
        m_type = Application::kType_iPhone_3_5;
    } else {
        m_type = Application::kType_iPhone_4;
    }
    m_scale = [[UIScreen mainScreen] scale];
    if (horizontal) {
        m_viewSize.x = rect.size.height;
        m_viewSize.y = rect.size.width;
    } else {
        m_viewSize.x = rect.size.width;
        m_viewSize.y = rect.size.height;
    }
}

void ApplicationImpl::_createView() {
    CGRect rect = [[UIScreen mainScreen] bounds];
    g_window = [[UIWindow alloc] initWithFrame:rect];
    g_view = [[GLESView alloc] initWithFrame:rect appImpl:this];
    g_controller = [[GLESViewController alloc] initWithAppImpl:this];
    [g_controller setView:g_view];
    [g_window setRootViewController:g_controller];
    [g_window makeKeyAndVisible];
}

void ApplicationImpl::_view_update(vn::f32 deltaTime) {
    m_delegate->onUpdate(deltaTime);
}

void ApplicationImpl::_view_render() {
    m_delegate->onRender(m_device);
}

void ApplicationImpl::_view_suspend() {
    m_delegate->onSuspend();
}

void ApplicationImpl::_view_resume() {
    m_delegate->onResume();
}

void ApplicationImpl::_view_touchBegan(long tid, const vector2f &pt) {
    m_delegate->onTouchBegan(tid, pt);
}

void ApplicationImpl::_view_touchMoved(long tid, const vector2f &pt) {
    m_delegate->onTouchMoved(tid, pt);
}

void ApplicationImpl::_view_touchEnded(long tid, const vector2f &pt) {
    m_delegate->onTouchEnded(tid, pt);
}

void ApplicationImpl::_view_touchCancelled(long tid, const vector2f &pt) {
    m_delegate->onTouchCancelled(tid, pt);
}

void ApplicationImpl::_defaultFrameBuffer_bind() {
    [g_view bindDrawable];
}

void ApplicationImpl::_text_insert(const str32 &text) {
    m_delegate->onInsertText(text);
}

void ApplicationImpl::_text_mark(const str32 &text) {
    m_delegate->onMarkText(text);
}

void ApplicationImpl::_text_unmark() {
    m_delegate->onUnmarkText();
}

void ApplicationImpl::_text_deleteBackward() {
    m_delegate->onDeleteBackward();
}

void ApplicationImpl::_keyboard_willShow() {
    m_delegate->onKeyboardWillShow();
}

void ApplicationImpl::_keyboard_didShow() {
    m_delegate->onKeyboardDidShow();
}

void ApplicationImpl::_keyboard_willHide() {
    m_delegate->onKeyboardWillHide();
}

void ApplicationImpl::_keyboard_didHide() {
    m_delegate->onKeyboardDidHide();
}

_vn_end