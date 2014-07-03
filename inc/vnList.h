//
//  vnList.h
//  vnlib
//
//  Created by Wayne on 5/12/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnList_h
#define vnlib_vnList_h

#include "vnRefCounted.h"
#include "vnNonCopyable.h"
#include "vnAssert.h"

#include <utility>
#include <functional>

_vn_begin

template <class T>
class List : public NonCopyable, public AllocatedObject {
public:
    class Node : public RefCounted {
        friend class List<T>;
    public:
        T * prev() const {
            return m_prev;
        }
        
        T * next() const {
            return m_next;
        }
        
    private:
        T *m_prev = 0, *m_next = 0;
    };
    
    List() = default;
    
    List(List &&other)
    : m_head(other.m_head)
    , m_tail(other.m_tail)
    , m_size(other.m_size) {
        other.m_head = 0;
        other.m_tail = 0;
        other.m_size = 0;
    }
    
    ~List() {
        clear();
    }
    
    List & operator =(List &&rhs) {
        clear();
        m_head = rhs.m_head;
        m_tail = rhs.m_tail;
        m_size = rhs.m_size;
        rhs.m_head = 0;
        rhs.m_tail = 0;
        rhs.m_size = 0;
        return *this;
    }
    
    void swap(List &other) {
        std::swap(m_head, other.m_head);
        std::swap(m_tail, other.m_tail);
        std::swap(m_size, other.m_size);
    }
    
    bool empty() const {
        return m_size == 0;
    }
    
    T * front() const {
        return m_head;
    }
    
    T * back() const {
        return m_tail;
    }
    
    void merge(List &other) {
        if (!m_size) {
            swap(other);
        } else {
            m_size += other.m_size;
            m_tail->m_next = other.m_head;
            other.m_head->m_prev = m_tail;
            m_tail = other.m_tail;
            other.m_head = 0;
            other.m_tail = 0;
            other.m_size = 0;
        }
    }
    
    void push_back(T *node, bool grab = true) {
        vnassert(node != nullptr);
        if (grab) {
            node->grab();
        }
        node->m_prev = m_tail;
        if (m_tail) {
            m_tail->m_next = node;
        } else {
            m_head = node;
        }
        ++m_size;
        m_tail = node;
    }
    
    void pop_back() {
        vnassert(m_tail);
        T *p = m_tail;
        if ((m_tail = p->m_prev)) {
            m_tail->m_next = nullptr;
            --m_size;
        } else {
            m_head = nullptr;
            m_size = 0;
        }
        p->drop();
    }
    
    void push_front(T *node, bool grab = true) {
        vnassert(node != nullptr);
        if (grab) {
            node->grab();
        }
        node->m_next = m_head;
        if (m_head) {
            m_head->m_prev = node;
        } else {
            m_tail = node;
        }
        ++m_size;
        m_head = node;
    }
    
    void pop_front() {
        vnassert(m_head);
        T *p = m_head;
        if ((m_head = p->m_next)) {
            m_head->m_prev = nullptr;
            --m_size;
        } else {
            m_tail = nullptr;
            m_size = 0;
        }
        p->drop();
    }
    
    void erase(T *node) {
        vnassert(node != nullptr);
        if (node->m_prev) {
            node->m_prev->m_next = node->m_next;
        }
        if (node->m_next) {
            node->m_next->m_prev = node->m_prev;
        }
        if (node == m_head) {
            m_head = node->m_next;
        }
        if (node == m_tail) {
            m_tail = node->m_prev;
        }
        node->m_prev = nullptr;
        node->m_next = nullptr;
        node->drop();
        --m_size;
    }
    
    void clear() {
        while (m_head) {
			T *p = m_head;
			m_head = m_head->m_next;
            p->m_prev = nullptr;
            p->m_next = nullptr;
            p->drop();
        }
        m_tail = nullptr;
        m_size = 0;
    }
    
    void clearNotDrop() {
        m_head = nullptr;
        m_tail = nullptr;
        m_size = 0;
    }
    
protected:
    T *m_head = nullptr, *m_tail = nullptr;
    size_t m_size = 0;
};

_vn_end

#endif
