//
//  vnRefCounted.h
//  vnbase
//
//  Created by Wayne on 13-9-15.
//  Copyright (c) 2013 viichi.com. All rights reserved.
//

#ifndef vnbase_vnRefCounted_h
#define vnbase_vnRefCounted_h

#include "vnAllocatedObject.h"
#include "vnAtomic.h"

_vn_begin

class _vn_base_api RefCounted : public AllocatedObject {
public:
	RefCounted() : m_ref_count(1) {}
	RefCounted(const RefCounted &) : m_ref_count(1) {}
	virtual ~RefCounted() {}
	
	RefCounted & operator =(const RefCounted &) { return *this; }
	
	int ref_count() const { return m_ref_count; }
	
	void grab() const { Atomic::increase(m_ref_count); }
	
	bool drop() const {
		vnassert(m_ref_count > 0);
		if (!Atomic::decrease(m_ref_count)) {
			_release();
			return true;
		}
		return false;
	}
	
protected:
	virtual void _release() const { delete this; }
	
private:
	mutable Atomic::Int32 m_ref_count;
};

template <class _RefCounted>
class RefCountedPtr {
public:
	RefCountedPtr() : m_pointee(0) {}
	RefCountedPtr(_RefCounted *ptr) : m_pointee(ptr) {}
	RefCountedPtr(const RefCountedPtr &rhs) : m_pointee(rhs.m_pointee) {
		if (m_pointee) {
			m_pointee->grab();
		}
	}
	
	template <class T>
	RefCountedPtr(const RefCountedPtr<T> &rhs) : m_pointee(rhs.m_pointee) {
		if (m_pointee) {
			m_pointee->grab();
		}
	}
	
	~RefCountedPtr() {
		release();
	}
	
	void swap(RefCountedPtr &rhs) {
		_RefCounted *t = rhs.m_pointee;
		rhs.m_pointee = m_pointee;
		m_pointee = t;
	}
	
	RefCountedPtr & operator =(const RefCountedPtr &rhs) {
		if (rhs.m_pointee != m_pointee) {
			RefCountedPtr t( rhs );
			swap(t);
		}
		return *this;
	}
	
	RefCountedPtr & operator =( _RefCounted *rhs ) {
		RefCountedPtr t(rhs);
		swap(t);
		return *this;
	}
	
	_RefCounted * operator ->() const {
		return m_pointee;
	}
	
	_RefCounted & operator *() const {
		return *m_pointee;
	}
	
	_RefCounted * ptr() const {
		return m_pointee;
	}
	
	bool null() const {
		return m_pointee == 0;
	}
	
	s32 ref_count() const {
		if (!m_pointee) {
			return 0;
		}
		return m_pointee->ref_count();
	}
	
	template <class T>
	RefCountedPtr<T> cast() const {
		if (!m_pointee) {
			return RefCountedPtr<T>();
		}
		T *p = static_cast<T *>(m_pointee);
		p->grab();
		return p;
	}
	
	template <class T>
	RefCountedPtr<T> dyn_cast() const {
		T *p = dynamic_cast<T *>(m_pointee);
		if (p) {
			p->grab();
		}
		return p;
	}
	
	void release() {
		if ( m_pointee ) {
			m_pointee->drop();
			m_pointee = 0;
		}
	}
    
    _RefCounted * move() {
        _RefCounted *t = m_pointee;
        m_pointee = 0;
        return t;
    }
    
    static const RefCountedPtr kNull;
	
private:
	_RefCounted *m_pointee;
    
};

template <class _RefCounted>
const RefCountedPtr<_RefCounted> RefCountedPtr<_RefCounted>::kNull;


_vn_end

#define VN_SAFE_DROP(ptr) if (ptr) { ptr->drop(); ptr = 0; }

#endif
