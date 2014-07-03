//
//  vnPackingStream.cpp
//  vnlib
//
//  Created by Wayne on 23/5/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnPackingStream.h"


_vn_begin

PackingStream::PackingStream() {
    m_buffers.push_back(LinkBuffer::create());
    m_buffers.back()->offset(9);
}

size_t PackingStream::read(void *buffer, size_t size) {
    return 0;
}

size_t PackingStream::write(const void *buffer, size_t size) {
    size_t remaining = size;
    const u8 *ptr = (const u8 *)buffer;
    while (remaining) {
        if (m_current == m_buffers.size()) {
            m_buffers.push_back(LinkBuffer::create());
        }
        LinkBuffer *buf = m_buffers[m_current].ptr();
        size_t bytes = buf->write(m_offset, ptr, remaining);
        remaining -= bytes;
        ptr += remaining;
        m_offset += bytes;
        if (m_offset == buf->capacity()) {
            ++m_current;
            m_offset = 0;
        }
    }
    m_size += size;
    return size;
}

bool PackingStream::seek(s64 offset, SeekDir way) {
    return false;
}

s64 PackingStream::tell() {
    return m_size;
}

std::vector<LinkBufferPtr> PackingStream::pack() {
    size_t s = _compressedDataSizeUInt(m_size);
    const LinkBufferPtr &buf = m_buffers.front();
    buf->offset(9 - s);
    BufferStream(buf.ptr()) << DataStream::ccu(m_size);
    std::vector<LinkBufferPtr> ret = std::move(m_buffers);
    m_buffers.push_back(LinkBuffer::create());
    m_buffers.back()->offset(9);
    m_offset = 0;
    m_current = 0;
    m_size = 0;
    return std::move(ret);
}








_vn_end
