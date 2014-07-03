//
//  vnLinkImpl.h
//  vn2d
//
//  Created by Cloverz on 6/18/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnLinkImpl_h
#define vn2d_vnLinkImpl_h

#import <CoreFoundation/CFSocket.h>

#include <netinet/in.h>
#include "vnLink.h"

_vn_begin

class LinkImpl : public Link, public AllocatedObject
{
private:
    static void _socket_event(CFSocketRef socket, CFSocketCallBackType type, CFDataRef address, const void *data, void *info);
public:
    LinkImpl(LinkHandler *linkHandler);
    ~LinkImpl();
    
    virtual State state() const;
    virtual bool connect(const c8 *addr, u16 port);
    virtual bool send(const LinkBufferPtr &buffer);
    virtual void close();
private:
    bool _init(const c8 *addr, u16 port);
    void _recv();
    
    State m_state;
    CFSocketRef m_socket;
    sockaddr_in m_addr;
    CFRunLoopSourceRef m_runloop_src;
    LinkHandler *m_handler;
};

_vn_end

#endif
