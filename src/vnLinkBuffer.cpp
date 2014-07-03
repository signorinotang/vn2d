//
//  vnLinkBuffer.cpp
//  vnlib
//
//  Created by Wayne on 23/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnLinkBuffer.h"

_vn_begin

void * LinkBuffer::lock() {
    return m_data + m_offset;
}

void LinkBuffer::unlock() {
    
}

size_t LinkBuffer::size() {
    return m_size;
}

size_t LinkBuffer::capacity() {
    return kStoregeSize - m_offset;
}

size_t LinkBuffer::offset() {
    return m_offset;
}

size_t LinkBuffer::offset(size_t _offset) {
    if (_offset > kStoregeSize) {
        _offset = kStoregeSize;
    }
    ssize_t size = m_size;
    size += (ssize_t)m_offset - (ssize_t)_offset;
    if (size < 0) {
        m_size = 0;
    } else {
        m_size = size;
    }
    return m_offset = _offset;
}

size_t LinkBuffer::read(size_t offset, void *data, size_t size) {
    if (offset >= m_size || !size) {
        return 0;
    }
    size_t rest = m_size - offset;
    if (size > rest) {
        size = rest;
    }
    memcpy(data, (u8 *)m_data + m_offset + offset, size);
    return size;
}

size_t LinkBuffer::write(size_t offset, const void *data, size_t size) {
    size_t c = kStoregeSize - m_offset;
    if (offset >= c || !size) {
		return 0;
	}
	size_t t = c - offset;
	if (size > t) {
		size = t;
	}
	memcpy((u8 *)m_data + m_offset + offset, data, size);
    t = offset + size;
    if (m_size < t) {
        m_size = t;
    }
    return size;
}

void LinkBuffer::resize(size_t size) {
    size_t c = kStoregeSize - m_offset;
    if (size > c) {
        m_size = c;
    } else {
        m_size = size;
    }
}

void LinkBuffer::clear() {
    m_size = 0;
}

LinkBufferPtr LinkBuffer::create() {
    return vnnew LinkBuffer();
}

_vn_end