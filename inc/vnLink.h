//
//  vnLink.h
//  vn2d
//
//  Created by Wayne on 6/18/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vn2d_vnLink_h
#define vn2d_vnLink_h

#include "vnLinkBuffer.h"

_vn_begin

class Link {
public:
    enum State {
        kClosed = 0,
        kConnecting,
        kConnected
    };
    
    virtual State state() const = 0;
    
    virtual bool connect(const c8 *addr, u16 port) = 0;
    virtual bool send(const LinkBufferPtr &buffer) = 0;
    virtual void close() = 0;
    
};

class LinkHandler {
public:
    virtual void init(Link *link) {}
    virtual void fini() {}
    virtual void onConnected() {}
    virtual void onDisconnected() {}
    virtual void onConnectFailed() {}
    virtual void onReceived(const LinkBufferPtr &buffer) {}
};

_vn_end

#endif
