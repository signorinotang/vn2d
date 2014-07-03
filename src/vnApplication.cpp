//
//  vnApplication.cpp
//  vn2d
//
//  Created by Wayne on 14-1-9.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnApplication.h"

#if VN_PLATFORM == VN_PLATFORM_IOS
#   include "ios/vnApplicationImpl.h"
#elif VN_PLATFORM == VN_PLATFORM_MAC
#   include "osx/vnApplicationImpl.h"
#elif VN_PLATFORM == VN_PLATFORM_WIN
#   include "win/vnApplicationImpl.h"
#endif

_vn_begin

Application::Application()
: m_impl(vnnew ApplicationImpl()) {
    
}

Application::~Application() {
    delete m_impl;
}

int Application::main(Delegate *delegate, bool horizontal) {
    return instance().m_impl->main(delegate, horizontal);
}

Application::Type Application::type() {
    return m_impl->type();
}

f32 Application::scale() {
    return m_impl->scale();
}

const vector2i & Application::viewSize() {
    return m_impl->viewSize();
}

Device * Application::createDevice(const vector2f &size) {
    return m_impl->createDevice(size);
}

Link * Application::createLink(LinkHandler *handler) {
    //return m_impl->createLink(handler);
	return 0;
}

const str8 & Application::resourcePath() {
    return m_impl->resourcePath();
}

const str8 & Application::cachePath() {
    return m_impl->cachePath();
}

const str8 & Application::tempPath() {
    return m_impl->tempPath();
}

const str8 & Application::documentPath() {
    return m_impl->documentPath();
}

void Application::requireInputFocus() {
    m_impl->requireInputFocus();
}

bool Application::hasInputFocus() {
    return m_impl->hasInputFocus();
}

void Application::releaseInputFocus() {
    m_impl->releaseInputFocus();
}

void Application::setInputAsciiOnly(bool flag) {
    m_impl->setInputAsciiOnly(flag);
}

bool Application::isInputAsciiOnly() {
    return m_impl->isInputAsciiOnly();
}

_vn_end
