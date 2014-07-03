//
//  vnLinkImpl.c
//  vn2d
//
//  Created by Cloverz on 6/18/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include <netdb.h>

#include "vnLinkImpl.h"

_vn_begin

void LinkImpl::_socket_event(CFSocketRef socket, CFSocketCallBackType type, CFDataRef address, const void *data, void *info)
{
    LinkImpl *_this = (LinkImpl *)info;
	switch (type)
    {
		case kCFSocketConnectCallBack:
			if (data)
            {
                _this->m_handler->onConnectFailed();
			}
            else
            {
                _this->m_state = kConnected;
				_this->m_handler->onConnected();
			}
			break;
		case kCFSocketReadCallBack:
			_this->_recv();
			break;
	}
}

LinkImpl::LinkImpl(LinkHandler *linkHandler) :m_socket(0), m_handler(linkHandler), m_state(kClosed), m_addr({}), m_runloop_src(0)
{
    m_handler->init(this);
}

LinkImpl::~LinkImpl() { m_handler = 0; }

Link::State LinkImpl::state() const
{
    return kClosed;
}

bool LinkImpl::connect(const c8 *addr, u16 port)
{
    if (!_init(addr, port))
        return false;
    
    /* Create socket. */
    CFSocketContext tag = {0, this};
    m_socket = CFSocketCreate(kCFAllocatorDefault, AF_INET, SOCK_STREAM, IPPROTO_TCP, kCFSocketConnectCallBack | kCFSocketReadCallBack, _socket_event, &tag);
    if (!m_socket)
        return false;
    
    /* Convert address in sockaddr_in to data. */
    CFDataRef data = CFDataCreate(kCFAllocatorDefault, (unsigned char *)&m_addr, sizeof(m_addr));
	CFSocketError e = CFSocketConnectToAddress(m_socket, data, -1);
    if (e != kCFSocketSuccess)
    {
        printf("ERROR: Connect failed!");
        return false;
    }
    
    /* Create a loop source, and put it to the Runloop. */
    if (m_runloop_src)
        CFRelease(m_runloop_src);
    
    m_runloop_src = CFSocketCreateRunLoopSource(kCFAllocatorDefault, m_socket, 0);
	CFRunLoopAddSource(CFRunLoopGetCurrent(), m_runloop_src, kCFRunLoopCommonModes);
    CFRunLoopRun();
    
    m_state = kConnecting;
    
    return true;
}

bool LinkImpl::send(const LinkBufferPtr &buffer)
{
    if (!m_socket)
		return false;
    
    ssize_t ret = ::send(CFSocketGetNative(m_socket), buffer->lock(), buffer->size(), 0);
    if (ret < 0)
        return false;
    
    return true;
}

void LinkImpl::close()
{
    m_handler->fini();
    
    if (!m_socket)
		return;
    
	CFSocketInvalidate(m_socket);
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(), m_runloop_src, kCFRunLoopCommonModes);
	CFRelease(m_runloop_src);
	CFRelease(m_socket);
    
	m_socket = 0;
    m_addr = {};
    m_runloop_src = 0;
    
    delete this;
}

bool LinkImpl::_init(const c8 *addr, u16 port)
{
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);
    
    hostent *ent = gethostbyname(addr);
    if (ent && ent->h_addrtype == AF_INET && ent->h_length)
        m_addr.sin_addr.s_addr = *(unsigned int *)ent->h_addr_list[0];
    else
    {
        return false;
    }
    
    return true;
}

void LinkImpl::_recv()
{
    LinkBufferPtr buf = LinkBuffer::create();
    size_t capacity = buf->capacity();
	ssize_t bytes = ::recv(CFSocketGetNative(m_socket), buf->lock(), capacity, 0);
    buf->unlock();
    buf->resize(bytes);
	if (bytes < 0)
    {
		/* Error. */
		m_handler->onDisconnected();
	}
    else if (bytes == 0)
    {
		/* Disconnect. */
        m_handler->onDisconnected();
	}
    else
    {
		this->m_handler->onReceived(buf);
	}
}

_vn_end
