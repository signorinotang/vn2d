//
//  vnUnpackingStream.cpp
//  vnlib
//
//  Created by Wayne on 5/23/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnUnpackingStream.h"
#include "vnAssert.h"

_vn_begin

size_t UnpackingStream::read(void *buffer, size_t size) {
    if (m_state != 3) {
        return 0;
    }
    if (size > m_size) {
        size = m_size;
    }
    _read(buffer, size);
    m_size -= size;
    if (!m_size) {
        m_state = 0;
        _update();
    }
    return size;
}

size_t UnpackingStream::write(const void *buffer, size_t size) {
    return 0;
}

bool UnpackingStream::seek(s64 offset, SeekDir way) {
    return false;
}

s64 UnpackingStream::tell() {
    return 0;
}

void UnpackingStream::append(const LinkBufferPtr &buffer) {
    m_buffers.push_back(buffer);
    m_available += buffer->size();
    _update();
}

void UnpackingStream::append(LinkBufferPtr &&buffer) {
    m_buffers.push_back(buffer);
    m_available += buffer->size();
    _update();
}

bool UnpackingStream::ready() {
    return m_state == 3;
}

size_t UnpackingStream::size() {
    return m_size;
}

void UnpackingStream::_read(void *data, size_t size) {
    vnassert(m_available >= size);
    m_available -= size;
    u8 *ptr = (u8 *)data;
    while (size) {
        vnassert(!m_buffers.empty());
        LinkBuffer *buf = m_buffers.front().ptr();
        size_t ret = buf->read(m_offset, ptr, size);
        size -= ret;
        ptr += ret;
        m_offset += ret;
        if (m_offset == buf->size()) {
            m_offset = 0;
            m_buffers.pop_front();
        }
    }
}

void UnpackingStream::_update() {
    switch (m_state) {
        case 0:
            if (!m_available) {
                break;
            } else {
                u8 lead;
                _read(&lead, 1);
                m_stream.write(&lead, 1);
                m_size = _compressedDataSizeLeadByte(lead) - 1;
                if (m_size) {
                    m_state = 1;
                } else {
                    goto l_read_size;
                }
            }
            
        case 1:
            if (m_available < m_size) {
                break;
            } else {
                u8 buf[8];
                _read(buf, m_size);
                m_stream.write(buf, m_size);
l_read_size:
                m_stream.seek(0);
                m_size = (size_t)_readCompressedUInt(m_stream);
				m_stream.seek(0);
                m_state = 2;
            }
            
        case 2:
            if (m_available < m_size) {
                break;
            }
            m_state = 3;
    }
}

_vn_end
