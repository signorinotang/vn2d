//
//  vnLinkBuffer.h
//  vnlib
//
//  Created by Wayne on 23/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnLinkBuffer_h
#define vnlib_vnLinkBuffer_h

#include "vnBuffer.h"

_vn_begin

class LinkBuffer;
typedef RefCountedPtr<LinkBuffer> LinkBufferPtr;
class LinkBufferManager;

class LinkBuffer : public Buffer {
public:
    enum { kStoregeSize = 8192 };
    
    static LinkBufferPtr create();
    
    virtual void * lock();
    virtual void unlock();
    virtual size_t size();
    virtual size_t read(size_t offset, void *data, size_t size);
    virtual size_t write(size_t offset, const void *data, size_t size);
    
    void resize(size_t size);
    void clear();
    
    size_t offset(size_t _offset);
    size_t offset();
    size_t capacity();
protected:
    LinkBuffer() = default;
    
    u8 m_data[kStoregeSize];
    size_t m_size = 0;
    size_t m_offset = 0;
};


_vn_end

#endif
