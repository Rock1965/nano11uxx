/*
 ===============================================================================
 Name        : gc.cpp
 Author      : uCXpresso Group
 Version     : v1.0.1
 Date		 : 2014/3/24
 Copyright   : Copyright (C) www.embeda.com.tw
 License	 : MIT
 Description : Simple Garbage Collector Template Class
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2014/3/24	v1.0.0	First Edition									Jason
 2014/3/25	v1.0.1	Add Mutex Lock for Multi-Tasking.				Jason
 ===============================================================================
 */
#ifndef GC_TEMPLATE
#define GC_TEMPLATE

#include <string.h>
#include "class/gc.h"

template <class CType>
gcHandleT<CType>::gcHandleT(CType *elem) {
	if ( elem ) {
		m_gc = new GC_T;
		if ( m_gc ) {
			m_gc->refcount = 1;
			m_gc->elem = elem;
		}
	} else {
		m_gc = NULL;
	}
}

template <class CType>
gcHandleT<CType>::gcHandleT(const gcHandleT<CType> &gc) {
	m_gc = gc.m_gc;
	reference();
}

template <class CType>
gcHandleT<CType>::~gcHandleT() {
	release();
}

template <class CType>
void gcHandleT<CType>::release() {
	if ( m_gc ) {
#if GC_FOR_MULTITASKING
		m_gc->mutex.lock();
#endif
		if ( m_gc->refcount ) {
			m_gc->refcount--;
			if ( m_gc->refcount==0 ) {
				// if refcount=0, free the m_gc source
				delete ((CType *)m_gc->elem);
				delete m_gc;
			}
		}
#if GC_FOR_MULTITASKING
		m_gc->mutex.unlock();
#endif
		m_gc = NULL;
	}
}

template <class CType>
void gcHandleT<CType>::reference() {
	if ( m_gc ) {
#if GC_FOR_MULTITASKING
		m_gc->mutex.lock();
#endif
		m_gc->refcount++;

#if GC_FOR_MULTITASKING
		m_gc->mutex.unlock();
#endif
	}
}

template <class CType>
gcHandleT<CType>& gcHandleT<CType>::operator = (gcHandleT<CType> &gc) {
	// releases old
	release();

	// refers to new
	m_gc = gc.m_gc;
	reference();
	return *this;
}

template <class CType>
gcHandleT<CType>& gcHandleT<CType>::operator = (CType* elem) {
	// releases old
	release();

	// refers to new without count
	if ( elem ) {
		m_gc = new GC_T;
		if ( m_gc ) {
			m_gc->elem = elem;
			m_gc->refcount = 1;
		}
	}
	return *this;
}

template <class CType>
void gcHandleT<CType>::operator = (xHandle gc) {
	// releases old
	release();

	// refers to new without count
	m_gc = (GC_T *) gc;
}

template <class CType>
void gcHandleT<CType>::weakTo (gcHandleT<CType> &gc) {
	// dereference to old
	release();

	// reference to new object without count
	m_gc = gc.m_gc;
}

template <class CType>
xHandle gcHandleT<CType>::clone() {
	GC_T *gc;
	gc = new GC_T;
	if ( gc ) {
		gc->elem = new CType;
		if ( gc->elem ) {
			memcpy(gc->elem, m_gc->elem, sizeof(CType));
			gc->refcount = 1;
			return gc;
		}
		delete gc;
	}
	return NULL;
}

template <class CType>
CType* gcHandleT<CType>::operator ->() {
	if ( m_gc ) {
		return (CType *)m_gc->elem;
	}
	return NULL;
}

template <class CType>
gcHandleT<CType>::operator CType *() {
	if ( m_gc ) {
		return (CType *)m_gc->elem;
	}
	return NULL;
}

template <class CType>
CType& gcHandleT<CType>::operator *() {
	return *((CType *)m_gc->elem);
}

template <class CType>
xHandle gcHandleT<CType>::operator &() {
	reference();
	return m_gc;
}

template <class CType>
gcHandleT<CType>::operator xHandle () {
//	reference(); // mask, and increase reference count by user's condition.
	return m_gc;
}

template <class CType>
int gcHandleT<CType>::count() {
	if ( m_gc ) {
		return m_gc->refcount;
	}
	return 0;
}

template <class CType>
bool gcHandleT<CType>::operator == (gcHandleT<CType> &gc) {
	return ((m_gc==gc.m_gc) ? true : false);
}

template <class CType>
bool gcHandleT<CType>::operator != (gcHandleT<CType> &gc) {
	return ((m_gc!=gc.m_gc) ? true : false);
}

template <class CType>
bool gcHandleT<CType>::operator == (CType *elem) {
	if ( elem ) {
		return ((m_gc->elem==elem) ? true : false);
	} else {
		return ((m_gc==NULL) ? true : false);
	}
}

template <class CType>
bool gcHandleT<CType>::operator != (CType *elem) {
	if ( elem ) {
		return ((m_gc->elem!=elem) ? true : false);
	} else {
		return ((m_gc!=NULL) ? true : false);
	}
}

#endif // GC_TEMPLATE
